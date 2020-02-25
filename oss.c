//Amy Seidel
//CS4760 - OS
//Project 2
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

void child();
void parent();

int writing();
int reading();
int fill_buffer(char * bufptr, int size);

#define SHMKEY	859047             /* Parent and child agree on common key.*/
#define BUFF_SZ	sizeof ( int )
#define BUF_SIZE 1024
#define SHM_KEY 0x1234

struct shmseg {
    int cnt;
    int complete;
    char buf[BUF_SIZE];
};

//making all of the flags global so they can be changed in main and used in the function
int hflag, nflag, sflag, bflag, iflag, oflag;

int main( int argc, char *argv[] ) {

    //declaring variables for time
    time_t parentTime;
    parentTime = time(NULL);
    int defaultTime = 10;

    //variables
    int var;
    char *nVal;
    char * sVal;
    char * iVal;
    char *oVal;
    char * bVal;


    //variables for file
    int numProcess;
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
                //    nVal = 4;
                //   sVal = 2;
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
            fprintf(fpOut, "This is being written in the file. This is an int variable: %d", numProcess);
            printf("Filename: %s \n", oVal);
        }
    }
    else {
        perror("You need a -o value\n");
        exit(-1);
    }

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

    //SHARED MEMORY
    switch (fork())
    {
        case -1:
            perror("not good\n");
            return ( 1 );

        case 0:
            child();
            break;

        default:
            parent();
            break;
    }




    // writing();
   // reading();

}

void parent()
{
    // Get shared memory segment identifier

    int shmid = shmget ( SHMKEY, BUFF_SZ, 0777 | IPC_CREAT );
    if ( shmid == -1 )
    {
       // cerr << "Parent: ... Error in shmget ..." << endl;
        exit (1);
    }

    // Get the pointer to shared block

    char * paddr = ( char * )( shmat ( shmid, 0, 0 ) );
    int * pint = ( int * )( paddr );
int i;
    for ( i = 0; i < 10; i++ )
    {
        sleep ( 2 );

        *pint = 10 *i ;             /* Write into the shared area. */

        printf("Child: Read Val. =  %s \n", pint) ;
    }
}

void child()
{
    int i;
    sleep ( 5 );
    int shmid = shmget ( SHMKEY, BUFF_SZ, 0777 );

    if ( shmid == -1 )
    {
      //  cerr << "Child: ... Error in shmget ..." << endl;
        exit ( 1 );
    }

    int * cint = ( int * )( shmat ( shmid, 0, 0 ) );

    for (  i = 0; i < 10; i++ )
    {
        sleep ( 1 );
        printf("Child: Read Val. =  %s \n", cint) ;
    }
}






/*
int reading(){
    //HMMMMMMMM

    int shmid;
    struct shmseg *shmp;
    shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
    if (shmid == -1) {
        perror("Shared memory");
        return 1;
    }

    // Attach to the segment to get a pointer to it.
    shmp = shmat(shmid, NULL, 0);
    if (shmp == (void *) -1) {
        perror("Shared memory attach");
        return 1;
    }

    /* Transfer blocks of data from shared memory to stdout
    while (shmp->complete != 1) {
        printf("segment contains : \n\"%s\"\n", shmp->buf);
        if (shmp->cnt == -1) {
            perror("read");
            return 1;
        }
        printf("Reading Process: Shared Memory: Read %d bytes\n", shmp->cnt);
        sleep(3);
    }
    printf("Reading Process: Reading Done, Detaching Shared Memory\n");
    if (shmdt(shmp) == -1) {
        perror("shmdt");
        return 1;
    }
    printf("Reading Process: Complete\n");
    return 0;
}


int writing (){
    int shmid, numtimes;
    struct shmseg *shmp;
    char *bufptr;
    int spaceavailable;
    shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
    if (shmid == -1) {
        perror("Shared memory");
        return 1;
    }

    // Attach to the segment to get a pointer to it.
    shmp = shmat(shmid, NULL, 0);
    if (shmp == (void *) -1) {
        perror("Shared memory attach");
        return 1;
    }

    /* Transfer blocks of data from buffer to shared memory
    bufptr = shmp->buf;
    spaceavailable = BUF_SIZE;
    for (numtimes = 0; numtimes < 5; numtimes++) {
        shmp->cnt = fill_buffer(bufptr, spaceavailable);
        shmp->complete = 0;
        printf("Writing Process: Shared Memory Write: Wrote %d bytes\n", shmp->cnt);
        bufptr = shmp->buf;
        spaceavailable = BUF_SIZE;
        sleep(3);
    }
    printf("Writing Process: Wrote %d times\n", numtimes);
    shmp->complete = 1;

    if (shmdt(shmp) == -1) {
        perror("shmdt");
        return 1;
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("shmctl");
        return 1;
    }
    printf("Writing Process: Complete\n");
    return 0;
}

int fill_buffer(char * bufptr, int size) {
    static char ch = 'A';
    int filled_count;

    //printf("size is %d\n", size);
    memset(bufptr, ch, size - 1);
    bufptr[size-1] = '\0';
    if (ch > 122)
        ch = 65;
    if ( (ch >= 65) && (ch <= 122) ) {
        if ( (ch >= 91) && (ch <= 96) ) {
            ch = 65;
        }
    }
    filled_count = strlen(bufptr);

    //printf("buffer count is: %d\n", filled_count);
    //printf("buffer filled is:%s\n", bufptr);
    ch++;
    return filled_count;
}




*/



/*
int length = 0;
int num = o;

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


*/

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
