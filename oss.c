//Amy Seidel
//CS4760 - OS
//Project 2
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

#define _XOPEN_SOURCE 700
#define BUF_SIZE 1024

int limit = 0;

//functions from the robbins robbins text: myhandler() setupinterrupt() setuptimer()
static void myhandler(int s) {
    int errsave;
    errsave = errno;
    exit(0);
}
//https://www.qnx.com/developers/docs/6.4.1/neutrino/prog/inthandler.html
static int setupinterrupt(void) {
    struct sigaction act;
    act.sa_handler = myhandler;
    act.sa_flags = 0;
    return (sigemptyset(&act.sa_mask) || sigaction(SIGPROF, &act, NULL));
}

//https://pubs.opengroup.org/onlinepubs/7908799/xsh/systime.h.html
static int setuptimer(int n) {
    struct itimerval value;
    value.it_interval.tv_sec = n;
    value.it_interval.tv_usec = 0;
    value.it_value = value.it_interval;
    return (setitimer(ITIMER_PROF, &value, NULL));
}

//https://www.tutorialspoint.com/c_standard_library/c_function_time.htm
int getSeconds(){
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    int seconds = local->tm_sec;
    return seconds;
}
//class notes
struct shmseg {
    int cnt;
    int complete;
    char buf[BUF_SIZE];
};

bool primeChecker(int n, int b, int i, int cSecond, FILE *fp);
//making all of the flags global so they can be changed in main and used in the function
int hflag, nflag, sflag, bflag, iflag, oflag;

int main( int argc, char *argv[] ) {

    //declaring variables for time
    time_t pTime;
    pTime = time(NULL);
    int sec = 2;
    //variables
    int var;
    char *nVal;
    char * sVal;
    char * iVal;
    char *oVal;
    char * bVal;


    //variables for file
   // int numProcess;
    int lineNum = 0;
    static char buf[200];

    //while loop to execute getopt
    while ((var = getopt(argc, argv, "hnsbio")) != -1) {
        switch (var) {
            case 'h':
                hflag = 1;
                break;
            case 'n':
                nflag = 1;
                nVal = optarg;
                break;
            case 's':
                sflag = 1;
                sVal = optarg;
                break;
            case 'b':
                bflag = 1;
                bVal = optarg;
                break;
            case 'i':
                iflag = 1;
                iVal = optarg;
                break;
            case 'o':
                oflag = 1;
                oVal = optarg;
                break;
            case '?':
                perror("You chose an invalid option\n");
                exit(-1);
            default:
                break;
        }
    }

    int nt, st, bt, it;
    //getting arguments supplied by user
    if (optind < argc) {
            nVal = argv[optind];
            sVal = argv[++optind];
            bVal = argv[++optind];
            iVal = argv[++optind];
            oVal = argv[++optind];
            nt = atoi(nVal);
            st = atoi(sVal);
            bt = atoi(bVal);
            it = atoi(iVal);
        ++optind;
    }


    //hFlag checking
    if ((hflag == 1)) {
        printf("HELP: Enter oss -n x -s x -B x -i I -o filename.log \n");
        printf("-n Indicate the maximum total of child processes oss will ever create. (Default 4)\n");
        printf("-s Indicate the number of children allowed to exist in the system at the same time. (Default 2)\n");
        printf("-b Start of the sequence of numbers to be tested for primality\n");
        printf("-i Increment between numbers that we test\n");
        printf("-o filename for output file\n");
        exit(0);
    }


    //oFlag checking, opening the file
    if (oflag == 1) {
        FILE *fpOut = fopen(oVal, "w");
        if (fpOut == NULL) {
            fprintf(stderr, "Cannot write to file");
            perror("Failed: ");
            exit(1);
        }
        else {
           // fprintf(fpOut, "This is being written in the file\n");
           // printf("Filename: %s \n", oVal);
        }
    }
    else {
        perror("You need a -o value\n");
        exit(-1);
    }
    FILE *fpOut = fopen(oVal, "w");

    //nFlag checking,
    if (nflag == 1) {
        printf("n = %d\n", nt);
    }
    else {
        perror("You need a -n value\n");
        exit(-1);
    }

    //sFlag checking,
    if (sflag == 1) {
        printf("s = %d\n", st);
    }
    else {
        perror("You need a -s value\n");
        exit(-1);
    }

    //bFlag checking,
    if (bflag == 1) {
        printf("b = %d\n", bt);
    }
    else {
        perror("You need a -b value\n");
        exit(-1);
    }

    //iFlag checking,
    if (iflag == 1) {
        printf("i = %d\n", it);
    }
    else {
        perror("You need a -i value\n");
        exit(-1);
    }

    //BEGIN PROCESSING PART

    //Setting up the timer and interupts
    setupinterrupt();
    setuptimer(sec);

    //variable declarations
    int j;
    int wstatus;
    pid_t childPid, w;

    for(j = 0; j < nt; j++ ) {
        childPid = fork();
        if (childPid == 0) {

            //checking the time
            int cSecond = getSeconds();
            fprintf(fpOut, "Launched at %d ", cSecond);
            bool flag = primeChecker( nt, bt, it, cSecond, fpOut);
            char error[100];

        //if it is a prime
        if(flag == true) {
                char temp[10] = {0};
                char sPid[1000] = {0};
                sprintf(sPid, "%d", getpid());
                strcat(sPid, ": ");
                fputs(sPid, fpOut);
                cSecond = getSeconds();
                fprintf(fpOut, "Current time: %d  \n", cSecond);
            }

        //process terminated without checking primality
        if (limit == 1) {
            char temp[20];
            sprintf(temp, "%d", getpid());
            strcpy(error, temp);
            strcat(error, ": Process terminated without checking\n");
            fputs(error, fpOut);
            limit = 0;
        }

    }

    //wait for child to end http://man7.org/linux/man-pages/man2/wait.2.html
    else{
        do {
            w = waitpid(childPid, &wstatus, WUNTRACED | WCONTINUED);
            if (w == -1) {
                break;
            }
        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
    }
        int cSecond = getSeconds();
        char temp[50];
        sprintf(temp, "%d: ", getpid());
        fputs(temp, fpOut);
        fprintf(fpOut, "Terminated at: %d  \n", cSecond);
    }
fclose(fpOut);

    exit(0);

}

bool primeChecker(int n, int b, int i, int cSecond, FILE *fp)  //function to evaluate the subset sum
{
    int x, j;
    int flag = 1;

    //check if more than 2 seconds has passed
    if(getSeconds() - cSecond == 2){
        limit = 2;
        return false;
    }

    //checking if it is a prime
double root = sqrt(10);
    for(x = 0; x <=4; x++){
        for(j = 2; j <= root; j++){
            if(b % 2 == 1){
               int temp;
                temp = b;
                fprintf(fp, "%d is prime  \n", b);
                b = temp+i;
            }
            else{
                int temp;
                temp = b;
                fprintf(fp, "%d is not prime  \n", b);
                b = temp+i;
            }

        }
    }
return true;

}




