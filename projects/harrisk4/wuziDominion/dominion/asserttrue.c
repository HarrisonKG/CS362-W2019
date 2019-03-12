#include "asserttrue.h"
#include <stdio.h>
#include <string.h>

// custom-built assert function to report success or failure of statement

int asserttrue(int result, int expected, const char* s, int* success){
	if (result != expected){
		printf("Test failed: %s: Result of %i, Expected %i\n", s, result, expected);
		*success = -1;
		return 0;
	} else {
		//printf("Test succeeded\n");
		return 1;
	}
}
