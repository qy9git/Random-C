#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <math.h> //cabs
#include <time.h> //nanosleep
#include <sys/ioctl.h> //ioctl
#include <signal.h> //sigaction
#include <limits.h> //UINT_MAX
#include <assert.h> //assert
#include <stdbool.h> //bool

static unsigned int N=100u;
static bool Border=true;
static double Range = 2.;
static void Display(void){
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz );
    --sz.ws_row;
    double ratioX, ratioY;
    if(2u*sz.ws_row<=sz.ws_col){
        ratioX=(double)sz.ws_col/(2.*(double)sz.ws_row);
        ratioY=1.;
    }else{
        ratioX=1.;
        ratioY=2.*(double)sz.ws_row/(double)sz.ws_col;
    }
    const unsigned int midX = sz.ws_col/2u;
    const unsigned int midY = sz.ws_row/2u;
    for(unsigned int i = 0u; i<sz.ws_row; ++i){
        const double Yval = ratioY * Range * ((double)i-(double)midY)/((double)midY);

        for(unsigned int j = 0u; j<sz.ws_col; ++j){

            const double Xval = ratioX * Range * ((double)j-(double)midX)/((double)midX);

            const double complex C = Xval + Yval * I;
            double complex Z = C;
            for(unsigned int i=0u; i<N; ++i)Z=Z*Z+C;

            if(cabs(Z)<=2)
                putchar('*');
            else{
                if(Border && Xval*Xval+Yval*Yval>=4){
                    printf("\u2588");
                }
                else
                    putchar(' ');
            }
        }
        putchar('\n');
    }
    printf("%lf,%lf,%u,%u ",ratioX,ratioY,sz.ws_col,sz.ws_row);
}

static void ReDisplay(int ignored){
    system("clear");
    const struct timespec K = {0l,100000000l};assert(K.tv_nsec <= 999999999l);
    nanosleep(&K,NULL); // to avoid constant flashing when rendering (100ms)
    Display();
    puts("lmqo");
}

int main(int argc, char**argv){
    if(argc >=2 && atoi(argv[1])>=0)
        N=(unsigned)atoi(argv[1]);
    Border = !(argc >= 3 && argv[2][0] == '-' && argv[2][1] == 'n' && argv[2][2] == 'b');
    const struct sigaction sa = {.sa_handler=ReDisplay};
    sigaction(SIGWINCH,&sa,NULL);
    Display();
    while(1){
        printf("Iterations: %u, Range: %lf, NoBorder: %s, Input (q/nb/b/it=/r=) : ", N, Range, !Border?"True":"False");
        char input[14];
        scanf("%13s",&input);
        if(input[0] == 'q')exit(0);
        if(input[0] == 'n' && input[1] == 'b')Border=false;
        if(input[0] == 'b')Border=true;
        if(input[0] == 'i' && input[1] == 't' && input[2] == '='){
            unsigned long Q=strtoul(&input[3],NULL,0);
            if(Q<=UINT_MAX)N=(unsigned int)Q;
        }
        if(input[0] == 'r' && input[1] == '='){
            double Q=strtod(&input[2],NULL);
            if(Q>0)Range=Q;
        }
        system("clear");
        Display();
    }
}
