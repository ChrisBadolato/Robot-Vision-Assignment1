//Christopher Badolato
//Robot Vision
//1/10/2020


#include <stdio.h>                  /*  Marr-Hildreth.c  (or marrh.c) */
#include <math.h>
#include <string.h>
#define  PICSIZE 256
#define  MAXMASK 100

        int    pic[PICSIZE][PICSIZE];
        double outpic1[PICSIZE][PICSIZE];
        double outpic2[PICSIZE][PICSIZE];
        int outpicx[256][256];
        int outpicy[256][256];
        int    edgeflag[PICSIZE][PICSIZE];
        double maskx[MAXMASK][MAXMASK];
        double masky[MAXMASK][MAXMASK];
        double conv[PICSIZE][PICSIZE];
        double ival[256][256];
        double peak[256][256];
        double cannyPart4Out[256][256];
        int scaledMagnitude[256][256];
        int histogram[256];

int main(){
    int     i,j,p,q,s,t,mr,centx,centy;
    double  maskval,sum1,sum2,sig,maxival,minival,maxval,ZEROTOL,maskxval,maskyval,slope = 0;
    double cutOff = 0, percent, HI, LOW;
    FILE    *fo1, *fo2, *fo3, *fp1, *fopen();
        char header1[100], string[200];
    int header2, header3, header4, threshold, areaOfTops = 0, moreToDo = 1;


    fp1=fopen("C:\\Users\\Chris Badolato\\Desktop\\RobotVision\\garb34.pgm","rb");
    fscanf(fp1, "%s", header1);
    fscanf(fp1, "%s", string);
    fscanf(fp1, "%d %d", &header2, &header3);
    fscanf(fp1, "%d", &header4);

    fo1=fopen("C:\\Users\\Chris Badolato\\Desktop\\RobotVision\\garbCanny1.pgm","wb");
    fprintf(fo1,"P5\n");
    fprintf(fo1,"%d %d\n", 256, 256);
    fprintf(fo1,"255\n");

    fo2=fopen("C:\\Users\\Chris Badolato\\Desktop\\RobotVision\\garbCanny2.pgm","wb");
    fprintf(fo2,"P5\n");
    fprintf(fo2,"%d %d\n", 256, 256);
    fprintf(fo2,"255\n");

    fo3=fopen("C:\\Users\\Chris Badolato\\Desktop\\RobotVision\\garbCanny3.pgm","wb");
    fprintf(fo3,"P5\n");
    fprintf(fo3,"%d %d\n", 256, 256);
    fprintf(fo3,"255\n");

    sig = 1;

    ZEROTOL = 1;
    threshold = 1;

    mr = (int)(sig * 6) + 1;
    centx = (MAXMASK / 2);
    centy = (MAXMASK / 2);

    for (i=0;i<256;i++)
    { for (j=0;j<256;j++)
            {
              pic[i][j]  =  getc (fp1);
            }
    }

    for (p=-mr;p<=mr;p++){
        for (q=-mr;q<=mr;q++){
                // q = x
                // p = y
            maskxval = (q * exp(-(((q*q) + (p*p))/(2 * (sig * sig)))));
            maskyval = (p * exp(-(((q*q) + (p*p))/(2 * (sig * sig)))));
            maskx[p+centy][q+centx] = maskxval;
            masky[p+centy][q+centx] = maskyval;
       }
    }

    for (i=mr;i<=255-mr;i++){
        for (j=mr;j<=255-mr;j++){
        sum1 = 0;
        sum2 = 0;
            for (p=-mr;p<=mr;p++){
                for (q=-mr;q<=mr;q++){
                    sum1 += pic[i+p][j+q] * maskx[p+centy][q+centx];
                    sum2 += pic[i+p][j+q] * masky[p+centy][q+centx];
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

    for(i = 0; i < 256; i++){
        histogram[i] = 0;
    }

    int currentInt = 0;
    for (i=0;i<256;i++){
        for (j=0;j<256;j++){
            currentInt = floor(ival[i][j]);
            histogram[currentInt]++;
        }
    }

    percent = .05;
    cutOff = (percent*256*256);
    for(i = 256; i > 0; i--){
        areaOfTops += histogram[i];
        if(areaOfTops>cutOff){
            break;
        }
        HI = i;
        LOW = (.35 * HI);
    }


   for(i=mr;i<256-mr;i++){
        for(j=mr;j<256-mr;j++){
            if((outpicx[i][j]) == 0.0) {
                outpicx[i][j] = .00001;
            }
            slope = (double) outpicy[i][j]/outpicx[i][j];

            if( (slope <= .4142)&&(slope > -.4142)){
                if((ival[i][j] > ival[i][j-1])&&(ival[i][j] > ival[i][j+1])){
                    peak[i][j] = 255;
                }
            }
            else if( (slope <= 2.4142)&&(slope > .4142)){
                if((ival[i][j] > ival[i-1][j-1])&&(ival[i][j] > ival[i+1][j+1])){
                    peak[i][j] = 255;
                }
            }
            else if( (slope <= -.4142)&&(slope > -2.4142)){
                if((ival[i][j] > ival[i+1][j-1])&&(ival[i][j] > ival[i-1][j+1])){
                    peak[i][j] = 255;
                }
            }
            else{
                if((ival[i][j] > ival[i-1][j])&&(ival[i][j] > ival[i+1][j])){
                    peak[i][j] = 255;
                }
            }
        }
    }
    for (i=0;i<256;i++){
        for (j=0;j<256;j++){
            if(peak[i][j] == 255){
                if(ival[i][j] > HI){
                    peak[i][j] = 0;
                    cannyPart4Out[i][j] = 255;
                }
                else if(ival[i][j] < LOW){
                    peak[i][j] = 0;
                    cannyPart4Out[i][j] = 0;
                }
            }
        }
    }
    moreToDo = 1;

    while(moreToDo == 1){
        moreToDo = 0;
        for (i=0;i<256;i++){
            for (j=0;j<256;j++){
                if(peak[i][j] == 255){
                    for(p = -1; p <=1; p++){
                        for(q = -1; q <=1; q++){
                            if(cannyPart4Out[i+p][j+q] == 255){
                                peak[i][j] = 0;
                                cannyPart4Out[i][j] = 255;
                                moreToDo = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    for (i=0;i<256;i++){
        for (j=0;j<256;j++){
            ival[i][j] = (ival[i][j] / maxival) * 255;
            fprintf(fo1,"%c",(char)(int)(ival[i][j]));
        }
    }
    for (i=0;i<256;i++){
        for (j=0;j<256;j++){
            peak[i][j] = (peak[i][j] / maxival) * 255;
            fprintf(fo2,"%c",(char)(int)(peak[i][j]));
        }
    }
    for (i=0;i<256;i++){
        for (j=0;j<256;j++){
            cannyPart4Out[i][j] = (cannyPart4Out[i][j] / maxival) * 255;
            fprintf(fo3,"%c",(char)(int)(cannyPart4Out[i][j]));
        }
    }
}
