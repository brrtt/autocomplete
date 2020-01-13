#include <stdio.h>
#include <stdlib.h>
#include <string.h>



const int size=100000;

typedef int Key;
typedef char * Item;

struct hashNode
{
  Key key;
  Item word;
  struct hashNode *next;
  struct hashNode *prior;
};

typedef struct hashNode HashNode;
HashNode **hashTable=NULL;
int hashTableSize=0;

void HTinit(int size)
{
 hashTable = malloc( size * sizeof(HashNode*) );

  // initialize - nothing is yet in the table
  int i;
  for(i=0; i<size; i++)
    hashTable[i] = NULL;
  
  hashTableSize = size;
}

Key hash(const char * word)
{
  Key hashKey=0;
  int i;
  int prime=37; 
  // int wordLength=strlen(word);
  
  for(i=0;i<2;i++)
    {
      prime*=prime;
      hashKey+=word[i]*prime;
    }
  hashKey%=size;
  Key index=hashKey;
  return index; 
}

int HTfind(Key index,const char * word)
{
  HashNode * head=hashTable[index];
  int checks=0;

  while(head)
    {
      checks++;
      if(strcmp(head->word,word)==0)
	  return 1;
      head=head->next;
    }
  return 0;
}

int HTcollide(Key index)
{
  printf("collide");
  if(hashTable[index])
      return 1;
  else
    return 0;
}
  


int main(int argc, char * argv[])
{
  char *str=malloc(100);
  char *trim=malloc(1);
  int i=0;
  int n=100;
  Key key;
  HashNode * hashTable[size]={NULL};
  // HTinit(size);
  FILE * f = fopen(argv[1],"r");
  if (f==NULL)
    {
      printf("File does not exist.\n");
      return 0;
    }
  while( NULL != fgets(str,n,f))
    {
      HashNode *head=malloc(sizeof(HashNode));
       HashNode *chain=malloc(sizeof(HashNode));
       HashNode *link=malloc(sizeof(HashNode));
       if((trim=strchr(str,'\n')) != NULL)
	 *trim='\0';
       key=hash(str);
       // printf("%d\n",key);
       // HTcollide(key);
       //  puts(str);
       if(hashTable[key]==NULL)
	  {
	   head->key=key;
	   head->word=str;
	   head->next=NULL;
	   head->prior=NULL;
	   hashTable[key]=head;
	   head=hashTable[key];
	   printf("%d: %s\n",key,head->word);
	   //  printf("%s %s\n", chain->word, link->word);
	   
	  }
       /*   if(hashTable[key])
	     {
	       chain=head;
	       link=head;
	       printf("can enter if\n");
	       while(link!=NULL && strcmp(link->word,str)<0)
		 {
		   printf("can enter loop\n");
		   chain=link;
		   link=link->next;
		 }
	       printf("exit loop");
	       link->key=key;
	       link->word=str;
	       link->next=NULL;
	       link->prior=NULL;
	       chain->next=link;
	       printf(" link = %s\n",chain->word);
	     }
      
         else (chain==link)
	     {
	      HashNode *new=malloc(sizeof(HashNode));
	      strcpy(new->word,str);
	      new->key=key;
	      new->next=head;
	      new->prior=NULL;
	      head=new;
	      printf("made chain");
	      } */
	 }
   

  
  return 0;
}
