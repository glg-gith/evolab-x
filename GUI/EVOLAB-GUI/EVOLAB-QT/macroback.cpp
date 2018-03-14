///---------------------------------------------------------------------
/*

When the program runs, there is no symbol table [1].

If you want to do this - and /why/ do you want to do this? - you
can create your own table:
*/

static struct { int value; char *name; } backwards =
{
	{ A, "A" },
	{ B, "B" },
	// and so on, for all the macros you want to do it to
	{ 0, 0 }
};

/*
Now you can look up the value and find the corresponding string.

[1] Not one that can be portably accessed by an ANSI C program.
In any case, usually the #defines have gone before we have
what's usually called a "symbol table".

*/
///---------------------------------------------------------------------

/* contains non-portable code */
/* compilation (if called macroback.c)
* gcc -O -rdynamic -ansi -pedantic -Wall macroback.c -ldl -o
macroback
*/

#include <stdio.h>
#define concat(a,b) a##b
#define my_define(name,value) char *concat(const_id_,value)=#name; \
enum { name = value }
/* values should be non-negative integer constants */

my_define(A,12);
my_define(B,11);

const char **symbol_access(const char *name);

int main()
{
	char array[A];
	int i;
	printf("A=%d works\n",sizeof array);
	printf("B=%d works\n",B);
	while(scanf("%d",&i)==1) {
		char name[16];
		const char **found;
		sprintf(name,"const_id_%d",i);
		if((found=symbol_access(name))) printf("%s=%d\n",*found,i);
		else printf("Value %i not found\n",i);
	}
	return 0;
}

/* OS dependent part, should work under GNU linux */

#include <dlfcn.h>
const char **symbol_access(const char *symbol_name)
{
	static void * handle;
	if(handle || (handle=dlopen(NULL,RTLD_LAZY)))
		return dlsym(handle,symbol_name);
	fprintf(stderr,"Could not access symbol table: %s\n",dlerror());
	return NULL;
}
