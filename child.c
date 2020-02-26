#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <dirent.h>
#include <grp.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
int limitReached = 0;
bool primeChecker(int n, int b, int i, int currentSeconds, FILE *fp);

int getSeconds(){                                    //function to retrieve the current second in time from the computer
    time_t now;
    time(&now);                                                                       //get the current time from system
    struct tm *local = localtime(&now);                                                       //specifically the seconds
    int seconds = local->tm_sec;                                                                     //store the seconds
    return seconds;                                                                                          //return it
}
int main(void)
{
    printf("Hello world!\n");
    return 0;
}

bool primeChecker(int n, int b, int i, int currentSeconds, FILE *fp)  //function to evaluate the subset sum
{
    int x, j;
    int flag = 1;

    if(getSeconds() - currentSeconds == 2){       //if the current time is one more than one second from our start point
        limitReached = 2;                                                                  //mark time limitReached as 1
        return false;                                                                                     //return false
    }

    double root = sqrt(10);
    for(x = 0; x <=4; x++){
        for(j = 2; j <= root; j++){
            if(b % 2 == 1){
                // char temp[1000];
                int temp;
                temp = b;
                fprintf(fp, "%d is prime  \n", b);
                b = temp+i;
            }
            else{
                // char temp[1000];
                int temp;
                temp = b;
                fprintf(fp, "%d is not prime  \n", b);
                b = temp+i;
            }

        }
    }
    return true;

}
