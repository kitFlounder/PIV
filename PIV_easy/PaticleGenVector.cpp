/***************************
program name:ParticleGenerater
auther:Hiroki Tanaka
date:2021.04.19
***************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
const int width = 1024;
const int height = 1024;
const int t = 10;
const char *input_image = "base.bmp";           // name of input file
const char *output_image1 = "ParticleMap1.bmp"; // name of output file
const char *output_image2 = "ParticleMap2.bmp"; // name of output file
const int quantity = 500;
const int a_max = 255;
const int a_min = 128;
const int setting_ave = 70;
const int setting_var = 20;
const int mpp = 2; //micrometer per pixel
unsigned char header_buf[1078];
unsigned char image_out1[height][width];
unsigned char image_out2[height][width];
FILE *infile;
FILE *outfile1;
FILE *outfile2;
int brightness(int n, int m);
int main()
{
    infile = fopen(input_image, "rb");
    if (infile == NULL)
    {
        printf("No such an input_image !\n");
        return (0);
    }
    fread(header_buf, sizeof(unsigned char), 1078, infile); // Read Header
    fclose(infile);
    srand(time(NULL));
    for (int k = 0; k < quantity; k++)
    {
        double a;
        int x,x2;
        int y,y2;
        double u, v;
        int i = 0;
        int j = 0;
        do
        {
            x = width * (double)rand() / RAND_MAX;
            y = height * (double)rand() / RAND_MAX;
            u = t * sin(M_PI * x / width) * cos(M_PI * y / height);
            v = - t * cos(M_PI * x / width) * sin(M_PI * y / height);
            x2 = x + (int)u;
            y2 = y + (int)v;
            a = (a_max - a_min + 1) * ((double)rand() / RAND_MAX) + a_min; //brightness
        } 
        while (x,x2 == width || y,y2 == height || a == a_max + 1);
        int r = (setting_var * sqrt(-2 * log((double)rand() / RAND_MAX)) * cos(2 * M_PI * (double)rand() / RAND_MAX) + setting_ave) / 2; //radius
        int n = a * exp((double)(-(i * i + j * j) * 8 * mpp * mpp) / r / r);
        do
        {
            do
            {
                if (x + i < width)
                {
                    if (y + j < height)
                    {
                        image_out1[y + j][x + i] = brightness(n, image_out1[y + j][x + i]);
                    }
                    if (y - j >= 0)
                    {
                        image_out1[y - j][x + i] = brightness(n, image_out1[y - j][x + i]);
                    }
                }
                if (x - i >= 0)
                {
                    if (y + j < height)
                    {
                        image_out1[y + j][x - i] = brightness(n, image_out1[y + j][x - i]);
                    }
                    if (y - j >= 0)
                    {
                        image_out1[y - j][x - i] = brightness(n, image_out1[y - j][x - i]);
                    }
                }

                if (x2 + i < width)
                {
                    if (y2+ j < height)
                    {
                        image_out2[y2 + j][x2 + i] = brightness(n, image_out2[y2 + j][x2 + i]);
                    }
                    if (y2 - j >= 0)
                    {
                        image_out2[y2 - j][x2 + i] = brightness(n, image_out2[y2 - j][x2 + i]);
                    }
                }
                if (x2 - i >= 0)
                {
                    if (y2 + j < height)
                    {
                        image_out2[y2 + j][x2 - i] = brightness(n, image_out2[y2 + j][x2 - i]);
                    }
                    if (y2 - j >= 0)
                    {
                        image_out2[y2 - j][x2 - i] = brightness(n, image_out2[y2 - j][x2 - i]);
                    }
                }
                i++;
                n = a * exp((double)(-(i * i + j * j) * 8 * mpp * mpp) / r / r);
            } while (n != 0);
            i = 0;
            j++;
            n = a * exp((double)(-(i * i + j * j) * 8 * mpp * mpp) / r / r);
        } while (n != 0);
    }

    outfile1 = fopen(output_image1, "wb");
    fwrite(header_buf, sizeof(unsigned char), 1078, outfile1); // Write Header
    fwrite(image_out1, sizeof(image_out1), 1, outfile1);         // Write 8 bit image intensity
    fclose(outfile1);
    outfile2 = fopen(output_image2, "wb");
    fwrite(header_buf, sizeof(unsigned char), 1078, outfile2); // Write Header
    fwrite(image_out2, sizeof(image_out2), 1, outfile2);         // Write 8 bit image intensity
    fclose(outfile2);

    return (0);
}
int brightness(int n, int m)
{
    if (n > m)
    {
        return n;
    }
    else
    {
        return m;
    }
}