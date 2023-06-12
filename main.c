#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "heap.h"
#include "custom_unistd.h"
#include <time.h>        

#define PAGE_SIZE 4096

int main(void) {
	heap_setup();

    int* tab=heap_malloc(10);
    if(tab==NULL)
    {
        printf("Bad alloc\n");
        return 8;
    }
    for(int i=0;i<9;i++)
    {
        *(tab+i)=i;
    }
    for(int i=0;i<9;i++)
    {
        printf("%d\n",*(tab+i));
    }
    heap_clean();
  	return 0;
}