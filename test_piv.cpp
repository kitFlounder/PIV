/******************************************************************************
PROGRAM NAME:main.cpp
AUTHER: Hiroki TANAKA
DATE: 2020/5/16
2画像2次元PIVプログラム(相互相関法)
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double FPS = 30;      //1秒ごとの画像枚数(フレームレート)
const double MPP = 0.033;   //1ピクセルごとの距離(m)

const int width = 1024;     //画像幅
const int height = 1024;    //画像高さ
const int cal_width = 256;  //計算格子幅
const int cal_height = 256; //計算格子高さ
const int win_width = 32;   //探査窓・参照窓幅
const int win_height = 32;  //探査窓・参照窓高さ

const double cal_OW = 0.5;         //計算格子オーバーラップ率
const double inter_OW = 0.5;       //探査窓オーバーラップ率

unsigned char FOR[height][width];                                   //前方画像格納部
unsigned char NEXT[height][width];                                  //後方画像格納部
unsigned char cal[cal_height][cal_width];                           //計算格子格納部
unsigned char ref[win_height][win_width];                           //参照窓格納部
unsigned char inter[win_height][win_width];                         //探査窓格納部
double corr[cal_height / win_height][cal_width / win_width]; //探査窓毎の相関係数格納部

double u[height / cal_height][width / cal_width];       //計算格子毎のx方向速度
double v[height / cal_height][width / cal_width];       //計算格子毎のy方向速度

const char *input_image1 = "01.bmp";         //入力する前方画像ファイル名
const char *input_image2 = "02.bmp";         //入力する後方画像ファイル名
const char *output_image = "vector.bmp";    //出力する速度場画像ファイル名

unsigned char header_buf[1078];
unsigned char image_in1[height][width];
unsigned char image_in2[height][width];
unsigned char image_out[height][width];

FILE *infile1;
FILE *infile2;
FILE *outfile;

int main()
{
    //1枚目の画像読み込み
    infile1 = fopen(input_image1, "rb");
    if (infile1 == NULL)
    {
        printf("No such an input_image! \n ");
        return (0);
    }                                                           //Notice file missing
    fread(header_buf, sizeof(unsigned char), 1078, infile1);    // Read Header
    fread(image_in1, sizeof(image_in1), 1, infile1);            // Read 8 bit image intensity
    fclose(infile1);

    //2枚目の画像読み込み
    infile2 = fopen(input_image2, "rb");
    if (infile2 == NULL)
    {
        printf("No such an input_image! \n ");
        return (0);
    }                                                           //Notice file missing
    fread(header_buf, sizeof(unsigned char), 1078, infile2);    // Read Header
    fread(image_in2, sizeof(image_in1), 1, infile2);            // Read 8 bit image intensity
    fclose(infile2);

    int i, j;//画像・計算格子・窓の読込に使用
    int p, q;//計算格子の走査に使用
    int k, l;//探査窓の走査に使用

    //前方画像格納
    for ( i = 0; i < height; i++)
    {
        for ( j = 0; j < width; j++)
        {
            FOR[i][j] = image_in1[i][j];
        }
    }
    //後方画像格納
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            NEXT[i][j] = image_in2[i][j];
        }
    }

    //計算格子  
    int cal_y, cal_x;       //計算格子の開始点
    //計算格子の走査
    for (p = 0; p < (height / (cal_height * cal_OW)) - 1; p++)
    {
        for (q = 0; q < (width / (cal_width * cal_OW)) - 1; q++)
        {            
            //計算格子の開始点設定
            cal_y = p * cal_height * cal_OW;
            cal_x = q * cal_width * cal_OW;
            //計算格子の格納
            for (i = 0; i < cal_height; i++)
            {
                for (j = 0; j < cal_width; j++)
                {
                    cal[i][j] = NEXT[cal_y + i][cal_x + j];
                }
            }

            //参照窓
            int ref_sum;        //参照窓内の輝度の総和
            double ref_ave ;    //参照窓内の輝度の平均
            int ref_y, ref_x;   //参照窓の開始点
            //参照窓の開始点設定
            ref_y = cal_y + (cal_height - win_height) / 2;
            ref_x = cal_x + (cal_width - win_width) / 2;
            //参照窓の格納
            for (i = 0; i < win_height; i++)
            {
                for ( j = 0; j < win_width; j++)
                {
                    ref[i][j] = FOR[ref_y + i][ref_x + j];
                    ref_sum = ref_sum + ref[i][j];
                }
            }
            ref_ave = ref_sum / (win_height * win_width);
            
            //探査窓
            int inter_sum;          //探査窓内の輝度の総和
            double inter_ave;       //探査窓内の輝度の平均
            int inter_y, inter_x;   //探査窓の開始点
            //探査窓の走査
            for (k = 0; k < (cal_height / (win_height * inter_OW)) - 1; k++)
            {
                for ( l = 0; l < (cal_width / (win_width * inter_OW)) - 1; l++)
                {
                    //探査窓開始点の設定
                    inter_y = cal_y + k * win_height * inter_OW;
                    inter_x = cal_x + l * win_height * inter_OW;
                    //探査窓の格納
                    for (i = 0; i < win_height; i++)
                    {
                        for (j = 0; j < win_width; j++)
                        {
                            inter[i][j] = cal[inter_y + i][inter_x + j];
                            inter_sum = inter_sum + inter[i][j];
                        } 
                    }
                    inter_ave = inter_sum / (win_height * win_width);

                    //探査窓毎の相互相関係数の算出
                    double R = 0;
                    double R_ref = 0;   //参照窓の平均差二乗の総和
                    double R_inter = 0; //探査窓の平均差二乗の総和
                    for (i = 0; i < win_height; i++)
                    {
                        for ( j = 0; j < win_width ; j++)
                        {
                            R       = R       + (ref[i][j] - ref_ave)     * (inter[i][j] - inter_ave);
                            R_ref   = R_ref   + (ref[i][j] - ref_ave)     * (ref[i][j] - ref_ave);
                            R_inter = R_inter + (inter[i][j] - inter_ave) * (inter[i][j] - inter_ave);
                        }
                    }
                    R_ref = sqrt(R_ref);
                    R_inter = sqrt(R_inter);
                    corr[k][l] = R / ((R_ref) * (R_inter));
                    //デバッグ用　探査窓毎の相関係数表示
                    printf("CAL(%d , %d),INTER(%d , %d),(x,y) = (%d , %d),corr=%lf,R=%lf,R_ref=%lf,R_inter=%lf \n ", p, q, k,l,inter_x,inter_y , corr[k][l], R, R_ref, R_inter);
                    //次の探査窓へ
                }
            }
            
            //計算格子毎の相互相関平面の算出
            int corr_x = 0, corr_y = 0; //相関係数最大の探査窓の開始点算出に用いる(相関係数の最大値を持つ探査窓の開始点)
            double max = 0;     //相関係数最大値
            for (i = 0; i < cal_height / ((win_height * inter_OW)) - 1; i++)
            {
                for ( j = 0; j < cal_width / ((win_width * inter_OW)) - 1; j++)
                {
                    if (corr[i][j] > max)
                    {
                        corr_y = i ;
                        corr_x = j ;
                        max = corr[i][j];
                    }
                    else
                    {
                        max = max;
                    }
                }
            }

            //計算格子毎の速度ベクトル(ピクセル)の算出
            u[p][q] = (ref_y - (cal_y + corr_y * win_height) * FPS) * MPP;
            v[p][q] = (ref_x - (cal_x + corr_x * win_width) * FPS) * MPP;

            //デバッグ用
            printf("\n CAL(%d , %d)(x,y) = (%d ,%d) ,REF(x,y) = (%d ,%d)\n ", p, q, cal_x, cal_y,ref_x , ref_y);
            printf("CAL(%d , %d)MAX(x,y) =(%d,%d),MAX = %lf \n", p, q, corr_x, corr_y, max);
            printf("CAL(%d , %d)u(x,y) =(%lf,%lf) \n\n", p,q,u[p][q],v[p][q]);
            //次の計算格子へ
        }
    }
    //2画像ごとの速度ベクトル描画
    
    //誤ベクトル除去

    return 0;
}
