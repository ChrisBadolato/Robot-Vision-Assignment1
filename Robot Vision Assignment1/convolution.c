//Christopher Badolato
//Robot Vision
//1/10/2020


#include <stdio.h>                          /* Sobel.c */
#include <math.h>
#include <string.h>

    int pic[256][256];
    int outpicx[256][256];
    int outpicy[256][256];
    int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
    double ival[256][256],maxival;

int main (){
    int i,j,p,q,mr,sum1,sum2;
    double threshold, threshold2;
    FILE *fo1, *fo2, *fo3, *fp1, *fopen();
    char header1[100], string[200];
    int header2, header3, header4;

    fp1=fopen("C:\\Users\\Chris Badolato\\Desktop\\RobotVision\\garb34.pgm","rb");
    fscanf(fp1, "%s", header1);
    fscanf(fp1, "%s", string);
    fscanf(fp1, "%d %d", &header2, &header3);
    fscanf(fp1, "%d", &header4);

    fo1=fopen("C:\\Users\\Chris Badolato\\Desktop\\RobotVision\\garbHighThreshold.pgm","wb");
    fprintf(fo1,"P5\n");
    fprintf(fo1,"%d %d\n", 256, 256);
    fprintf(fo1,"255\n");

    fo2=fopen("C:\\Users\\Chris Badolato\\Desktop\\RobotVision\\garbLowThreshold.pgm","wb");
    fprintf(fo2,"P5\n");
    fprintf(fo2,"%d %d\n", 256, 256);
    fprintf(fo2,"255\n");

    fo3=fopen("C:\\Users\\Chris Badolato\\Desktop\\RobotVision\\garbGradientMagnitude.pgm","wb");
    fprintf(fo3,"P5\n");
    fprintf(fo3,"%d %d\n", 256, 256);
    fprintf(fo3,"255\n");

    threshold = 175;
    threshold2 = 30;

    for (i=0;i<256;i++){
        for (j=0;j<256;j++){
            pic[i][j]  =  getc (fp1);
            pic[i][j]  &= 0377;
        }
    }

    mr = 1;
    for (i=mr;i<256-mr;i++){
        for (j=mr;j<256-mr;j++){
            sum1 = 0;
            sum2 = 0;
            for (p=-mr;p<=mr;p++){
                for (q=-mr;q<=mr;q++){
                    sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                    sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                }
            }
        outpicx[i][j] = sum1;
        outpicy[i][j] = sum2;
        }
    }

    maxival = 0;
    for (i=mr;i<256-mr;i++){
        for (j=mr;j<256-mr;j++){
            ival[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) + (outpicy[i][j]*outpicy[i][j])));
            if (ival[i][j] > maxival)
                maxival = ival[i][j];
        }
    }
        //File 1
    for (i=0;i<256;i++){
        for (j=0;j<256;j++){
            ival[i][j] = (ival[i][j] / maxival) * 255;
            if(ival[i][j] >= threshold){
                fprintf(fo1,"%c",(char)((int)(ival[i][j])));
            }
            else{
                fprintf(fo1,"%c",(char)((int)(0)));
            }
        }
    }
            //File 2
    for (i=0;i<256;i++){
        for (j=0;j<256;j++){
            if(ival[i][j] >= threshold2){
            fprintf(fo2,"%c",(char)((int)(ival[i][j])));
                }
            else{
                fprintf(fo2,"%c",(char)((int)(0)));
            }
        }
    }
            //File 3
    for (i=0;i<256;i++){
        for (j=0;j<256;j++){

            fprintf(fo3,"%c",(char)((int)(ival[i][j])));
        }
    }

        fclose(fo1);
        fclose(fo2);
        fclose(fo3);
        fclose(fp1);

}
