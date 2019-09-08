//Amy Seidel
//CS4760 - OS 
//Project 1 
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <dirent.h>
#include <grp.h>
int main( int argc, char *argv[] )  {
	
	int var;
	int hflag,lflag,  Lflag, dflag,  Iflag = 0;
	int nflag = 0;
	int n = 4;
	while ((var = getopt(argc, argv, "hIdL")) != -1) { 
		switch(var) {
		  case 'h':
		   	hflag = 1;
		   	break;
		  case 'd':
			dflag = 1;
			break;
		  case 'I':
			Iflag = 1;
			break; 
		  case 'L':
			Lflag = 1;
			break;
		   default:
		 	lflag = 1; 
			hflag = 1;
			break;
		}
	}
	
	if(argv[optind] == NULL){
		printdir(".",0);
	}
	for(; optind < argc; optind++){      
       	//	tree(argv[optind], 4);
    	} 
	if((hflag ==1)) {
		printf("This is a help message\n");
		exit(0);
	}
	if((Iflag == 1)){
	}
	if((Lflag == 1)) {
		
	}
	if((dflag == 1)) {
		
	}
}


void printdir(char *dir, int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    int spaces = depth*4;
	
    //Vars for options
    char *p = getenv("USER");
    int gid = (atoi(dir));
    struct stat mystat; 
    struct group *grp = getgrgid(gid);
    char buf[512];
    


   /* if(NULL == (grp = getgrgid(mystat.st_gid)))
	{
	  perror("getpwuid()");
	}
*/

    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
            if(strcmp(".",entry->d_name) == 0 || 
                strcmp("..",entry->d_name) == 0 )
                continue;
	    
            printf("%*-4s%s/",spaces,"",entry->d_name);
       	    stat(buf, &mystat);
     	    printf(" %-8s", entry->d_name);
	    printf(" %-8zu",mystat.st_size);
	    printf(" %-8s", p);
            if(grp){
	 	 printf(" %-8s", grp->gr_name);
		}
            /* Recurse at a new indent level */
	    printf(" %-8x\n", mystat.st_mode);
            printdir(entry->d_name,depth+1);
        }
        else printf("%*s%s\n",spaces,"",entry->d_name);
    }
    chdir("..");
    closedir(dp);
}

