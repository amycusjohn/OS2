//Amy Seidel
//CS4760 - OS 
//Project 1 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
int main( int argc, char *argv[] )  {
	
	int var;
	int hflag, nflag = 0;
	while ((var = getopt(argc, argv, "h")) != -1) { 
		switch(var) {
		  case 'h':
		    hflag = 1;
		    break;
		  case 'n':
		    nflag = 1;
		    default:
		 	nflag = 1; 
			hflag = 1;
			break;
		}
	}

	if((hflag ==1)) {
		printf("This is a help message\n");
		exit(0);
	}

}	
