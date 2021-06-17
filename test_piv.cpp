/******************************************************************************
PROGRAM NAME:main.cpp
AUTHER: Hiroki TANAKA
DATE: 2020/5/16
2次元PIVプログラム(相互相関法)
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int width = 1024;
const int height = 1024;
const int cal_width = 256;
const int cal_height = 256;
const int win_width = 32;
const int win_height = 32;

unsigned char cal[cal_height][cal_width];
unsigned char ref[win_height][win_width];
unsigned char inter[win_height][win_width];

const char *input_image1 = "1.bmp";           // name of input file
const char *input_image2 = "2.bmp";           // name of input file
const char *output_image = "vector.bmp"; // name of output file
unsigned char header_buf[1078];
unsigned char image_in1[height][width];
unsigned char image_in2[height][width];
unsigned char image_out[height][width];
FILE *infile1;
FILE *infile2;
FILE *outfile;

int main()
{
    int i,j;
    int p, q;
    //1枚目の画像読み込み
    infile1 = fopen(input_image1, "rb");
    if (infile1 == NULL)
    {
        printf("No such an input_image! \n ");
        return (0);
    }                                                       //Notice file missing
    fread(header_buf, sizeof(unsigned char), 1078, infile1); // Read Header
    fread(image_in1, sizeof(image_in1), 1, infile1);           // Read 8 bit image intensity
    fclose(infile1);
    //2枚目の画像読み込み
    infile2 = fopen(input_image2, "rb");
    if (infile2 == NULL)
    {
        printf("No such an input_image! \n ");
        return (0);
    }                                                        //Notice file missing
    fread(header_buf, sizeof(unsigned char), 1078, infile2); // Read Header
    fread(image_in2, sizeof(image_in1), 1, infile2);         // Read 8 bit image intensity
    fclose(infile2);

    //計算格子の走査
    for (p = 0; p < height / cal_height; p++)
    {
        for ( q = 0; q < width/cal_width; q++)
        {
            int off_y, off_x;
            off_y = p*cal_height;
            off_x = q*cal_width;
            //計算格子の格納
            for (i = 0; i < cal_height; i++)
            {
                
                for (j = 0; j < cal_width; j++)
                {
                    int cal_x = off_x + j;
                    cal[i][j] = input_image2[off_y + i][off_x + j];
                }
            }
            //参照窓の格納
            int ref_sum;
            int ref_y = off_y+cal_height;
            int ref_x = off_x+cal_width;
            for (i = 0; i < win_height; i++)
            {
                for ( j = 0; j < win_width; j++)
                {
                    ref[i][j] = input_image1[off_y + (cal_height - win_height) / 2 + i][off_x + (cal_width - win_width) / 2+j];
                    ref_sum = ref_sum + ref[i][j];
                }
            }
            int k, l;
            //探査窓の走査
            int inter_sum;
            unsigned char corr [cal_height / win_height][cal_width / win_width];
            for (k = 0; k < cal_height / win_height; k++)
            {
                for ( l = 0; l < cal_width/win_width; l++)
                {
                    int inter_y, inter_x;
                    inter_y = k * win_height;
                    inter_x = l * win_height;
                    //探査窓の格納
                    for (i = 0; i < win_height; i++)
                    {
                        for (j = 0; j < win_width; j++)
                        {
                            inter[i][j] = cal[off_y + inter_y + i][off_x + inter_x + j];
                            inter_sum = inter_sum + inter[i][j];
                        } 


                    }
                    //相互相関係数の算出
                        inter;
                        ref;
                        corr[k][l];
                        //次の探査窓へ
                }
            }
            
            //相互相関平面の算出
            int corr_x, corr_y;
            double max;
            for (i = 0; i < cal_height / win_height; i++)
            {
                for ( j = 0; j < cal_width/win_width; j++)
                {
                    if (corr[i][j]>max)
                    {
                        i = corr_y;
                        j = corr_x;
                        max = corr[i][j];
                    }
                }
            }
            
            //次の計算格子へ
        }
    }
    


    return 0;
}
