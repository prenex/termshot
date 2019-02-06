/* 
 * Terminal vty screenshot application
 * - Only real vty terminal (no xterm, st, ..)
 * - No colors or anything, just plain screen text
 *
 * By: prenex
 * See: http://github.com/prenex/termshot
 *
 * Licence: unlicence
 */

#include "stdio.h"
#include "ctype.h"
#include "string.h"
#include "dirent.h"
#include "stdlib.h"

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

int main(int argc, char **argv) {

	/* Currenty used strings */
	char *vcspath;
	char *vcsapath;
	BOOLE needfree;

	/* Output file string handling vars */
	char outfilename[MAX_OFILE_LEN];


	/* Handle parameters */
	if(argc == 2) {
		/* Check if we got a vty number */
		char num = argv[1][0];
		if(isdigit(num)) {
			/* Use the given vty number */

			/* Copy strings so we can modify them (they are const char* in code)*/
			vcspath = strdup(n_vcspath);
			vcsapath = strdup(n_vcsapath);
			needfree = 1; /* will need to call free! */

			/* vcs change */
			int endindex = strlen(vcspath);
			vcspath[endindex-1] = num;

			/* vcsa change */
			endindex = strlen(vcsapath);
			vcsapath[endindex-1] = num;
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

	/* TODO: Get column and row counts */

	/* Create numbered output filename: tshot_<num> */
	int num = 0;
	DIR *d;
	struct dirent *entry;
	d = opendir(".");
	if(d) {
		/* See the biggest file entry corresponding to the pattern */
		while((entry = readdir(d)) != NULL) {
			printf("%s\n", entry->d_name);
			int entrys_num = 0;
			sscanf(entry->d_name, outfile_pattern, &entrys_num);
			printf(" - entrys_num = %d\n", entrys_num);
			if(entrys_num >= num) {
				num = entrys_num + 1;
			}
		}
		closedir(d);
	}

	/* This creates the filename: tshot_<num> */
	snprintf(outfilename, MAX_OFILE_LEN, outfile_pattern, num);

	/* Loop through lines of the screen */
	/* TODO: Get screen memory data */
	/* TODO: Save screen memory data */

	/* Notify user */
	printf("Termshot saved: %s -> %s\n", vcspath, outfilename);

	/* EXIT */
	return 0;
}

/* vim: set ts=4 sw=4 noet : */
