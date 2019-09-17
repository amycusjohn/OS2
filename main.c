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
bool isSubsetSum(int set[], int n, int sum);

//making all of the flags global so they can be changed in main and used in the function
int hflag, iflag, oflag, tflag = 0;

int main( int argc, char *argv[] )  {
	
	//variables
	int var;
	char * iVal;
	char * oVal;
	char *tVal;

	//while loop to execute getopt
	while ((var = getopt(argc, argv, "hiot")) != -1) {
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
		  case 't':
			tflag = 1;
			tVal = optarg;
			break;
		   default:
			break;
		}
	}
	//getting arguments supplied by user
    if (optind < argc)
    {
               iVal = argv[optind];
          //     printf("ival %s ", iVal);
               oVal = argv[++optind];
          //    printf("oval %s ", oVal);
               tVal = argv[1+optind];
              // printf("tval %s ", tVal);
        ++optind;
    }

    //hFlag checking
	if((hflag == 1)) {
		printf("Legal Inputs: -h, -i inputfilename, -o outputfilename, -t time."
         " The default is input.dat output.dat 10\n");
		exit(0);
	}

    //iFlag checking
	if(iflag == 1){
        FILE *fp = fopen(iVal, "w");
        if(fp == NULL)
        {
            printf("Error!");
            exit(1);
        }
    }
	//Default case
	else{
        FILE *fp = fopen("input.dat", "w");
        if(fp == NULL)
        {
            printf("Error!");
            exit(1);
        }
    }
    //iFlag checking, opening the file
    if(iflag == 1){
        FILE *fpIn = fopen(iVal, "r");
        if(fpIn == NULL)
        {
            fprintf(stderr, "Cannot read from input.txt");
            exit(1);
        }
    }
    //Default case
    else{
        FILE *fpIn = fopen("input.dat", "r");
        if(fpIn == NULL)
        {
            fprintf(stderr, "Cannot read from input.txt");
            exit(1);
        }
    }

    //iFlag checking, opening the file
    if(iflag == 1){
        FILE *fpOut = fopen(oVal, "w");
        if(fpOut == NULL)
        {
            fprintf(stderr, "Cannot read from file");
            exit(1);
        }
    }
        //Default case
    else{
        FILE *fpOut = fopen("output.dat", "w");
        if(fpOut == NULL)
        {
            fprintf(stderr, "Cannot read from output.dat");
            exit(1);
        }
    }






    int set[] = {3, 34, 4, 12, 5, 2};
    int sum = 9;
    int n = sizeof(set)/sizeof(set[0]);
    if (isSubsetSum(set, n, sum) == 1)
        printf(" ");
    else
       printf(" No subset with given sum");
    return 0;

}
/*****************************************
 * FUNCTION TO CHECK SUBSET
 *****************************************/
bool isSubsetSum(int set[], int n, int sum)
{
    int i;
    int size = 8;
    // Base Cases
    if (sum == 0)
        return 1;

    if (n == 0 && sum != 0) {

        return 1;
    }

    // If last element is greater than sum, then ignore it
    if (set[n-1] > sum)
        return isSubsetSum(set, n-1, sum);


    /* else, check if sum can be obtained by any of the following
       (a) including the last element
       (b) excluding the last element   */
    return isSubsetSum(set, n-1, sum) ||
           isSubsetSum(set, n-1, sum-set[n-1]);

}