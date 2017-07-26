#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>


int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("usage: program <pipe_file> <dimension>\n");
        exit(EXIT_FAILURE);
    }

    char *fifo_path = argv[1];
    int R = atoi(argv[2]);

    int **tab = malloc(sizeof(int*)*R);
    for(int i = 0; i < R; i++)tab[i] = malloc(sizeof(int)*R);
    for(int i = 0; i < R; i++)
        for(int j = 0; j < R; j++)
            tab[i][j] = 0;


    FILE *fifo_pointer;
    if( access(fifo_path , F_OK ) != -1 )
    {
        remove(fifo_path);
    }
    if(mkfifo(fifo_path , 0777) < 0)
    {
        perror("mkfifo failed\n");
        exit(EXIT_FAILURE);
    }
    fifo_pointer = fopen(fifo_path, "r");
    double re,im;
    int val;
    while(fscanf(fifo_pointer,"%lf %lf %d\n",&re,&im,&val) != EOF) // scanning data from operating memory using fifo
    {
        int i,j;
        i = (int) ((re+2)/3 * R); // we add 2, so we'll only have i>=0. Thanks to that, we can get kind of % of re/max, which we multiply by R and have position in table
        j = (int) ((im+1)/2 * R); // as above
        tab[i][j] = val;
    }
    fclose(fifo_pointer);
    remove(fifo_path);
    FILE *data_file = fopen("data", "w");
    for(int i = 0; i < R; i++)
        for (int j = 0; j < R; j++)
            fprintf(data_file, "%d %d %d\n", i, j, tab[i][j]);

    fclose(data_file);
    FILE* gnuplot = popen("gnuplot", "w");
    fprintf(gnuplot,"set view map\n");
    fprintf(gnuplot,"set xrange [0:%d]\n",R);
    fprintf(gnuplot,"set yrange [0:%d]\n",R);
    fprintf(gnuplot,"plot 'data' with image\n"); // taking data from file 'data'
    fflush(gnuplot);
    getc(stdin);
    return 0;
}