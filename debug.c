#include "autocomplete.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print(char *w, const char **a, int size) {
  int i;
  printf("AutoComplete %s:\n",w);
  for(i=0; i<size; i++)
    printf("\t%d: %s\n",i,a[i]);
}

void add_context(const char *filename) {
  FILE *context = fopen(filename,"r");
  if (context==NULL)
    {
      printf("Couldn't find %s in local directory. ",filename);
      printf("\nPlease download and try again.");
      return;
    }
  
  char word[100], prior[100];
  while(EOF != fscanf(context, "%99s %99s",word,prior))
    {
      char *w = strdup(word);
      char *p = strdup(prior);			
      AutoComplete_AddContext(w,p);
    }
  fclose(context);
}

int main(int argc, char *argv[])
{
  if (argc < 2) 
    {
      printf("Usage: ./debug dictionary <# of suggestions>\n\n");
      printf("Example:\n");
      printf("         ./debug words.english.small 3\n\n");

      printf("If you want to test your own dictionary, pass the filename\n"
	     "as the first argument to this program.\n");
      printf("Using words.english.small dictionary.\n\n");
      AutoComplete_LoadDictionary("words.english.small");
      add_context("words.english.small.context");
    }
  else 
    {
      AutoComplete_LoadDictionary(argv[1]);
      char context[1000];
      sprintf(context,"%s.context",argv[1]);
      add_context(context);
    }

  int number=3;

  if (argc>=3) {
    sscanf(argv[2],"%d",&number);
    printf("Using %d suggestions.\n\n",number);
  }
  
  printf("Words=%d Memory=%.2lfMB\n",AutoComplete_NumberOfDictionaryWords(),
	 ((double)AutoComplete_TotalMemoryUsed())/1000000);

  
  printf("Enter a few letters to complete: (Ctrl-C to quit)\n");
  char word[100];
  int tmp=0;
  const char **a = malloc(sizeof(char*)*number);

  while(scanf("%20s",word)==1) {
    tmp = AutoComplete_Suggestion(word,"",a,number);
    if (tmp>0)
      print(word,a,tmp);    
  }

  return 0;
}
