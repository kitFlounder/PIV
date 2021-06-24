/******************************************************************************
AUTHER: Yohsuke Tanaka
DATE: 2018.07.27
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

const char *xxlabel = "x [pixel]"; ///color bar range min
const char *yylabel = "y [pixel]"; ///color bar range min

const int x_min = 0;    ///color bar range min
const int x_max = 1280; ///color bar range min
const int y_min = 0;    ///color bar range min
const int y_max = 1024; ///color bar range min

const double v_r = 6.0; ///magnified ratio for vector length

const char *cb_label = "|U| [pixel]"; ///color bar range min
const int cb_min = 0;                 ///color bar range min
const int cb_max = 8;                 ///color bar range min

const char *read_file_object = "cylinder.dat";
const char *read_file_dir = "02_vec";
const char *read_file_header = "av-vec_v";
const char *write_file_dir = "03_splot_vec_contour_map";
const char *write_file_header = "splot_vec_contour_map";

//Graph parameters for GNU
char read_file[100];
void graph_GNU(); //png & eps
FILE *gp;         //gnuplot
FILE *infile;
/*********************************   MAIN   *********************************/
main()
{
    mode_t mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
    mkdir(write_file_dir, mode);

    int i = 0;
    int UP = 0;
    while (UP == 0)
    {
        i++;
        sprintf(read_file, "%s//%s%04d.dat", read_file_dir, read_file_header, i);
        printf("%s//%s%04d.dat\n", read_file_dir, read_file_header, i);
        infile = fopen(read_file, "rb");
        fclose(infile);
        if (infile == NULL)
        {
            printf("break!");
            break;
        }
        if ((gp = popen("gnuplot", "w")) == NULL)
        {
            printf("gnuplot is not here!\n");
            exit(0);
        }
        //PNG image
        fprintf(gp, "set terminal pngcairo enhanced\n");
        fprintf(gp, "set output '%s//%s%04d.png'\n", write_file_dir, write_file_header, i);
        fprintf(gp, "set multiplot\n"); // <steps in scan>,<steps between scans>
        fprintf(gp, "unset key\n");     // <steps in scan>,<steps between scans>

        fprintf(gp, "set lmargin screen 0.15\n"); // <steps in scan>,<steps between scans>
        fprintf(gp, "set rmargin screen 0.85\n"); // <steps in scan>,<steps between scans>
        fprintf(gp, "set tmargin screen 0.85\n"); // <steps in scan>,<steps between scans>
        fprintf(gp, "set bmargin screen 0.15\n"); // <steps in scan>,<steps between scans>

        fprintf(gp, "set xrange [%d:%d]\n", x_min, x_max);       // <steps in scan>,<steps between scans>
        fprintf(gp, "set xlabel '%s'offset 0.0,0.5\n", xxlabel); // <steps in scan>,<steps between scans>
        fprintf(gp, "set yrange [%d:%d]\n", y_min, y_max);       // <steps in scan>,<steps between scans>
        fprintf(gp, "set ylabel '%s'offset 1.0,0.0\n", yylabel); // <steps in scan>,<steps between scans>

        fprintf(gp, "set cblabel '%s'offset 0.0,0.0\n", cb_label);
        fprintf(gp, "set cbrange['%d':'%d']\n", cb_min, cb_max);
        fprintf(gp, "set colorbox vertical user origin 0.895, 0.2 size 0.025,0.6\n");
        fprintf(gp, "set palette rgbformulae 22,13,-31\n");

        fprintf(gp, "set pm3d map\n"); // <steps in scan>,<steps between scans>
        fprintf(gp, "splot '%s//%s%04d.dat'  using 2:1:3 with pm3d, '%s//%s%04d.dat' using 2:1:($1*0.0):(%lf*$5):(%lf*$4)::($1*0.0) with vectors head filled lt 2 lc 'black' \n", read_file_dir, read_file_header, i, read_file_dir, read_file_header, i, v_r, v_r);

        fprintf(gp, "set palette rgbformulae  0,0,0\n");
        fprintf(gp, "unset colorbox\n");
        fprintf(gp, "unset border\n");
        fprintf(gp, "splot '%s'  using 2:1:($3 == 0 ? NaN : $3) with pm3d\n", read_file_object);

        fprintf(gp, "unset multiplot\n"); // <steps in scan>,<steps between scans>

        fflush(gp); //Clean up Data

        fprintf(gp, "exit\n"); // Quit gnuplot
        pclose(gp);
    }
    return (0);
}