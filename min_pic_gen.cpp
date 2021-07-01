/******************************************************************************
PROGRAM NAME:main.cpp
AUTHER: Hiroki TANAKA
DATE: 2020/5/16
背景除去のための最小画像作成プログラム(相互相関法)
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int n = 100;       //読込画像枚数

const int width = 1024;     //画像幅
const int height = 1024;    //画像高さ

const double cal_OW = 0.5; //計算格子オーバーラップ率
const int inter_OW = 1;    //探査窓移動幅(<inter_height,inter_width)

unsigned char FOR[height][width];           //前方画像格納部
unsigned char NEXT[height][width];          //後方画像格納部

const char *input_image_dir = "ParticleMap1.bmp";    //入力する前方画像ファイル名
const char *input_image_header = "ParticleMap2"; //入力する後方画像ファイル名
const char *output_image_dir = "back_ground";         //出力する速度場画像ファイル名
const char *output_image_header = "min_pic";      //出力する速度場画像ファイル名

unsigned char header_buf[1078];
unsigned char image_in1[height][width]; //前方画像格納部
unsigned char image_in2[height][width]; //後方画像格納部
unsigned char image_out[height][width]; //出力画像格納部

char FOR_file[100];
char NEXT_file[100];
char OUT_file[100];

FILE *infile1;
FILE *infile2;
FILE *readfile;
FILE *outfile;
FILE *gp;

int main()
{
    int im;   //連続画像読込に使用
    int i, j; //画像・計算格子・窓の読込に使用
    int p, q; //計算格子の走査に使用
    int k, l; //探査窓の走査に使用
    //初期最小画像読み込み
    sprintf(FOR_file, "%s//%s%04d.bmp", input_image_dir, input_image_header, 0);
    infile1 = fopen(FOR_file, "rb");
    if (infile1 == NULL)
    {
        printf("No such an input_image! \n ");
        return (0);
    }                                                        //Notice file missing
    fread(header_buf, sizeof(unsigned char), 1078, infile1); // Read Header
    fread(image_in1, sizeof(image_in1), 1, infile1);         // Read 8 bit image intensity
    fclose(infile1);
    //初期最小画像格納
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            FOR[i][j] = 0;               //初期化
            FOR[i][j] = image_in1[i][j]; //格納
        }
    }

    for (im = 1; im < n - 1; im++)
    {
        //比較画像読み込み
        sprintf(NEXT_file, "%s//%s%04d.bmp", input_image_dir, input_image_header, im + 1);
        infile2 = fopen(NEXT_file, "rb");
        if (infile2 == NULL)
        {
            printf("No such an input_image! \n ");
            return (0);
        }                                                        //Notice file missing
        fread(header_buf, sizeof(unsigned char), 1078, infile2); // Read Header
        fread(image_in2, sizeof(image_in2), 1, infile2);         // Read 8 bit image intensity
        fclose(infile2);

        //比較画像格納
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                NEXT[i][j] = 0;               //初期化
                NEXT[i][j] = image_in2[i][j]; //格納
            }
        }
        //最小画像作成
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                if (FOR[i][j]>NEXT[i][j])
                {
                    FOR[i][j] = NEXT[i][j];
                }
            }
        }
    }
    //画像組ごとの速度ベクトルのdat出力
    sprintf(OUT_file, "%s//%s%04d.dat", output_image_dir, output_image_header, im);
    printf("%s//%s%04d.dat\n", output_image_dir, output_image_header, im);
    outfile = fopen(OUT_file, "w");
    for (i = 0; i < (height); i++)
    {
        for (j = 0; j < (width); j++)
        {
            fprintf(outfile, "%d \n",  FOR[i][j]);
        }
        fprintf(outfile, "\n");
    }
    fclose(outfile);
    //次の画像組へ
    return 0;
}