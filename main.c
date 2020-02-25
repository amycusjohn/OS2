//Amy Seidel
//CS4760 - OS 
//Project 2
#include <time.h>
#include <sys/types.h>
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


void displaySubset(int subSet[], int size, time_t now, FILE *out);
void findSubset(int set[], int size, int sum, time_t time, FILE *out);
void subsetSum(int set[], int subSet[], int n, int subSize, int total, int nodeCount ,int sum, time_t time, FILE *out);

//making all of the flags global so they can be changed in main and used in the function
int hflag, nflag, sflag, bflag, iflag, oflag;

int main( int argc, char *argv[] )  {

    //declaring variables for time
    time_t parentTime;
    parentTime = time(NULL);
    int defaultTime = 10;

	//variables
	int var;
	char * nVal;
	char * sVal;
	char * iVal;
	char * oVal;
	char * bVal;


	//variables for file
    int numProcess;
    int lineNum = 0;
    static char buf[200];

	//while loop to execute getopt
	while ((var = getopt(argc, argv, "hnsbio")) != -1) {
		switch(var) {
		  case 'h':
		   	hflag = 1;
		   	break;
		  case 'i':
			iflag = 1;
			iVal = optarg;
			break;
		  case 'o':
			oflag = 1;
			oVal = optarg;
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
            default:
			break;
		}
	}

	//getting arguments supplied by user
    if (optind < argc)
    {
               iVal = argv[optind];
               oVal = argv[++optind];
           //    tVal = argv[optind];
           //    defaultTime = atoi(tVal);
               ++optind;
    }


    //hFlag checking
    if((hflag == 1)) {
        printf("HELP: Enter oss -n x -s x -B x -i I -o filename.log \n");
        printf("-n Indicate the maximum total of child processes oss will ever create. (Default 4)\n");
        printf("-s Indicate the number of children allowed to exist in the system at the same time. (Default 2)\n");
        printf("-b Start of the sequence of numbers to be tested for primality\n");
        printf("-i Increment between numbers that we test\n");
        printf("-o filename for output file\n");
        exit(0);
    }





    //oFlag checking, opening the file
    if(oflag == 1){
        FILE *fpOut = fopen(oVal, "w");
        if(fpOut == NULL)
        {
            fprintf(stderr, "Cannot write to file");
            perror("Failed: ");
            exit(1);
        }
        else{
            fprintf(fpOut, "This is being written in the file. This is an int variable: %d", numProcess);
        }
    }
    //Default case
    else{
        char * outFile = "output.dat";
        FILE *fpOut = fopen("output.dat", "w");
        strcpy(oVal, outFile);
        FILE *fpIn = fopen(iVal, "r");
        if(fpOut == NULL)
        {
            fprintf(stderr, "Cannot write to output.dat");
            perror("Failed: ");
            exit(1);
        }
        else{
           // fprintf(fpOut, "This is being written in the file. This is an int variable: %d", numProcess);
        }
    }

    //Finished with file processing


    //Begin getting strings from the file
    int i, n, count = 0;
    char line[64][126];
    FILE *fpIn = fopen(iVal, "r");
    FILE *fpOut = fopen(oVal, "w");


    //gets line from file and stores it in new array, line
    //referenced from https://www.w3resource.com/c-programming-exercises/file-handling/c-file-handling-exercise-4.php
    while(fgets(line[i], sizeof(line[i]), fpIn)) {
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
    fclose(fpIn);

    //finding the length of each line from the file, processing it
    for(i=1; i< numProcess+1; i++) {

        //variables
        int length = 0;
        int j;
        int k = 0;
        //finding the length of each line of the file
        for (j = 0; line[i][j] != '\0'; j++) {
            if (isspace(line[i][j])) {
                length++;
            }
        }

        //new integer array of size length for each line in the file
        // filling it with 0s from 0 - length of the line
        int num[length];
        for (j = 0; j < length + 1; j++) {
            num[j] = 0;
        }

        //fill num array with the values from the the orginal array
        //checks if there is a space and then fixes the integers to be correct
        //referenced https://www.ntu.edu.sg/home/ehchua/programming/cpp/c1_Basics.html

        for (j = 0; line[i][j] != '\0'; j++) {
            if (isspace(line[i][j])) {
                k++;
            } else {
                num[k] = num[k] * 10 + (line[i][j] - 48);
            }
        }

        //finished with the array business so time to start working with processes
        //variables for wait status and forking
        //https://pubs.opengroup.org/onlinepubs/9699919799/functions/waitpid.html
        //https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.2.0/com.ibm.zos.v2r2.bpxbd00/rtwaip.htm
        int wstatus;
        pid_t pid = fork(), waitStat;

        if(pid == -1) {
            perror("logParse: Child process failed\n");
            exit(0);
        }

        //making the parent wait
        else if(pid > 0) {
            do {
                waitStat = waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);
                if (waitStat == -1) break;
            } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

        }
        //working on the child process
        //https://stackoverflow.com/questions/35020540/how-and-why-sizeofa-sizeofa0-in-c-is-used-to-calculate-the-number-of-eleme
        else{
            int sum[length-1];
            int x;
            int total = num[0];

            for (x = 1 ; x <sizeof(num)/sizeof(num[0])+1; x++) {
                sum[x-1] = num[x];
            }

            //setting timer for the child process
            time_t now;
            now = time(NULL);

            //printing pid, = subset/no subset found
            fprintf(fpOut, "%d: ", getpid());
            fprintf(fpOut, "%d = ", num[0]);
          //  findSubset(sum, sizeof(sum)/sizeof(sum[0]), num[0], now, fpOut);
            exit(0);
        }

    }

    //timer for parent process
    time_t currentTime;
    currentTime = time(NULL);

    if(parentTime - currentTime == defaultTime){
        printf("%d: ", getpid());
        perror("logParse: Processesing took too long\n");
        exit(1);
    }
    printf( "LogParse tried to find the sums. Please check your output file.\n");
}




/*****************************************
 * FUNCTION TO CHECK SUBSET
 * taken from https://www.tutorialspoint.com/Subset-Sum-Problem
 *****************************************/
/*
void displaySubset(int subSet[], int size, time_t now, FILE *out) {
   int i;
   time_t seconds;
   seconds = time(NULL);
   if( seconds - now == 1) {
       printf("%d: ", getpid());
       perror("logParse: Processesing took longer than 1 second\n");
       exit(0);
   }
       for(i = 0; i < size; i++) {
           fprintf(out, "%d  ", subSet[i]);
       }
       fprintf(out, "\n");

}

void subsetSum(int set[], int subSet[], int n, int subSize, int total, int nodeCount ,int sum, time_t time, FILE *out) {
   int i;
   int count = 0;

   if( total == sum) {
       displaySubset(subSet, subSize, time, out);     //print the subset
       return;
   }
   else {
       for( i = nodeCount; i < n; i++ ) {     //find node along breadth
           subSet[subSize] = set[i];
           subsetSum(set,subSet,n,subSize+1,total+set[i],i+1,sum, time, out);     //do for next node in depth
           count++;
       }
   }
   if((count == subSize) && (subSet[1] != NULL)){
           fprintf(out,"No subset found\n");
   }
}

void findSubset(int set[], int size, int sum, time_t time, FILE *out) {
   int *subSet[size];     //create subset array to pass parameter of subsetSum

   subsetSum(set, subSet, size, 0, 0, 0, sum, time, out);
}
*/