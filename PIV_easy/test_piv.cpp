/******************************************************************************
PROGRAM NAME:main.cpp
AUTHER: Hiroki TANAKA
DATE: 2020/5/16
2画像2次元PIVプログラム(相互相関法)
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double FPS = 0.1;      //1秒ごとの画像枚数(フレームレート)
const double MPP = 0.01;   //1ピクセルごとの距離(m)

const int width = 1024;         //画像幅
const int height = 1024;        //画像高さ
const int cal_width = 128;      //計算格子幅
const int cal_height = 128;     //計算格子高さ
const int win_width = 64;       //探査窓・参照窓幅
const int win_height = 64;      //探査窓・参照窓高さ

const double cal_OW = 0.5;      //計算格子オーバーラップ率
const int inter_OW = 1;      //探査窓移動幅(<inter_height,inter_width)

unsigned char FOR[height][width];             //前方画像格納部
unsigned char NEXT[height][width];            //後方画像格納部
unsigned char cal[cal_height][cal_width];     //計算格子格納部
unsigned char ref[win_height][win_width];     //参照窓格納部
unsigned char inter[win_height][win_width];   //探査窓格納部

const int win_yq = cal_height - win_height;   //計算格子毎の探査窓の個数
const int win_xq = cal_width - win_width;     //計算格子毎の探査窓の個数
double corr[win_yq][win_xq];                  //探査窓毎の相関係数格納部

const int cal_yq = height / (cal_height * cal_OW) - (1 / cal_OW - 1);    //計算格子の個数
const int cal_xq = width / (cal_width * cal_OW) - (1 / cal_OW - 1);      //計算格子の個数
int corr_y[cal_yq][cal_xq];                                              //相関係数最大の探査窓の開始点算出に用いる(相関係数の最大値を持つ探査窓の開始点)
int corr_x[cal_yq][cal_xq];
double max[cal_yq][cal_xq];                                              //相関係数最大値
double u[cal_yq][cal_xq];                                                //計算格子毎のx方向速度
double v[cal_yq][cal_xq];                                                //計算格子毎のy方向速度
double U[cal_yq][cal_xq];                                                //計算格子毎の速度絶対値

const char *input_image1 = "ParticleMap1.bmp";    //入力する前方画像ファイル名
const char *input_image2 = "ParticleMap2.bmp";    //入力する後方画像ファイル名
const char *output_image = "vector.bmp";          //出力する速度場画像ファイル名

unsigned char header_buf[1078];
unsigned char image_in1[height][width];     //前方画像格納部
unsigned char image_in2[height][width];     //後方画像格納部
unsigned char image_out[height][width];     //出力画像格納部

//Graph parameters for GNU
const char *xxlabel = "{/Times-New-Roman:Italic=20 x} [pixel]";
const char *yylabel = "{/Times-New-Roman:Italic=20 y} [pixel]";
const char *cb_label = "{/Times-New-Roman:Italic=20 U} [m/sec]";                     //color bar range min
const double v_r = 1.0;                                                              //magnified ratio for vector length
const int x_min = 0;                                                                 //x range min
const int x_max = width/(cal_width*cal_OW);                                          //x range max
const int y_min = 0;                                                                 //y range min
const int y_max = 2*height/(cal_height*cal_OW);                                      //y range max
const int cb_min = 0;                                                                //color bar range min
const float cb_max = 1.5;                                                            //color bar range max
const char *read_file_dir = "01_plot_2dvec_vector";
const char *read_file_header = "2dvec_vector";
const char *write_file_dir = "02_2dvec_vector_map";
const char *write_file_header = "2dvec_vector_map";
char read_file[100];
char output_file[100];
void graph_GNU(); //png & eps

FILE *infile1;
FILE *infile2;
FILE *readfile;
FILE *outfile;
FILE *gp;

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
            // FOR[i][j] = 0;                  //初期化
            FOR[i][j] = image_in1[i][j];    //格納
        }
    }
    //後方画像格納
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            // NEXT[i][j] = 0;                 //初期化
            NEXT[i][j] = image_in2[i][j];   //格納
        }
    }

    //計算格子
    int cal_y = 0;
    int cal_x = 0; //計算格子の開始点
    //計算格子の走査
    for (p = 0; p < cal_yq; p++)
    {
        for (q = 0; q < cal_xq; q++)
        {            
            //計算格子の開始点設定
            cal_y = p * cal_height * cal_OW;
            cal_x = q * cal_width * cal_OW;
            //計算格子の格納
            for (i = 0; i < cal_height; i++)
            {
                for (j = 0; j < cal_width; j++)
                {
                    cal[i][j] = 0;                              //初期化
                    cal[i][j] = NEXT[cal_y + i][cal_x + j];     //格納
                }
            }

            //参照窓
            int ref_sum = 0;            //参照窓内の輝度の総和
            double ref_ave = 0;         //参照窓内の輝度の平均
            int ref_y = 0;
            int ref_x = 0;              //参照窓の開始点
            //参照窓の開始点設定
            ref_y = cal_y + (cal_height - win_height) / 2;
            ref_x = cal_x + (cal_width - win_width) / 2;
            //参照窓の格納
            for (i = 0; i < win_height; i++)
            {
                for ( j = 0; j < win_width; j++)
                {
                    ref[i][j] = 0;                              //初期化
                    ref[i][j] = FOR[ref_y + i][ref_x + j];      //格納
                    ref_sum = ref_sum + ref[i][j];              //総和計算
                }
            }
            ref_ave = ref_sum / (win_height * win_width);       //平均値計算

            double R_ref = 0;                                   //参照窓の平均差二乗の総和
            for (i = 0; i < win_height; i++)
            {
                for (j = 0; j < win_width; j++)
                {
                    R_ref = R_ref + (ref[i][j] - ref_ave) * (ref[i][j] - ref_ave);
                }
            }
            R_ref = sqrt(R_ref);

            //探査窓
            int inter_sum = 0;              //探査窓内の輝度の総和
            double inter_ave = 0;           //探査窓内の輝度の平均
            int inter_y = 0;
            int inter_x = 0;                //探査窓の開始点
            //探査窓の走査
            for (k = 0; k < win_yq; k++)
            {
                for (l = 0; l < win_xq; l++)
                {
                    //探査窓開始点の設定
                    inter_y =  k * inter_OW;
                    inter_x =  l * inter_OW;
                    inter_sum = 0;
                    inter_ave = 0;
                    //探査窓の格納
                    for (i = 0; i < win_height; i++)
                    {
                        for (j = 0; j < win_width; j++)
                        {
                            inter[i][j] = 0;                                //初期化
                            inter[i][j] = cal[inter_y + i][inter_x + j];    //格納
                            inter_sum = inter_sum + inter[i][j];            //総和計算
                        } 
                    }
                    inter_ave = inter_sum / (win_height * win_width);       //平均値計算

                    //探査窓毎の相互相関係数の算出
                    double R = 0;       //参照窓・探査窓の平均差の積の総和
                    double R_inter = 0; //探査窓の平均差二乗の総和
                    // corr[k][l] = 0;     //探査窓毎の相互相関係数の初期化

                    for (i = 0; i < win_height; i++)
                    {
                        for ( j = 0; j < win_width ; j++)
                        {
                            R       = R       + (ref[i][j] - ref_ave)     * (inter[i][j] - inter_ave);
                            R_inter = R_inter + (inter[i][j] - inter_ave) * (inter[i][j] - inter_ave);
                        }
                    }
                    R_inter = sqrt(R_inter);
                    corr[k][l] = R / ((R_ref) * (R_inter)); //相互相関係数計算
                    //デバッグ用　探査窓毎の相関係数表示
                    printf("CAL(%d , %d),INTER(%d , %d),(x,y) = (%d , %d),corr=%lf,R=%lf,R_ref=%lf,R_inter=%lf \n ", p, q, k,l,inter_x,inter_y , corr[k][l], R, R_ref, R_inter);
                    //次の探査窓へ
                    // free(inter);
                    // free(ref);
                }
            }
            
            //計算格子毎の相互相関平面の算出(最大相関係数とその時の探査窓の開始点)
            // corr_x[p][q] = 0, corr_y[p][q] = 0;
            // max[p][q] = 0;
            for (i = 0; i < win_yq; i++)
            {
                for ( j = 0; j < win_xq; j++)
                {
                    if (corr[i][j] > max[p][q])
                    {
                        corr_y[p][q] = i;
                        corr_x[p][q] = j;
                        max[p][q] = corr[i][j];
                    }
                    else
                    {
                        max[p][q] = max[p][q];
                    }
                }
            }

            //計算格子毎の速度ベクトル(ピクセル)の算出(参照窓と相関係数最大の探査窓の開始点の変位を使用)
            u[p][q] = (((cal_y + corr_y[p][q] * inter_OW) - ref_y) * FPS) * MPP;
            v[p][q] = (((cal_x + corr_x[p][q] * inter_OW) - ref_x) * FPS) * MPP;

            U[p][q] = sqrt(u[p][q] * u[p][q] + v[p][q] * v[p][q]);                              //速度の絶対値

            //デバッグ用
            printf("\n CAL(%d , %d)(x,y) = (%d ,%d) ,REF(x,y) = (%d ,%d)\n ", p, q, cal_x, cal_y, ref_x, ref_y);
            printf("CAL(%d , %d)MAX(x,y) =(%d,%d),MAX = %lf \n", p, q, corr_x[p][q], corr_y[p][q], max[p][q]);
            printf("CAL(%d , %d)u(x,y) =(%lf,%lf) \n\n", p,q,u[p][q],v[p][q]);
            //次の計算格子へ
            // free(cal);
            // free(corr);
        }
    }

    for ( i = 0; i < cal_yq; i++)
    {
        for ( j = 0; j < cal_xq; j++)
        {
            printf("CAL(%d , %d):corr(%d,%d) = %lf \n", i, j, corr_x[i][j], corr_y[i][j], max[i][j]);
        }
        
    }
    
    sprintf(read_file, "%s//%s.dat", read_file_dir, read_file_header);
    printf("%s//%s.dat\n", read_file_dir, read_file_header);

    //2画像ごとの速度ベクトルのdat出力
    outfile = fopen(read_file, "w");
    for (i = 0; i < (height / (cal_height * cal_OW))-1; i++)
    {
        for (j = 0; j < (width / (cal_width * cal_OW))-1; j++)
        {
            fprintf(outfile, "%d \t %d \t %.5lf \t %.5lf \t  \n", i, j, u[i][j], v[i][j]);
        }
        fprintf(outfile, "\n");
    }
    fclose(outfile);

    //速度ベクトル描画
    // readfile = fopen(read_file, "w");
    // fclose(readfile);
    // if ((gp = popen("gnuplot -persist", "w")) == NULL)
    // {
    //     printf("gnuplot is not here!\n");
    //     exit(0);
    // }
    // //PNG image
    // fprintf(gp, "set terminal pngcairo enhanced font 'Times New Roman,15' \n");
    // fprintf(gp, "set output '%s//%s.png'\n", write_file_dir, write_file_header);
    // fprintf(gp, "set multiplot\n");     // <steps in scan>,<steps between scans>
    // fprintf(gp, "unset key\n");         // <steps in scan>,<steps between scans>
    // fprintf(gp, "set size ratio -1\n"); // <steps in scan>,<steps between scans>

    // fprintf(gp, "set lmargin screen 0.15\n"); // <steps in scan>,<steps between scans>
    // fprintf(gp, "set rmargin screen 0.85\n"); // <steps in scan>,<steps between scans>
    // fprintf(gp, "set tmargin screen 0.85\n"); // <steps in scan>,<steps between scans>
    // fprintf(gp, "set bmargin screen 0.15\n"); // <steps in scan>,<steps between scans>

    // fprintf(gp, "set xrange [%d:%d]\n", x_min, x_max);       // <steps in scan>,<steps between scans>
    // fprintf(gp, "set xlabel '%s'offset 0.0,0.5\n", xxlabel); // <steps in scan>,<steps between scans>
    // fprintf(gp, "set yrange [%d:%d]\n", y_min, y_max);       // <steps in scan>,<steps between scans>
    // fprintf(gp, "set ylabel '%s'offset 0.5,0.0\n", yylabel); // <steps in scan>,<steps between scans>

    // fprintf(gp, "set cblabel '%s'offset 0.0,0.0\n", cb_label);
    // fprintf(gp, "set cbrange['%d':'%f']\n", cb_min, cb_max);
    // fprintf(gp, "set colorbox vertical user origin 0.8, 0.2 size 0.025,0.6\n");
    // fprintf(gp, "set palette rgbformulae 22,13,-31\n");

    // fprintf(gp, "set pm3d map\n"); // <steps in scan>,<steps between scans>
    // for (i = 0; i < (height / (cal_height * cal_OW)) - 1; i++)
    // {
    //     for (j = 0; j < (width / (cal_width * cal_OW)) - 1; j++)
    //     {
    //         fprintf(gp, "splot   (%d):(%d):(0.0):(%lf):(%lf):(0.0) with vectors head filled lt 2 lc 'black' \n", j, i ,(v_r * (u[i][j] / sqrt(u[i][j] * u[i][j] + v[i][j] * v[i][j]))),(v_r * (v[i][j] / sqrt(u[i][j] * u[i][j] + v[i][j] * v[i][j]))) );
    //     }
    // }
    // fflush(gp);            //Clean up Data
    // fprintf(gp, "exit\n"); // Quit gnuplot

    // pclose(gp);

    //誤ベクトル除去

    return 0;
}
