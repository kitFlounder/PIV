/******************************************************************************
PROGRAM NAME:main.cpp
AUTHER: Hiroki TANAKA
DATE: 2020/5/16
2画像2次元PIVプログラム(相互相関法)
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
double a;
int x, x2;
int y, y2;
int p = 0;
int q = 0;
const int width = 1024;  //画像幅
const int height = width; //画像高さ
const int cal_width = 128;  //計算格子幅
const int cal_height = cal_width; //計算格子高さ
const int cal_yq = 15; //計算格子の個数
const int cal_xq = cal_yq;   //計算格子の個数
double u[cal_yq][cal_xq]; //計算格子毎のx方向速度
double v[cal_yq][cal_xq]; //計算格子毎のy方向速度
double U[cal_yq][cal_xq];
const int t = 10;
const double FPS = 0.1;  //1秒ごとの画像枚数(フレームレート)
const double MPP = 0.01; //1ピクセルごとの距離(m)

const char *read_file_dir = "01_plot_2dvec_vector";
const char *read_file_header = "green";
char read_file[100];
FILE *readfile;
FILE *outfile;

int main()
{
    for ( p = 0; p <cal_yq ; p++)
    {
        for (q = 0; q <cal_xq; q++)
        {
            x = (q)*cal_width;
            y = (p)*cal_height;
            //u[p][q] = t * sin(M_PI / 2 + M_PI * q / cal_xq) * cos(M_PI / 2 + M_PI * p / cal_yq) * MPP * 0.1;
            //v[p][q] = -t * cos(M_PI / 2 + M_PI * q / cal_xq) * sin(M_PI / 2 + M_PI * p / cal_yq) * MPP * 0.1;
            u[p][q] = t * sin(2 * M_PI *(q+0.5) / cal_xq) * cos(2 * M_PI * (p+0.5) / cal_yq) * MPP * 0.1;
            v[p][q] = -t * cos(2 * M_PI * (q+0.5) / cal_xq) * sin(2 * M_PI * (p+0.5) / cal_yq) * MPP * 0.1;
            U[p][q] = sqrt(u[p][q] * u[p][q] + v[p][q] * v[p][q]);
        }
    }
    sprintf(read_file, "%s//%s.dat", read_file_dir, read_file_header);
    printf("%s//%s.dat\n", read_file_dir, read_file_header);

    //2画像ごとの速度ベクトルのdat出力
    outfile = fopen(read_file, "w");
    for (int p = 0; p < cal_yq; p++)
    {
        for (q = 0; q < cal_xq; q++)
        {
            fprintf(outfile, "%d \t %d \t %.5lf \t %.5lf \t %.5lf \t  \n", p, q, u[p][q], v[p][q] ,U[p][q]);
        }
        fprintf(outfile, "\n");
    }
    fclose(outfile);
    return 0;
}

    