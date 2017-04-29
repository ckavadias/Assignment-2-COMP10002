/* Comment */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[]) {
	char A[5], B[4] = {'a', 'b', 'c', '\0'};
	
	A[0] = 'a';A[1] = 'b'; A[2] = 'c';
	
	printf("%d\n", strcmp(A, B));
	return 0;
}