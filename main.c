//Amy Seidel
//CS4760 - OS 
//Project 1 
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
void printdir(char *dir, int indent);

//making all of the flags global so they can be changed in main and used in the function
int hflag,tflag, gflag, pflag, iflag, uflag, sflag, Lflag, dflag,  Iflag = 0;
int n = 4;

int main( int argc, char *argv[] )  {
	
	//variables 
	int var;

	//while loop to execute getopt
	while ((var = getopt(argc, argv, "hILldgipstu")) != -1) { 
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
		  case 't':
			tflag = 1;
			break;
		  case 'p': 
			pflag = 1;
			break;
		  case 'i':
			iflag = 1;
			break;
		  case 'u':
			uflag = 1;
			break;
		  case 'g':
			gflag = 1;
			break;
		  case 's':
			sflag = 1;
			break;
		  case 'l':
			uflag = 1;
			tflag = 1;
			iflag = 1;
			pflag = 1;
			gflag = 1;
			sflag = 1;
		   default:
			break;
		}
	}
	if((hflag ==1)) {
		printf("HELP: Enter ./dt -options directory name\n");
		exit(0);
	}
	if(argv[optind] != NULL){
		printdir(argv[optind],0);
	
	}
	//prints current directory if there is no argument provided
	else{
		printdir(".", 0);
	}
    	 
}
/*****************************************
 * FUNCTION TO PRINT DIRECTORY AND OPTIONS
 *****************************************/

void printdir(char *dir, int indent)
{
    //variables for declaring directory and stat
    DIR *d;
    struct dirent *entry;
    struct stat statbuf;
    int spaces = indent+n;
	
    //Vars for options
    char *p = getenv("USER");
    int gid = (atoi(dir)); 
    struct group *grp = getgrgid(gid);
    char buf[512];
   
   //Error checking if it is a real directory name or not
    if((d = opendir(dir)) == NULL) {
	fprintf(stderr, "ERRO %s: ", dir);
	perror(""); 
        return;
    }

    chdir(dir);
    while((entry = readdir(d)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode))
	 {
            // ignoring "." and ".." directories
            if(strcmp(".",entry->d_name) == 0 || 
                strcmp("..",entry->d_name) == 0 )
                continue;
	    
	//Printing information
            printf("%s",entry->d_name);
     	
	    //print permissions
	    if(pflag == 1){
		printf("\t");
		printf( (S_ISDIR(statbuf.st_mode)) ? "d" : "-");	  	
		printf( (statbuf.st_mode & S_IRUSR) ? "r" : "-");
		printf( (statbuf.st_mode & S_IWUSR) ? "w" : "-");
		printf( (statbuf.st_mode & S_IXUSR) ? "x" : "-");
		printf( (statbuf.st_mode & S_IRGRP) ? "r" : "-");
		printf( (statbuf.st_mode & S_IWGRP) ? "w" : "-");
		printf( (statbuf.st_mode & S_IXGRP) ? "x" : "-");
		printf( (statbuf.st_mode & S_IROTH) ? "r" : "-");
		printf( (statbuf.st_mode & S_IWOTH) ? "w" : "-");
		printf( (statbuf.st_mode & S_IXOTH) ? "x" : "-");
		
	    }
            //print number of links
            if(iflag == 1){
	  	  printf("\t%d", statbuf.st_ino);
	    }
      	    //print UID
	    if(uflag == 1){ 
	   	 printf("\t%s", p);
	    }	
           //print group id
           if(gflag == 1){ 
		   if(grp){
		 	 printf("\t%s", grp->gr_name);
			}
	   }
	   if(sflag == 1){		
	     printf("\t%d",statbuf.st_size);
	   }

	   //printing time
	   if(dflag == 1){
	    printf("\t%s\n", ctime(&statbuf.st_mtime));
	  } 
	   printdir(entry->d_name, indent+n); 
        }
        else printf("%*s%s\n ",spaces,"",entry->d_name);
    }
    chdir("..");
    closedir(d);
}

