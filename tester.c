#include "autocomplete.h"
#include <stdio.h>
#include <string.h>

static int total_tests  = 0;
static int passed_tests = 0;

#define FAILED(MSG) fprintf(stderr,"Failed %s (%s:%d)\n",MSG,__FILE__,__LINE__)
#define FAILED_WORD(MSG,WORD,PRIOR) fprintf(stderr,"Failed %s [%s/%s] (%s:%d)\n",MSG,WORD,PRIOR,__FILE__,__LINE__)

void add_missing_dictionary_test() {
  total_tests+=1;
  if (AutoComplete_LoadDictionary("someoldfilethatshouldntexist")==1)
    {
      passed_tests+=1;
    }
  else
    {
      FAILED("trying to load a non-existent dictionary.");
    }
}

void add_small_context(const char *filename) {
  FILE *context = fopen(filename,"r");
  if (context==NULL)
    {
      printf("Couldn't find words.english.small.context in local directory. ");
      printf("\nPlease download and try again.");
      printf("\nThis will result in many tests failing (that perhaps shouldn't)!");
    }
  
  char word[100], prior[100];
  while(EOF != fscanf(context, "%s%s",word,prior))
    {
      char *w = strdup(word);
      char *p = strdup(prior);
      AutoComplete_AddContext(w,p);
    }
  fclose(context);
}

void add_small_dictionary_test() {
  total_tests+=1;
  if (AutoComplete_LoadDictionary("words.english.small")==0)
    {
      passed_tests+=1;
      add_missing_dictionary_test();  
      add_small_context("words.english.small.context");
    }
  else
    {
      FAILED("trying to load words.english.small.");
      total_tests++; // makes up for not calling add_missing_dictionary_test
      if (fopen("words.english.small","r")==NULL)
	{
	  printf("Couldn't find words.english.small in local directory. ");
	  printf("\nPlease download and try again.");
	}
    }
}

void word_lookup(const char*word, const char*prior) {
  total_tests+=1;
  int num=3;
  char *string = strdup(word);
  if (strlen(word)>=3)
    string[2] = '\0';
  
  const char *res[100] = {NULL};
  num = AutoComplete_Suggestion(string,prior,res,100);

  if(num>=1) {
    int i;
    int flag=0;
    for(i=0;i<num;i++) {
      if (res[i]==NULL) {
	FAILED_WORD("by returning the wrong value from Suggestion on ",word,prior);
	break;
      }

      if(strcmp(res[i],word)==0) {
	passed_tests+=1;
	flag=1;
	break;
      }
    }
    if (flag==0)
      {
	FAILED_WORD("to find the correct word (but found other words) when testing ",word,prior);
      }
  } else {
    FAILED_WORD("to find any word (dictionary empty) when testing ",word,prior);
  }
}

void AddWord_test(const char*word) {
  AutoComplete_AddWord(word);
  word_lookup(word,"");
}


int main(int argc, char *argv[])
{ 
  AddWord_test("xylophone");
  AddWord_test("zilch");
  AddWord_test("xilinx");
  AutoComplete_AddContext("xenia","flower");
  word_lookup("xenia","");

  total_tests++;
  if (AutoComplete_NumberOfDictionaryWords()==4)
    {
      passed_tests++;
    }
  else
    {
      FAILED("to count the correct number of words");
    }

  AddWord_test("xilinx");

  total_tests++;
  if (AutoComplete_NumberOfDictionaryWords()==4)
    {      
      passed_tests++;
    }
  else
    {
      FAILED("by incrementing when a word was already present.");
    }

  int prior_total = AutoComplete_TotalMemoryUsed();

  add_small_dictionary_test();
  word_lookup("about","all");
  word_lookup("day","next");
  word_lookup("take","us");
  word_lookup("sound","that");
  word_lookup("too","little");

  total_tests++;
  if (AutoComplete_NumberOfDictionaryWords()==191)
    {
      passed_tests++;
    }
  else
    {
      FAILED("by counting the wrong number of words");
    }

  total_tests++;
  if (AutoComplete_TotalMemoryUsed()>prior_total*10)
    {
      passed_tests++;
    }
  else
    {
      FAILED("by computing memory used incorrectly.");
    }


  prior_total = AutoComplete_TotalMemoryUsed();
  AddWord_test("xenia");
  total_tests++;
  if (AutoComplete_TotalMemoryUsed()==prior_total && prior_total > 0)
    {
      passed_tests++;
    }
  else
    {
      FAILED("by using more memory even when a word was already present." );
    }
  
  prior_total = AutoComplete_TotalMemoryUsed();
  AddWord_test("xiphoidal");
  total_tests++;
  if (AutoComplete_TotalMemoryUsed()>prior_total)
    {
      passed_tests++;
    }
  else
    {
      FAILED("by not using more memory when adding a word to dictionary." );
    }
  

  //printf("\n %2d / %2d ",passed_tests, total_tests);

  fprintf(stderr,"Passed %d of %d (%2.1lf%%) tests.\n",passed_tests,total_tests, 100.0*passed_tests/(double)total_tests);

  return 0;
}
