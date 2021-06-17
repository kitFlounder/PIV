/******************************************************************************
PROGRAM NAME:main.cpp
AUTHER: Hiroki TANAKA
DATE: 2020/5/16
2次元PIVプログラム(相互相関法)
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
const int n = 8000; //画像枚数
const int width = 1024;
const int heigt = 1024; //画像の幅wと高さh ピクセル
const int Wwidth = 32;
const int Wheigt = 32; //参照窓・探査窓の幅と高さ
const int Cwidth = 128;
const int Cheigt = 128;//計算格子の幅と高さ(探査窓がいくつ入るか？)
unsigned char MIN_pic[width * heigt];//最小画像の輝度格納[w][h]
unsigned char NEXT_pic[width * heigt];//比較画像の輝度格納[w][h]
const char *input_image = " n . bmp ";     // name of input file
const char *output_image = "vector . bmp"; //name of output file
unsigned char header_buf[1078];
int FOR_pic[width * heigt];//前方画像の輝度格納
int BACK_pic[width * heigt];//後方画像の輝度格納
int REF_win[Wwidth*Wheigt];//参照窓の輝度格納
int INT_win[Wwidth*Wheigt];//探査窓の輝度格納
int CROSS[(2 * width / Cwidth -1)*(2 * heigt / Cheigt -1)]; //50%overlapping時の相関係数の格納
const int FPS = 125;                       // FlamePerLate
const int MPP = 0.033;                     // MaterPerPixel
FILE *infile_for;
FILE *infile_back;
FILE *outfile;
//前処理
//最小画像作成 比較画像を繰り返し読み込んで各ピクセルごとに比較
int MIN
{
    FOR_pic = 0;                              //前方画像の初期化
    infile_for = fopen(input_image[1], "rb"); // 入力画像読み込み
    if (infile_for == NULL)                   // 成否判定
    {
        printf("No such an input_image !\n");
        return (0);
    }
    fread(header_buf, sizeof(unsigned char), 1078, ); // ヘッダー読み込み
    fread(FOR_pic, sizeof(FOR_pic), 1, infile_for);
    fclose(infile_for);
    int num, i, j;
    for (size_t num = 1; i < n; i++)
    {
        NEXT_pic[][] = {};                           //比較画像の読み込み格納
        infile_back = fopen(input_image[num], "rb"); // 入力画像読み込み
        if (infile_back == NULL)                     // 成否判定
        {
            printf("No such an input_image !\n");
            return (0);
        }
        fread(header_buf, sizeof(unsigned char), 1078, );  // ヘッダー読み込み
        fread(NEXT_pic, sizeof(NEXT_pic), 1, infile[num]); // Read 8 bit image intensity
        fclose(infile_back);
        for (size_t i = 0; i < heigt; i++) // 行方向の走査
        {
            for (size_t j = 0; j < width; i++) // 各行内の列方向の走査
            {
                if (MIN_pic[i][j] > NEXT_pic[i][j])
                {
                    MIN_pic[i][j] = NEXT_pic[i][j]; //比較して小さい部分を置き換え
                }
            }
        }
    }
}
// 二次元PIV
// 相互相関関数算出
int CrossCorr()
{
    REF_win[];
    INT_win[];
    int i, j, k;
    int REF_sum, REF_ave;
    int INT_sum, INT_ave;
    //参照窓と探査窓の平均光強度算出
    for ( i = 0; i < width*heigt; i++)
    {
        REF_sum = REF_sum + REF_win[i];
        INT_sum = INT_sum + INT_win[i];
    }
    REF_ave = REF_sum / (width * heigt);
    INT_ave = INTEROGATE_sum / (width * heigt);

    //相互相関係数の計算
    for ( i = 0; i < count; i++)
    {
        R = R + (REF_win[i]-REF_ave)*(INT_win[i]-INT_ave);
        R_ref = R_ref + (REF_win[i]-REF_ave)*(REF_win[i]-REF_ave);
        R_int = R_int + (REF_win[i] - REF_ave) * (REF_win[i] - REF_ave);
    }
    R_fg = R / (R_ref * R_int);
}

// 配列内最大値算出・戻り値は座標・相関係数算出に使用
int Max()
{
}
// 相関係数から速度場ベクトル算出 速度の絶対値と角度があればいける？
int Vector()
{
    FPS;
    MPP;
    CROSS[][];
}
// 与えられたベクトルから分布図生成と出力
int PlotVector()
{
    //GNUplotを用いて計算格子の数だけベクトル配置
    outfile = fopen(output_image, "wb");
    fwrite(header_buf, sizeof(unsigned char), 1078, outfile); // ヘッダー書き出し
    fwrite(image_out, sizeof(image_out), 1, outfile);         // 画像書き出し
    fclose(outfile);
}
int main()
{
    int num, i, j, k, l, m;
    //1枚目の画像(前方画像)読み込み
    FOR_pic[width][heigt] = {}; //前方画像の初期化
    infile[1] = fopen(, "rb");  // 入力画像読み込み
    if (infile[1] == NULL)      // 成否判定
    {
        printf("No such an input_image !\n");
        return (0);
    }
    fread(header_buf, sizeof(unsigned char), 1078, ); // ヘッダー読み込み
    fread(FOR_pic, sizeof(FOR_pic), 1, infile[1]);
    fclose(infile[1]);

    for (num = 1; num < n; i++) //for全画像2 < n
    {
        //参照窓・計算格子の開始点割り振り //計算格子・参照窓の設定(開始点と窓サイズ)
        //時間的に隣接する2画像(i, i + 1) の読み込み、格納 i = 1を除いて後方画像のみ読み込む
        BACK_pic = {};               //後方画像の初期化
        infile[num] = fopen(, "rb"); // 入力画像読み込み
        if (infile[num] == NULL)     // 成否判定
        {
            printf("No such an input_image !\n");
            return (0);
        }
        fread(header_buf, sizeof(unsigned char), 1078, ); // ヘッダー読み込み
        fread(BACK_pic, sizeof(BACK_pic), 1, infile[num]);
        fclose(infile[num]);
        for (i = 0; i < ; i++) //for全参照窓(各計算格子内) iは計算格子・参照窓
        {
            ​
                //前方画像の当該部分を参照窓として読み込み、格納
                REF_win = {};
            for (k = 0; k < count; k++)
            {
                REF_win[k][l] = FOR_pic[k][l];
            }
            for (j = 0; i < count; i++) //for各計算格子内の探査窓 jは探査窓
            {
                // 計算格子内における探査窓の開始点を代入
                INT_win = {};
                for (k = 0; k < count; k++)
                {
                    INT_win[k][l] = BACK_pic[yi + k][xi + l]; //後方画像における探査窓の読み込み、格納
                }
                CrossCorr; //参照窓と当該探査窓における相関係数の算出・関数呼び出し
                // 相関係数の格納
                C = CROSS[];
                //次の計算格子内の探査窓に繰り返すfor文終わり
            }
            //相関平面の算出 * *(相関係数最大の探査窓を候補窓に)
            // MAX関数の呼び出し 相関平面の配列を与えて相関係数最大の座標を返す
            //窓の変位ベクトルとフレームレートから速度ベクトルの算出
            //VECTOR関数の呼び出し 座標を与えて参照窓・計算格子毎の速度ベクトルを返す
            //各時刻間での速度ベクトル画像の出力呼び出し
            // PlotVector関数の呼び出し 速度ベクトルを与えてベクトル画像を返す
            //(誤ベクトルの検知・除去)
            REF_win = {}; //参照窓格納部初期化
            //次の参照窓・計算格子に繰り返すfor文終わり
        }
        FOR_pic = 0;       //前方画像の初期化
        FOR_pic = BACK_pic; //後方画像の前方画像への置換(i = i + 1)
        //次の時間的隣接2画像(i + 1, i + 2) に繰り返すfor文終わり
    }
    return (0);
}
//後処理
//git確認