/******************************************************************************
PROGRAM NAME :
AUTHER :
DATE :
Think a Bit , Code a Bit , Test a Bit
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
const char *output_data_file = " result.dat "; // name of output file
const int x_grid = 30;                           // number of grids in x direction
const int y_grid = 30;                           // number of grids in y direction
const int grid_space = 10;                       // grid width
const double PI = 4.0 * atan(1.0);               // Cal . Pi
float u[x_grid][y_grid];                         // u vector array
float v[x_grid][y_grid];                         // v vector array
float U[x_grid][y_grid];                         // absolute vector array
FILE *output_file;                               // pointer for output file
/*********************************
MAIN
*********************************/
main()
{
    int i, j;
    // preparing for output file
    output_file = fopen(output_data_file, " w ");
    // Calc . 2 D velocity vector and absolute value of velocity field
    for (i = 0; i < x_grid; i++)
    {
        for (j = 0; j < y_grid; j++)
        {
            // Sample vector field and absolute value of vector
            u[i][j] = sin(2.0 * PI / x_grid * i);
            v[i][j] = cos(2.0 * PI / y_grid * j);
            U[i][j] = sqrt(u[i][j] * u[i][j] + v[i][j] * v[i][j]);
            fprintf(output_file, "%d \t %d \t %.3lf \t %.3lf \t %.3lf \n ", i * grid_space, j * grid_space, U[i][j], u[i][j], v[i][j]);
            // Caution : just line breaking for printing this document
            printf("%d \t %d \t %.3lf \t %.3lf \t %.3lf \n ", i * grid_space, j * grid_space, U[i][j], u[i][j], v[i][j]);
        }
    }
    fclose(output_file);
    return (0);
}