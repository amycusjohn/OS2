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
		traverse(".", n);
	}
	for(; optind < argc; optind++){      
       		traverse(argv[optind], 4);
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


void traverse(const char *name, int indent){
	DIR *mydir;
	struct dirent *myfile;
	struct stat mystat;
	char buf[512];
	errno = 0;
	char *p = getenv("USER");
	
	struct group *gid;
	gid_t g;
	gid = getgrnam(name);
	gid->gr_gid;;

	if(!(mydir = opendir(name))){
		return;}
	while((myfile = readdir(mydir)) != NULL)
	    {
       		 sprintf(buf, "%s/%s", name, myfile->d_name);
       		 stat(buf, &mystat);
       		 printf(" %s", myfile->d_name);
		 printf(" %zu",mystat.st_size);
		 printf(" %s", p);
	//	 printf(" %s\n", gid);
	//inodes 	 printf(" %d\n",mystat.st_ino);
	    }
 
    closedir(mydir);
}














	
