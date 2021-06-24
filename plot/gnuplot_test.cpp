#include <stdio.h>
#include <stdlib.h>
int main()
{
    FILE *gp; // For gnuplot

    // gnuplotの起動コマンド
    if ((gp = popen("gnuplot", "w")) == NULL)
    {
        printf("gnuplot can't boot! \n");
        exit(EXIT_FAILURE);
    }

    // --- gnuplotにコマンドを送る --- //
    fprintf(gp, "set xrange [-10:10]\n"); // 範囲の指定(省略可)
    fprintf(gp, "set yrange [-1:1]\n");

    fprintf(gp, "plot sin(x)\n"); //sin(x)を描く
    fprintf(gp, "set terminal pngcairo\n");
    fprintf(gp, "set out ' sin.png ' \n");
    fprintf(gp, "replot \n");

    fflush(gp); // バッファに格納されているデータを吐き出す（必須）
    fprintf(gp, "exit\n"); // gnuplotの終了
    pclose(gp);
}