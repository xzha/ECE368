#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"

Node * Node_create(long value)
{
  Node * head = Node_mymalloc(); //request for a node from the page
  head -> value = value; 
  head -> next = NULL;
 
  return head;
}

List * List_create(int gap)
{
  List * list = List_mymalloc(); //request for a list from the page
  list -> node = NULL;
  list -> next = NULL;
  
  List * current = list; 

  while (gap > 1)
    {
      current -> next = List_mymalloc(); //create a list that is k long
      current -> node = NULL;
      current = current -> next; 
      gap--;
    }

  current -> node = NULL;
  current -> next = NULL;
  return list;
}

void List_destroy(List * list)
{
  List * temp = list;
  
  while (list != NULL)
    {
      temp = list -> next;
      List_myfree(list); //return the list to the unused list
      list = temp;
    }
}

void Node_clear(Node * head)
{
  Node * temp = head;

  while(head != NULL)
    {
      temp = head -> next;
      Node_myfree(head); //return the head to the unused list
      head = temp;
    }
}

Node * Load_File(char * Filename)
{
  FILE * fh;
  Node * head = NULL;
  Node * current = NULL;
  long value;
  int i = 0;
  int g = 0;

  fh = fopen(Filename, "r");
  if (fh == NULL)
    {
      printf("ERROR\n");
      printf("INVALID INPUT FILE!\n");
      return NULL;
    }
  while (!feof(fh))
    {
      g = fscanf(fh,"%ld\n", &value);
      if (head == NULL)
	{
	  head = Node_create(value);
	  current = head;
	}
      else
	{
	  current -> next = Node_create(value);
	  current = current -> next;
	}
      i++;
    }

  Node * dummy = Node_mymalloc(); //store the amount of numbers read in from the file
  dummy -> value = i;
  dummy -> next = head; 
  
  Node * temp;
  if (i < 2)
    {
      while(dummy != NULL)
	{
	  temp = dummy->next;
	  Node_myfree(dummy);
	  dummy = temp;
	}
      printf("NOTHING TO SORT\n");
      fclose(fh);
      return NULL;
    }
  
  if (g < 0)
    {
      while(dummy != NULL)
	{
	  temp = dummy->next;
	  Node_myfree(dummy);
	  dummy = temp;
	}
      printf("FSCANF ERROR\n");
      fclose(fh);
      return NULL;
    }

  fclose(fh);
  return dummy;
}

Node * Shell_Sort(Node * fulllist)
{
  //GATHER SIZE
  int size = fulllist -> value;

  //MANAGE HEAD
  Node * head = fulllist->next;
  Node_myfree(fulllist);

  //SEQUENCE
  int k = 1;
  int p = 0;
  int gap;
  int count;
  
  //SHELL SORT VARIABLES
  Node * node = head;
  List * lhead = NULL;
  int choice = 0;

  //INITIALIZE GAP
  while (k < size)
    {
      k *= 3;
      p += 1;
    }

  k /= 3;
  p -= 1;

  while (p >= 0)
    {
      gap = k;
      count = p;

      do
	{
	  //SHELL SORT
	  lhead  = List_create(gap); //generate list
	  if (gap == 729) //manual reset: end in ascending order
	    {
	      choice = 1;
	    }
	  lhead  = List_fill(node,lhead, &choice); //fill the subarrays with values from linked list
	  node   = Node_reconstruct(node, lhead); //restring the linked list
	  List_destroy(lhead); //destroy the list
      	  gap = (gap / 3) * 2; //update gap
	  count--;
	}while(count >= 0);
      k /= 3;
      p -= 1;
    }
  
  //MEASURE FOR SIZE < 729
  if (node -> value > node -> next -> value)
    {
      node = Node_reverse(node);
    }

  return node;
}

//RECONSTRUCT THE LINKED LIST
Node * Node_reconstruct(Node * head, List * list)
{
  List * lhead = list;
  head = lhead -> node;
  Node * current = head;

  lhead -> node = lhead -> node -> next; 
  lhead = lhead -> next;

  if (lhead == NULL)
    {
      lhead = list; //reset the list to its head
    }

  while (lhead -> node != NULL)
    {
      current -> next  = lhead -> node; 
      lhead -> node = lhead -> node -> next;
      current = current -> next;
      lhead = lhead -> next;
      if (lhead == NULL)
	{
	  lhead = list;
	}
    }
  current -> next = NULL;
  
  return head; 

}

//REVERSE THE LINKED LIST
Node * Node_reverse(Node * head)
{
  Node * back = NULL;
  Node * current = head;
  Node * front;
  
  while (current != NULL)
    {
      front = current -> next;
      current -> next = back;
      back = current;
      current = front;
    }
  head = back;
  return head;
}


//FILL THE SUBARRAYS
List * List_fill(Node * node, List * list, int * count)
{
  List * lhead = list; 
  Node * head = node;
  Node * insert = NULL;

  while (head != NULL)
    {
      insert = head; //node to insert
      head = head-> next;

      if ((*count) % 2 == 0) //flip flop between insert ascend and descend
	{
	  lhead -> node = List_insertion_up(lhead->node, insert);
	}
      else
	{
	  lhead -> node = List_insertion_down(lhead->node, insert);
	}

      lhead = lhead -> next; 
      if (lhead == NULL)
	{
	  lhead = list; //reset the list to its head
	}
    }

  (*count)++;
  return list;
}

//Insert nodes into subarrays in ascending order
Node * List_insertion_up(Node * head, Node * insert)
{
  Node * front = head;
  Node * back = head;
 
  if (head == NULL) //set insert as head for empty list
    {
      insert -> next = NULL;
      return insert;
    }
  while (front != NULL && insert->value > front -> value) //find position to insert
    {
      back = front;
      front = front -> next;
    }
  if(front == head) //insert to front
    {
      insert -> next = head;
      return insert;
    }
  else if (front == NULL) //insert to end
    {
      back -> next = insert;
      insert -> next = NULL;
    }
  else //insert between
    {
      back->next = insert;
      insert -> next = front;
    }

  return head; 
}

//Insert nodes into subarrys in descending order
Node * List_insertion_down(Node * head, Node * insert)
{
  Node * front = head;
  Node * back = head;

  if (head == NULL) //set insert as head for empty list
    {
      insert -> next = NULL;
      return insert;
    }
  while (front != NULL && insert->value < front -> value) //find position to insert
    {
      back = front;
      front = front -> next;
    }
  if(front == head) //insert to front
    {
      insert -> next = head;
      return insert;
    }
  else if (front == NULL) //insert to end
    {
      back -> next = insert;
      insert -> next = NULL;
    }
  else //insert between
    {
      back->next = insert;
      insert -> next = front;
    }

  return head; 
}

int Save_File(char * Filename, Node * head)
{
  FILE * fh;
  Node * temp = NULL;
  int i = 0; //amount of numbers written to file

  fh = fopen(Filename, "w");
  if(fh == NULL)
    {
      printf("ERROR\n");
      return 0;
    }

  temp = head;

  while (temp != NULL)
    {
      fprintf(fh, "%ld\n", temp->value);
      temp = temp->next;
      i++;
    }
  fclose(fh);
  return i;
} 
