/* *************************************************/
/* Terminal vty screenshot application:            */
/* - Only real vty terminal (no xterm, st, ..)     */
/* - No colors or anything, just plain screen text */
/*                                                 */
/* By: prenex                                      */
/* See: http://github.com/prenex/termshot          */
/*                                                 */
/* Licence: unlicence                              */
/* *********************************************** */

#include "stdio.h"
#include "ctype.h"
#include "string.h"
#include "dirent.h"
#include "stdlib.h"
#include "unistd.h"

/* **************** */
/* Config variables */
/* **************** */

#define BOOLE int

/* Make higher if you screenshot automated and it starts to override! */
#define MAX_OFILE_LEN 256

/* Default files ("current" vty) */
char *d_vcspath = "/dev/vcs";
char *d_vcsapath = "/dev/vcsa";

/* Used when telling which vty to termshot */
char *n_vcspath = "/dev/vcs1";
char *n_vcsapath = "/dev/vcsa1";

/* Used only as a fallback - should never happen: */
char *outfile_pattern = "tshot_%d.txt";

/* ***************** */
/* Dynamic variables */
/* ***************** */

/* The target vty (0..9) as parameter */
char vty_target_num;

/* Used for indexing last character of a cstring */
int lastindex;

/* Used to declare filename*/
int filenum = 0;

/* Current directory listing */
DIR *d;
struct dirent *entry;

/* The %d part of the outfile_pattern for the current entry */
int entrys_num;

/* File handles */

FILE *vcs;
FILE *outf;

/* Dimensions of the window for the vty */
unsigned int w_width;
unsigned int w_height;

/* Currenty used strings */
char *vcspath;
char *vcsapath;
BOOLE needfree;

/* Output file string handling vars */
char outfilename[MAX_OFILE_LEN];

/* For reading a line in from the screen memory */
char *linebuffer;

/* **** */
/* Code */
/* **** */

int main(int argc, char **argv) {
	/* Handle parameters */
	if(argc == 2) {
		/* Check if we got a vty number */
		vty_target_num = argv[1][0];
		if(isdigit(vty_target_num)) {
			/* Use the given vty number */

			/* Copy strings so we can modify them (they are const char* in code)*/
			vcspath = strdup(n_vcspath);
			vcsapath = strdup(n_vcsapath);
			needfree = 1; /* will need to call free! */

			/* vcs change */
			lastindex = strlen(vcspath);
			vcspath[lastindex-1] = vty_target_num;

			/* vcsa change */
			lastindex = strlen(vcsapath);
			vcsapath[lastindex-1] = vty_target_num;
		} else {
			/* Handle "--help" and other errors */
			printf("Usage:\n");
			printf("------\n");
			printf("\n");
			printf("	termshot        - shoot current vty\n");
			printf("	termshot [0..9] - shoot vty0..vty9\n");
			printf("	                 (usually there is only vty1..vty6)\n");
			printf("\n");
			printf("Remark: Doesn't work for graphics term (xterm, st, gnome-terminal, etc.)\n");
			printf("\n");

			/* EXIT */
			return 0;
		}
	} else {
		/* Set default paths if there is no parameter (or wrong count thereof) */
		vcspath = d_vcspath;
		vcsapath = d_vcsapath;
		needfree = 0; /* Shouldn't call free! */
	}

	/* TODO: Get width and height */
	w_width = 10;
	w_height = 10;

	/* Create numbered output filename: tshot_<filenum> */
	d = opendir(".");
	/* See the biggest file entrys filenum corresponding to the pattern */
	if(d) {
		while((entry = readdir(d)) != NULL) {
			/*printf("%s\n", entry->d_name);*/
			entrys_num = 0;
			sscanf(entry->d_name, outfile_pattern, &entrys_num);
			/*printf(" - entrys_num = %d\n", entrys_num);*/
			if(entrys_num >= filenum) {
				filenum = entrys_num + 1;
			}
		}
		closedir(d);
	}

	/* This creates the filename: tshot_<filenum> */
	snprintf(outfilename, MAX_OFILE_LEN, outfile_pattern, filenum);

	/* Process file */
	if(w_width > 0 && w_height > 0) {
		/* Allocate line-buffer */
		/* +1 more is added so that the cstring zero terminator is there */
		linebuffer = (char*) malloc(sizeof(char) * (w_width+1));
		/* Open vcs* file */
		vcs = fopen(vcspath, "r");
		if(vcs) {
			outf = fopen(outfilename, "w");
			if(outf) {
				/* Loop through lines of the screen with binary read */
				while((fread((void*)linebuffer, 1, w_width, vcs) > 0) && (w_height--)) {
					linebuffer[w_width] = ' '; /* Extra space is needed for algo */

					/* Overwrite whitespace with 0 as early as possible */
					lastindex = w_width;
					while(isspace(linebuffer[lastindex])) --lastindex;
					linebuffer[lastindex+1] = 0; /* zero terminate: endl */

					/* Write out result - with endlines! */
					fprintf(outf, "%s\n", linebuffer);
				}

				fclose(outf);
			}
			fclose(vcs);

			/* Notify user */
			printf("Termshot saved: %s -> %s\n", vcspath, outfilename);
		} else {
			fprintf(stderr, "Cannot open %s, try with proper rights!\n", vcspath);
		}
	}

	/* Cleanup */
	if(needfree) {
		/* Only when we used strdup to modify these */
		free(vcspath);
		free(vcsapath);
	}
	if(w_width > 0) {
		/* Only if there was a w_width */
		free(linebuffer);
	}

	/* EXIT */
	return 0;
}

/* vim: set ts=4 sw=4 noet : */
