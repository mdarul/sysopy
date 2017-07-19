#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TRY_SEEK(RESULT) if ((RESULT) == -1) \
{ \
    perror("failed to seek"); \
    abort(); \
}

void help_communicate() {
    printf("Usage:\n"
           "./program generate <file name> <records> <record_size>\n"
           "./program shuffle sys/lib <file name> <records_amount> <record_size>\n"
           "./program sort sys/lib <file name> <records_amount> <record_size>\n");
}

void generate(const char *file_name, size_t records_amount, size_t size)
{
    FILE *source = fopen("/dev/random", "r");
    FILE *destination = fopen(file_name, "w");

    if (source == NULL)
    {
        printf("Cannot open /dev/random!\n");
        abort();
    }
    if (destination == NULL)
    {
        printf("Cannot open output file '%s'!\n", file_name);
        abort();
    }

    void *buffer = malloc(size);
    if(!buffer) abort();

    for (int i = 0; i < records_amount; i++)
    {
        size_t rr = fread(buffer, size, 1, source);
        fwrite(buffer, size, rr, destination);
    }

    fclose(destination);
    fclose(source);
    free(buffer);
}

void shuffle_sys(const char *file_name, size_t records, size_t size) {
    int file = open(file_name, O_RDWR); // open with read-write mode
    if (file == -1) {
        perror("cannot open output file!");
        abort();
    }

    void *buffer_a = malloc(size);
    void *buffer_b = malloc(size);
    if(!buffer_a || !buffer_b) abort();

    for (size_t i = 0; i < records - 1; ++i)
    {
        size_t j = rand() % records;
        
        TRY_SEEK(lseek(file, i * size, SEEK_SET));
        ssize_t line_a = read(file, buffer_a, size);
        TRY_SEEK(lseek(file, j * size, SEEK_SET));
        ssize_t line_b = read(file, buffer_b, size);

        lseek(file, i * size, SEEK_SET);
        write(file, buffer_b, (size_t) line_b);
        lseek(file, j * size, SEEK_SET);
        write(file, buffer_a, (size_t) line_a);
    }

    free(buffer_b);
    free(buffer_a);
    close(file);
}

void shuffle_lib(const char *file_name, size_t records, size_t size) {
    FILE *file = fopen(file_name, "r+");
    if (file == NULL)
    {
        perror("cannot open output file!");
        abort();
    }

    void *buffer_a = malloc(size);
    void *buffer_b = malloc(size);
    if(!buffer_a || !buffer_b) abort();

    for (size_t i = 0; i < records - 1; ++i)
    {
        size_t j = rand() % records;

        TRY_SEEK(fseek(file, i * size, SEEK_SET));
        size_t lina_a = fread(buffer_a, size, 1, file);
        TRY_SEEK(fseek(file, j * size, SEEK_SET));
        size_t line_b = fread(buffer_b, size, 1, file);

        fseek(file, i * size, SEEK_SET);
        fwrite(buffer_b, line_b, 1, file);
        fseek(file, j * size, SEEK_SET);
        fwrite(buffer_a, lina_a, 1, file);
    }

    free(buffer_b);
    free(buffer_a);
    fclose(file);
}

void sort_sys(const char *file_name, size_t records_amount, size_t record_size) {
    int file = open(file_name, O_RDWR);
    if (file == -1)
    {
        printf("Cannot open output file!\n");
        abort();
    }

    void *buffer_a = malloc(record_size);
    void *buffer_b = malloc(record_size);
    if(!buffer_a || !buffer_b) abort();

    bool was_swapped = false;
    do
    {
        was_swapped = false;

        for (size_t i = 0; i < records_amount - 1; i++)
        {
            TRY_SEEK(lseek(file, i * record_size, SEEK_SET));
            ssize_t line_a = read(file, buffer_a, record_size);
            TRY_SEEK(lseek(file, (i+1) * record_size, SEEK_SET));
            ssize_t line_b = read(file, buffer_b, record_size);

            unsigned char key_line_a = ((unsigned char *) buffer_a)[0];
            unsigned char key_line_b = ((unsigned char *) buffer_b)[0];

            if (key_line_a > key_line_b)
            {
                TRY_SEEK(lseek(file, i * record_size, SEEK_SET));
                write(file, buffer_b, (size_t) line_b);
                TRY_SEEK(lseek(file, (i+1) * record_size, SEEK_SET));
                write(file, buffer_a, (size_t) line_a);

                was_swapped = true;
            }
        }
    }
    while (was_swapped);

    close(file);
    free(buffer_b);
    free(buffer_a);
}

void sort_lib(const char *file_name, size_t records_amount, size_t record_size) {
    FILE *file = fopen(file_name, "r+");
    if (!file)
    {
        printf("Cannot open output file!\n");
        abort();
    }

    void *buffer_a = malloc(record_size);
    void *buffer_b = malloc(record_size);
    if(!buffer_a || !buffer_b) abort();

    bool was_swapped = false;
    do
    {
        was_swapped = false;

        for (size_t i = 0; i < records_amount - 1; i++)
        {
            TRY_SEEK(fseek(file, i * record_size, SEEK_SET));
            size_t line_a = fread(buffer_a, record_size, 1, file);
            TRY_SEEK(fseek(file, (i+1) * record_size, SEEK_SET));
            size_t line_b = fread(buffer_b, record_size, 1, file);

            unsigned char key_line_a = ((unsigned char *) buffer_a)[0];
            unsigned char key_line_b = ((unsigned char *) buffer_b)[0];

            if(key_line_a > key_line_b)
            {
                TRY_SEEK(fseek(file, i * record_size, SEEK_SET));
                fwrite(buffer_b, line_b, 1, file);
                TRY_SEEK(fseek(file, (i+1) * record_size, SEEK_SET));
                fwrite(buffer_a, line_a, 1, file);
                was_swapped = true;
            }
        }
    }
    while (was_swapped);

    fclose(file);
    free(buffer_b);
    free(buffer_a);
}

int main(int argc, char *argv[])
{
    srand((unsigned int) time(NULL));

    const char *operation = argv[1];

    if (argc == 5 && strcmp(operation, "generate") == 0)
    {
        const char *file_name = argv[2];
        size_t records_amount = (size_t) atoi(argv[3]);
        size_t record_size = (size_t) atoi(argv[4]);
        generate(file_name, records_amount, record_size);
    }
    else if (argc == 6)
    {
        const char *method = argv[2];
        const char *file_name = argv[3];
        size_t records = (size_t) atoi(argv[4]);
        size_t record_size = (size_t) atoi(argv[5]);

        if (strcmp(operation, "shuffle") == 0)
        {
            if (strcmp(method, "sys") == 0) shuffle_sys(file_name, records, record_size);
            else if (strcmp(method, "lib") == 0) shuffle_lib(file_name, records, record_size);
        }
        else if (strcmp(operation, "sort") == 0)
        {
            if (strcmp(method, "sys") == 0) sort_sys(file_name, records, record_size);
            else if (strcmp(method, "lib") == 0) sort_lib(file_name, records, record_size);
        }
        else help_communicate();
    }
    else help_communicate();

    return 0;
}