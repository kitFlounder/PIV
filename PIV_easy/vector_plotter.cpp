/******************************************************************************
AUTHER: Yohsuke Tanaka
DATE: 2018.08.01
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
const int width = 1024;     //画像幅
const int height = width;    //画像高さ
const int cal_width = 128;  //計算格子幅
const int cal_height = cal_width; //計算格子高さ
const int win_width = 64;   //探査窓・参照窓幅
const int win_height = win_width;  //探査窓・参照窓高さ
const double cal_OW = 0.5;  //計算格子オーバーラップ率
const double inter_OW = 1; //探査窓オーバーラップ率

const char *xxlabel = "{/Times-New-Roman:Italic=20 x} [pixel]";
const char *yylabel = "{/Times-New-Roman:Italic=20 y} [pixel]";
const char *cb_label = "{/Times-New-Roman:Italic=20 U} [m/sec]"; //color bar range min
const double v_r = 100;                                 //magnified ratio for vector length
const int x_min = -1;                                    //x range min
const int x_max = width / (cal_width * cal_OW) - (1 / cal_OW - 1); //x range max
const int y_min = -1;                                    //y range min
const int y_max = height / (cal_height * cal_OW) - (1 / cal_OW - 1); //y range max
const int cb_min = 0;                                   //color bar range min
const float cb_max = 0.01;                               //color bar range max
const char *read_file_dir = "01_plot_2dvec_vector";
//const char *read_file_header = "2dvec_vector";
const char *read_file_header = "green";
const char *write_file_dir = "02_2dvec_vector_map";
//const char *write_file_header = "2dvec_vector_map";
const char *write_file_header = "green";
char read_file[100];
char output_file[100];
void graph_GNU(); //png & eps

FILE *infile1;
FILE *infile2;
FILE *readfile;
FILE *outfile;
FILE *gp;
/*********************************   MAIN   *********************************/
int main()
{
    mode_t mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
    mkdir(write_file_dir, mode);

        sprintf(read_file, "%s//%s.dat", read_file_dir, read_file_header);
        printf("%s//%s.dat\n", read_file_dir, read_file_header);
        readfile = fopen(read_file, "rb");
        fclose(readfile);
        if ((gp = popen("gnuplot -persist", "w")) == NULL)
        {
            printf("gnuplot is not here!\n");
            exit(0);
        }
        //PNG image
        fprintf(gp, "set terminal pngcairo enhanced font 'Times New Roman,15' \n");
        fprintf(gp, "set output '%s//%s.png'\n", write_file_dir, write_file_header);
        fprintf(gp, "set multiplot\n");     // <steps in scan>,<steps between scans>
        fprintf(gp, "unset key\n");         // <steps in scan>,<steps between scans>
        fprintf(gp, "set size ratio -1\n"); // <steps in scan>,<steps between scans>

        fprintf(gp, "set lmargin screen 0.15\n"); // <steps in scan>,<steps between scans>
        fprintf(gp, "set rmargin screen 0.85\n"); // <steps in scan>,<steps between scans>
        fprintf(gp, "set tmargin screen 0.85\n"); // <steps in scan>,<steps between scans>
        fprintf(gp, "set bmargin screen 0.15\n"); // <steps in scan>,<steps between scans>

        fprintf(gp, "set xrange [%d:%d]\n", x_min, x_max);       // <steps in scan>,<steps between scans>
        fprintf(gp, "set xlabel '%s'offset 0.0,0.5\n", xxlabel); // <steps in scan>,<steps between scans>
        fprintf(gp, "set yrange [%d:%d]\n", y_min, y_max);       // <steps in scan>,<steps between scans>
        fprintf(gp, "set ylabel '%s'offset 0.5,0.0\n", yylabel); // <steps in scan>,<steps between scans>

        fprintf(gp, "set cblabel '%s'offset 0.0,0.0\n", cb_label);
        fprintf(gp, "set cbrange['%d':'%f']\n", cb_min, cb_max);
        fprintf(gp, "set colorbox vertical user origin 0.8, 0.2 size 0.025,0.6\n");
        fprintf(gp, "set palette rgbformulae 22,13,-31\n");
        fprintf(gp, "set pm3d map\n"); // <steps in scan>,<steps between scans>
        //fprintf(gp, "splot '%s//%s.dat'  using 2:1:4:3:(sqrt($4*$4+$3*$3)) with pm3d, '%s//%s.dat' using ($2):($1):($1*0.0):(%lf*$4/sqrt($4*$4+$3*$3)):(%lf*$3/sqrt($4*$4+$3*$3)):($1*0.0) with vectors head filled lt 1 lc 'black' \n", read_file_dir, read_file_header, read_file_dir, read_file_header, v_r, v_r);
        fprintf(gp, "splot '%s//%s.dat'  using 2:1:(sqrt($4*$4+$3*$3)) with pm3d, '%s//%s.dat' using ($2):($1):($1*0.0):(%lf*$4):(%lf*$3):($1*0.0) with vectors head filled lt 2 lc 'black' \n", read_file_dir, read_file_header, read_file_dir, read_file_header, v_r, v_r);

        fflush(gp);            //Clean up Data
        fprintf(gp, "exit\n"); // Quit gnuplot

        pclose(gp);
    
    return (0);
}