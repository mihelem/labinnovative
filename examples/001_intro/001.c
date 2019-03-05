#include <stdio.h>
#include <stdlib.h>


/* Specialised to the language libraries at hand, e.g. *
 * in C you should check if malloc succeeded :		   */

// possibly NULL_DEREFERENCE
void not_checking_if_malloc_succeeded(){
	int *c = (int *)malloc(sizeof(int));
	
	*c = 6;
	free(c);
}
// corrected version:
void checking_if_malloc_succeeded(){
	int *c = (int *)malloc(sizeof(int));

	if( c ){
		*c = 6;
		free(c);
	}
}

// Let's try to hack into how memory checking is working...
// USE_AFTER_FREE (caught)
void use_after_freed_caught(){
	int *p = (int *)malloc(sizeof(int));

	if( p ){
		int *c = p;

		free(c);
		*p = 6;
		printf("%d  %p\n", *p, p);
	}
}
// USE_AFTER_FREE (NOT caught)
void use_after_freed_uncaught(){
	int *p = (int *)malloc(sizeof(int));

	if( p ){	
		int *c = p;

		free(p);
		*c = 6;
		printf("%d  %p\n", *c, c);
	}
}

// DEAD_STORE
void dead_store(){
	int i = 12;
}

// MEMORY_LEAK
void memory_leak(unsigned int n){
	int *memory = (int *)malloc(sizeof(int)*n);

	// BUFFER_OVERRUN (-a checkers --bufferoverrun )
	if( memory )
		memory[n]=1;
}

// NULL_DEREFERENCE
void null_dereference(){
	int *null = NULL;
	*null = 3;
}
// understand that the condition cannot be satisfied
void local_addr_noalias(int* p) {
	int* q = 0;
	int x = 1;
	if (&x == p) 
		*q = 42;
}

// RESOURCE_LEAK
void resource_leak( const char *filename ){
	fopen(filename, "r");
}

// PREMATURE_NIL_TERMINATION
void premature_nil_termination( char c ){
	const char str[] = {'a', c, 'b', 'c', 'd', '\0'};
	printf( "%s\n", str );
}

// THIS DOESN'T WORK
void set_ptr(int* ptr, int val) { *ptr = val; }

int set_ptr_param_array(int buf[]) {
  set_ptr(buf, 1);
  return buf[0];
}

void FN_set_ptr_param_array_get_null_bad() {
	set_ptr_param_array(0);
}


int main(){
	premature_nil_termination('\0');
}
