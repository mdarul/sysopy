#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int count_iters(double re, double im, int R)
{
    int iters = 0;
    double z_re = 0, z_im = 0, z_re_tmp;
    while ((z_re * z_re + z_im * z_im < 4) && iters < R) {
        z_re_tmp = z_re * z_re - z_im * z_im + re;
        z_im = 2 * z_re * z_im + im;
        z_re = z_re_tmp;
        iters++;
    }
    return iters;
}

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        printf("usage: program <pipe_file> <points_amount> <scale_for_inters>\n");
        exit(EXIT_FAILURE);
    }
    char *pipe_path = argv[1];
    int points_amount = atoi(argv[2]);
    int scale = atoi(argv[3]);
    srand(getpid());
    FILE *fifo = fopen(pipe_path, "a");
    double re_temp, z_re, z_im, re_0, im_0;
    for(int i = 0; i < points_amount ; i++)
    {
        re_0 = -2.0 + ((double)rand()/RAND_MAX) * 3.0;
        im_0 = -1.0 + ((double)rand()/RAND_MAX) * 2.0;
        z_re = 0.0;
        z_im = 0.0;
        fprintf(fifo, "%lf %lf %d\n", re_0, im_0, count_iters(re_0, im_0, scale));
    }
    return 0;
}