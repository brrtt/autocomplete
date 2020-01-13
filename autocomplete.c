//J Barrett Chesebrough
//ECE 209-051
//Dr. J Tuck

//Autocomplete.c
//This autocomplete program utilizes loaded dictionaries and mapped contexts
//to predict likely words given a partial word input.
//Pairs best with EOS linux

#include "autocomplete.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct word_list
{
  char word[100];
  struct word_list *next;
  struct word_list *prior;
};

typedef struct word_list* wordList;
wordList head=NULL;
int wordcount=0;     //number of words read by AutoComplete_AddWord
int contextcount=0; //number of words read by AutoComplete_Addcontext


// Add the words in file specified by filename to the
// auto-complete engine. This may be called multiple
// times with different filenames.  The words in the new
// file are appended to the existing list.  Duplicates are not 
// allowed.
// Argument: filename - file with words that need to be added
//                      to the table
// Return value: 0: success
//               1: bad file name
int AutoComplete_LoadDictionary(const char *filename)
{
   char* word=malloc(sizeof(char)*100);
  FILE* f;
  
  f=fopen(filename,"r");

  //check filename
  if(f==NULL)
    {
      printf("File not found\n");
      return 1;
    }

  //add words to autocomplete
  else
      while(fscanf(f,"%s",word)!=EOF)
	  AutoComplete_AddWord(word);
  
  fclose(f);
  printf("dictionary added\n");
  return 0;   
}

// Add a word to the autocomplete dictionary.  If the word is already 
// present, it simply does nothing.
void AutoComplete_AddWord(const char *word)
{
  wordList tmp=NULL;
  wordList chainLink=NULL;
  wordList new=(wordList)malloc(sizeof(struct word_list));

  strcpy(new->word,word);
  tmp=head; 
  chainLink=head;
  
  //if first word read
  if(head==NULL)
    {
      head=new;
      new->next=NULL;
      new->prior=NULL;
    }
  else
    {
      while(tmp!=NULL && strcmp(tmp->word,word)<0) 
	{
	  chainLink=tmp;
	  tmp=tmp->next;
	}
      
      //does not add words that are already present
      if(tmp!=NULL && strcmp(tmp->word,word)==0)
	{
	  free(new);
	  return;
	}

      //if word is at the front of the list
      if(chainLink==tmp)
	{
	  new->next=head;
	  head=new;
	  new->prior=NULL;
	}

      //if word the word is somewhere in the list
      else
	{
	  new->next=tmp;
	  chainLink->next=new;
	  new->prior=NULL;
	}
    }
  
  tmp=head; //reset tmp for later calls
  wordcount+=1;
}

// Add context for a word to the autocomplete dictionary.  If the word
// is not already present, then add it. If the word is present, add 
// prior to its context. If prior is already present, do not add it a 
// second time.
void AutoComplete_AddContext(const char *word, const char * prior)
{
  wordList tmp=NULL;
  wordList linkChain=NULL;
  wordList new=(wordList)malloc(sizeof(struct word_list));
  
  strcpy(new->word,word);
  tmp=head;
  new->next=NULL;

  while(tmp!=NULL && strcmp(tmp->word,word)!=0)
      tmp = tmp->next;

  //check if word exists
  if(tmp==NULL)
    {
      AutoComplete_AddWord(word);
      tmp=head;
      while(tmp!=NULL && strcmp(tmp->word,word)!=0)
	tmp = tmp->next;
    }
  
  linkChain=tmp;
  while(linkChain->prior!=NULL)
    {
    linkChain=linkChain->prior;
    linkChain->next=NULL;
    }
  
  linkChain->prior=new;
  new->prior=NULL;
  contextcount++; 
}


// Return an integer specifying the total amount of memory
// used by the auto-completer.
// Arguments: None
// Return value: integer that specifies in bytes the amount
//               of memory used, including global/static variables.
int AutoComplete_TotalMemoryUsed()
{
  int memory=0;
  memory+=wordcount*sizeof(struct word_list);
  memory+=contextcount*sizeof(struct word_list);
  printf("memory calculated\n");
  return memory;
}

// Return an integer specifying the total number of words
// added to the auto-completer.
int AutoComplete_NumberOfDictionaryWords()
{
  printf("wordcount: %d \n",wordcount);
  return wordcount;
}

// Return an integer specifying the total number of words
// used for semantic context.
int AutoComplete_NumberOfContextWords()
{
  printf("contextcount: %d \n",contextcount); 
  return contextcount;
}

// For a given partial word, return no more than size possible
// completions.
// Arguments: prompt - the beginning of a word to complete
//            prior -  a string with the word that came before it; it 
//                     may be NULL, and if so, no context is needed 
//                     for a match. Otherwise, the prior must be part 
//                     of the context of all returned words.
//            completions - an array to store the completions inside
//            size_of_array - size of the completions array
//
// Return value: number of completions returned in the array; 0 means
//               nothing was returned
//
int AutoComplete_Suggestion(const char *partial_word, const char *prior, const char *completions[], int size_of_array){
  int matches = 0;
  int n = 0;
  wordList tmp = NULL;
  wordList linkChain = NULL;

  n=strlen(partial_word);
  
  tmp=head;

  //matches with word from context
  if(prior != NULL)
    {
      while(matches < size_of_array && tmp != NULL)
	{
	  linkChain = tmp;
	  if(strncmp(tmp->word,partial_word,n) == 0)
	    {
	      while(tmp->prior != NULL)
		{
		  if(strcmp(tmp->prior->word,prior) == 0)
		    {
		      completions[matches] = tmp->word;
		      matches+=1;
		    }
		  tmp = tmp->prior;
		}
	    }
	  tmp=linkChain->next;
	}
    }
// matches with word from dictionary
  tmp=head;
  if(matches==0)
    {
      while(matches < size_of_array && tmp != NULL)
	{
	  linkChain=tmp;
	  if(strncmp(tmp->word,partial_word,n)==0)
	    {
	      completions[matches]=tmp->word;
	      matches+=1;
	    }
	  tmp=linkChain->next;
	}
    }
  printf("matches = %d\n",matches); 
  return matches;
}
