#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/*Constants required for tree administration*/
#define DEBUG 0
#define ZERO 0
#define ONE 1
#define TWO 2
#define INITIAL 100

/*Structure types for BST and information storage
  these structures will act as not only a tree but also
  a linked list, linking factors in linear order (next) while
  also linking the tree in an ordered fashion (left, right)*/
typedef struct leaf leaf_t;
typedef struct root root_t;

typedef struct{
	char* total;
	char* suffix;
	int num;
}info_t;

struct leaf{
	info_t info;
	leaf_t* right;
	leaf_t* left;
	leaf_t* next;
	leaf_t* prefix;
};

struct root{
	int total_factors;
	int char_read;
	int (*comp)(void*, void*);
	void (*add)(void*, void*);
	leaf_t* first;
	leaf_t* last;
};

/* prototypes of tree control functions, further explanations commented
with function architecture*/
void plant_tree(root_t** root, char* initial, char* int_suffix, 
	int (*comp)(void*, void*), void (*add)(void*, void*));
leaf_t* add_branch(char* total, int num_factor, char* suffix, 
	leaf_t* previous, leaf_t* prefix);
void search_tree(root_t* root, char *next_char, leaf_t** previous);
void output(root_t *root);
void free_leaves(leaf_t *leaf);
void free_tree(root_t *root);
void mem_error(char* func_name, void* check);
char* find_last(char* string);
char* check_room(char string[], int new_len, int *max_len);
void print_bug(char* current, int is_string);
void print_bug_int(int current, char* statement);

/* Program Architectures */

/* Takes arguments for additive and comparison functions, a starting
prefix string and starting suffix and assigns them to a root struct*/
void 
plant_tree(root_t** root, char* initial, char* int_suffix, 
  int (*comp)(void*, void*), void (*add)(void*, void*)){

		*root = (root_t*)malloc(sizeof(root_t));
	    mem_error("plant_tree", *root);
	    (*root)->first = (leaf_t*)malloc(sizeof(leaf_t));
	    mem_error("plant_tree, first", (*root)->first);
	    
		(*root)->first->info.total = initial;
		(*root)->first->info.suffix = int_suffix;
		(*root)->first->info.num = 0;
		(*root)->first->left = NULL;
		(*root)->first->right = NULL;
		(*root)->first->next = NULL;
		(*root)->first->prefix = NULL;
		(*root)->last = NULL;
		(*root)->total_factors = 0;
		(*root)->char_read = 0;
		(*root)->comp = comp;
		(*root)->add = add;
		
}
 
/* Takes the current complete string as total and the ordered factor
it applies to assigns these to a new leaf while also assigning the leaf's 
pointer to the previous factor's next field */
leaf_t*
add_branch(char* total, int num_factor, char* suffix, leaf_t* previous, 
	leaf_t* prefix){
	
	leaf_t *leaf;

	leaf = (leaf_t*)malloc(sizeof(leaf_t));
	mem_error("add_branch", leaf);
	
	leaf->info.total = total;
	leaf->info.suffix = suffix;
	leaf->info.num = num_factor;
	leaf->left = NULL;
	leaf->right = NULL;
	leaf->prefix = prefix;
	leaf->next = NULL;
	previous->next = leaf;
	
	return leaf;
}

/* Takes root pointer and traverses tree to find initial string, adding to 
this string as it goes until a branch addition is necessary and therefore
creation of new compression factor*/
void 
search_tree(root_t* root, char next_char[], leaf_t** previous){
	char *current, *suffix = NULL;
	int current_size = 2, max_size = INITIAL, equality;
	leaf_t *prefix, **curr_leaf;
	
	current = (char*)malloc(sizeof(char)*INITIAL);
	
	current[ZERO] = next_char[ZERO];
	current[ONE] = '\0';
	
	curr_leaf = &(root->first);
	prefix = (*curr_leaf);
	
	while(*curr_leaf != NULL){
		equality = (root->comp)(current, (*curr_leaf)->info.total);
		if (equality < 0){
			print_bug("MOVED LEFT\n", 1);
			curr_leaf = &((*curr_leaf)->left);
		}
		else if (equality > 0){
			print_bug("MOVED RIGHT\n", 1);
			curr_leaf = &((*curr_leaf)->right);
		}
		else{
			print_bug("EQUALITY FOUND\n", 1);
			next_char[ZERO] = getchar();
			
			if (next_char[ZERO] == EOF){
				prefix = (*curr_leaf)->prefix;
				root->last = (leaf_t*)malloc(sizeof(leaf_t));
				curr_leaf = &(root->last);
				break;
			}
			else{
				current_size++;
				root->char_read++;	
				current = check_room(current, current_size, &max_size);
				(root->add)(current, next_char);
				prefix = (*curr_leaf);
				curr_leaf = &(root->first);
			}
		}
	}
	root->total_factors++;
	current = (char*)realloc(current, current_size);
	suffix = find_last(current);
	(*curr_leaf) = add_branch(current, root->total_factors, suffix, *previous,
		prefix);
	*previous = (*curr_leaf);
	print_bug((*curr_leaf)->info.total, 1);
	print_bug("\n\n", 1);
	
	
		
}

/*print factors as output*/
void
output(root_t *root){
	leaf_t *curr_leaf;
	
	curr_leaf = root->first->next;
	while (curr_leaf != NULL){
		print_bug("NEXT FOUND:  ", 1);
		print_bug(curr_leaf->info.total, 1);
		print_bug("\n", 1);
		print_bug(curr_leaf->prefix->info.total, 1);
		print_bug("\n", 1);
		printf("%s%d\n", curr_leaf->info.suffix, curr_leaf->prefix->info.num);
		curr_leaf = curr_leaf->next;
	}
	fprintf(stderr, "encode: %6d bytes input\n",root->char_read - 1);
	fprintf(stderr, "encode: %6d factors generated\n", root->total_factors);
}

/*Free all malloced memory*/
void
free_tree(root_t *root){
	if (root == NULL){
		return;
	}
	free_leaves(root->first);
	free(root);
}

/*recursively free memory stored in leaves*/
void
free_leaves(leaf_t *leaf){
	if(leaf == NULL){
		return;
	}
/*leaves are connected linearly through next, can do a linear
	recursive progression in order to free*/
	free_leaves(leaf->next);
	free(leaf);
}

/*Print memory allocation error message */
void
mem_error(char* func_name, void* check){
	if(check == NULL){
		fprintf(stderr, "Memory allocation failed in %s/n", func_name);
		exit(EXIT_FAILURE);
	}
}	

/* find the last character of a string*/
char*
find_last(char string[]){
	char *last;
	int len;
	
	len = strlen(string);
	last = &string[len-1];
	
	print_bug("LAST IS: ", 1);
	print_bug(last, 1);
	print_bug("\n", 1);
	print_bug_int(len, "LENGTH OF FACTOR");
	
	return last;
}

/*check room for character addition in array, if not call realloc */
char*
check_room(char string[], int new_len, int *max_len){
	if (new_len > *max_len){
		(*max_len)*=2;
		string = (char*)realloc(string, sizeof(char)*(*max_len));
		mem_error("check_room", string);
	}
	return string;
}

/*print debugging out put for characters and strings */
void
print_bug(char* current, int is_string){
	if(DEBUG){
		if (is_string){
			fprintf(stderr, "%s", current);
		}
		else{
			fprintf(stderr, "%c", *current);
		}
	}
}

/*print integer information and description */
void
print_bug_int(int current, char* statement){
	if(DEBUG){
		fprintf(stderr, "%d %s\n", current, statement);
	}
}
