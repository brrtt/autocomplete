#include <stdio.h>
#include <stdlib.h>


int main(int argc, char * argv[])
{
  FILE * f = fopen(argv[1],"r");
  if (f==NULL)
    {
      printf("File does not exist.\n");
      return 0;
    }
  
  char * array = malloc(1000);
  int size = 1000;
  int i=0;

  while( EOF != fscanf(f, "%c", &array[i++]) )
    {
      
      if (i >= size)
	{
	  // run out of space
	  char * new_array = malloc( 2 * size );
	  int k;
	  for(k=0; k<size; k++)
	    new_array[k] = array[k];

	  size = 2 * size;
	  free(array);
	  array = new_array;

	  printf("We created a new array of size=%d.\n", size);
	}
      

    }
  
int j=0;
for(j=0; j<i; j++)
   printf("%c",array[j]);

  return 0;
}
