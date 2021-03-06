#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"

int main (int argc, char ** argv)
{
  //VALIDATE INPUT
  if (argc != 3)
    {
      printf("ERROR\n");
      printf("FORMAT: ./proj2 <input> <output>\n");
      return EXIT_FAILURE;
    }

  //INITIALIZE CLOCK
  clock_t input_clock_begin;
  clock_t input_clock_end;
  clock_t input_clock;
  
  clock_t output_clock_begin;
  clock_t output_clock_end;
  clock_t output_clock;

  clock_t sort_clock_begin;
  clock_t sort_clock_end;
  clock_t sort_clock;

  //GET INPUT
  input_clock_begin = clock();
  Node * head = NULL; //list without dummy node
  Node * fulllist = NULL; //list with dummy node
  fulllist = Load_File(argv[1]);
  if (fulllist == NULL)
    {
      destroyall();
      printf("ERROR LOADING INTO LINKED LIST\n");
      return EXIT_FAILURE;
    }
  input_clock_end = clock();

  //SORT
  sort_clock_begin = clock();
  head = Shell_Sort(fulllist); //shell sort
  sort_clock_end = clock();

  //SAVE FILE
  Node * temp;
  output_clock_begin = clock();
  int save_node = 0;
  save_node = Save_File(argv[2], head);
  if (save_node == 0)
    {
      while(head != NULL)
	{
	  temp = head->next;
	  Node_myfree(head);
	  head = temp;
	}
      printf("ERROR SAVING\n");
      //FREE
      destroyall();
      return EXIT_FAILURE;
    }
  output_clock_end = clock();

  //CLOCK
  input_clock = (input_clock_end - input_clock_begin)/ CLOCKS_PER_SEC;
  output_clock =(output_clock_end - output_clock_begin)/ CLOCKS_PER_SEC;
  sort_clock = (sort_clock_end - sort_clock_begin)/ CLOCKS_PER_SEC;

  //SCREEN DUMP
  printf("\nI/O time:  %le\n", (float)(input_clock + output_clock));
  printf("Sorting time:  %le\n\n", (float)(sort_clock));

  Node_clear(head);
  destroyall();
  return EXIT_SUCCESS;
}
