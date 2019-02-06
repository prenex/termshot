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
#include "time.h"
#include "stdlib.h"

#define BOOLE int

/* Default files ("current" vty) */
char *d_vcspath = "/dev/vcs";
char *d_vcsapath = "/dev/vcsa";

/* Used when telling which vty to termshot */
char *n_vcspath = "/dev/vcs1";
char *n_vcsapath = "/dev/vcsa1";

int main(int argc, char **argv) {

	/* Currenty used strings */
	char *vcspath;
	char *vcsapath;
	BOOLE needfree;

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

	/* TODO: Get screen memory data */

	/* Create timespamped output filename: termshot_DATE_TIME */

	time_t rawtime;
	struct tm *timeinfo;
	char outfilename[64];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(outfilename, 64, "./tshot_%x_%X.txt", timeinfo);

	/* TODO: Save screen memory data */

	/* Notify user */
	printf("Termshot saved: %s -> %s\n", vcspath, outfilename);

	/* Cleanup */
	if(needfree) {
		free(vcspath);
		free(vcsapath);
	}

	/* EXIT */
	return 0;
}

/* vim: set ts=4 sw=4 noet : */
