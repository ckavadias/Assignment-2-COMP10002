/* Assignment 2, compression sequence calling function,
   COMP10002, designed by Constantinos Kavadias, 664790
   ckavadias@student.unimelb.edu.au, October 2015*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compressiontrees.h"

/*main function protorypes*/
int string_comp(void *compare, void *current);
void add_string(void *initial, void *next);

int
main(int argc, char *argv[]) {
	root_t *root = NULL;
	leaf_t **previous = NULL;
	char next_char[TWO];
	int counter = 0;
	
	plant_tree(&root, "", "", string_comp, add_string);
	previous = (leaf_t**)malloc(sizeof(leaf_t*));
	*previous = root->first;
	next_char[ZERO] = getchar();
	next_char[ONE] = '\0';
	root->char_read++;
	
	
	while(next_char[ZERO] != EOF){
		counter++;
		print_bug("CYCLE START\n", 1);
		print_bug(next_char, 0);
		print_bug("\n", 0);
		search_tree(root, next_char, previous);
		print_bug("CYCLE FINISH ", 1);
		print_bug_int(counter,"");
		next_char[ZERO] = getchar();
		root->char_read++;
	}
	
	output(root);
	free_tree(root);
	
	return 0;
}

/*harness for using strcomp in comparison routine for tree*/
int 
string_comp(void *compare, void *current){
	char* str1, *str2;
	
	str1 = (char*)compare;
	str2 = (char*)current;
	
	return strcmp(str1, str2);
}

/*harness for string concatenation during tree operations */
void 
add_string(void *initial, void *next){
	char* start, *addition;
	
	start = (char*)initial;
	addition = (char*)next;
	
	strcat(start, addition);
}