/**
 * @file utils.c
 * @brief This file contains common methods that are used by different methods
 * @ref https://www.geeksforgeeks.org/data-structures/linked-list/
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

void insert_node(char *new_data)
{
  /**
   * @brief linked list to add new nodes
   *
   */
  struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
  struct Node *last = fat_node;
  new_node->data = (char *)malloc(sizeof(new_data));
  strcpy(new_node->data, new_data);
  new_node->next = NULL;

  if (fat_node == NULL)
  {
    fat_node = new_node;
    return;
  }
  if (strcmp(last->data, new_data) == 0)
  {
    printf("%s is already opened\n", new_data);
    return;
  }
  while (last->next != NULL)
  {
    last = last->next;
    if (strcmp(last->data, new_data) == 0)
    {
      printf("%s is already opened\n", new_data);
      return;
    }
  }
  last->next = new_node;
  return;
}

int delete_node(char *key)
{
  /**
   * @brief method to delete nodes from the linkedlist
   *
   */
  struct Node *temp = fat_node;
  struct Node *prev = NULL;

  if (temp != NULL && strcmp(temp->data, key) == 0)
  {
    fat_node = temp->next;
    free(temp);
    return 1;
  }
  while (temp != NULL && strcmp(temp->data, key) != 0)
  {
    prev = temp;
    temp = temp->next;
  }
  if (prev->next == temp->next)
    return 0;

  prev->next = temp->next;
  if (temp == NULL)
  {
    printf("File not opened\n");
  }
  else
  {
    free(temp);
  }
  return 1;
}

void print_node(void)
{
  /**
   * @brief method to print the all the data in the linked list
   *
   */
  struct Node *temp = fat_node;
  while (temp != NULL)
  {
    printf("%s\n", temp->data);
    temp = temp->next;
  }
}

tokenlist *new_tokenlist(void)
{
  tokenlist *tokens = (tokenlist *)malloc(sizeof(tokenlist));
  tokens->size = 0;
  tokens->items = (char **)malloc(sizeof(char *));
  tokens->items[0] = NULL; /* make NULL terminated */
  return tokens;
}

void add_token(tokenlist *tokens, char *item)
{
  int i = tokens->size;
  tokens->items = (char **)realloc(tokens->items, (i + 2) * sizeof(char *));
  tokens->items[i] = (char *)malloc(strlen(item) + 1);
  tokens->items[i + 1] = NULL;
  if (strcmp(item, "&") != 0)
  {
    strcpy(tokens->items[i], item);
    tokens->size += 1;
  }
}

tokenlist *get_tokens(char *input, char *delimiter)
{
  char *buf = (char *)malloc(strlen(input) + 1);
  strcpy(buf, input);

  tokenlist *tokens = new_tokenlist();

  char *tok = strtok(buf, delimiter);
  while (tok != NULL)
  {
    add_token(tokens, tok);
    tok = strtok(NULL, delimiter);
  }
  free(buf);
  return tokens;
}

void free_tokens(tokenlist *tokens)
{
  for (int i = 0; i < tokens->size; i++)
  {
    free(tokens->items[i]);
  }
  free(tokens->items);
  free(tokens);
}

void print_prompt(const char *img_name)
{
  int d = 0;
  prompt p;
  p.user = getenv("USER");
  p.machine = getenv("MACHINE");
  printf("%s@%s:", p.user, img_name);
  for (d = 0; d < env.current; d++)
  {
    printf("%s/", d == 0 ? "" : env.currentpath[d]);
  }
  printf(" > ");
}

void print_tokens(tokenlist *tokens)
{
  for (int i = 0; i < tokens->size; i++)
  {
    printf("token %d: (%s) \n", i, tokens->items[i]);
  }
}

void print_bootsector(bootsector bs)
{
  printf("Bytes per sector\t= %d\n", bs.BPB_BytsPerSec);
  printf("Sectors per cluster\t= %d\n", bs.BPB_SecPerClus);
  printf("Reserved region size\t= %d\n", bs.BPB_RsvdSecCnt);
  printf("Number of Fats\t\t= %d\n", bs.BPB_NumFATs);
  printf("Total sectors\t\t= %d\n", bs.BPB_TotSec32);
  printf("FAT size\t\t= %d\n", bs.BPB_FATSz32);
  printf("Root cluster\t\t= %d\n", bs.BPB_RootClus);
}

int get_first_sector_of_cluster(int n)
{
  int first_data_sector = bs.BPB_RsvdSecCnt + (bs.BPB_NumFATs * bs.BPB_FATSz32);
  int first_sector_of_cluster = first_data_sector + ((n - 2) * bs.BPB_SecPerClus);
  int pos = first_sector_of_cluster * bs.BPB_BytsPerSec;
  return pos;
}

void add_to_path(int current_cluster, char *name)
{

  env.current_cluster_number = current_cluster;
  strcpy((char *)env.name, name);
  strcpy(env.currentpath[env.current], name);
  env.current_cluster_path[env.current] = current_cluster;
  env.current++;
}

void print_env(void)
{
  printf("Current Cluster Number: %d\n", env.current_cluster_number);
  printf("Current Directory Name: %s\n", env.name);

  int i, j;
  for (i = 0; i < env.current; i++)
  {
    printf("currentpath: %s\n", env.currentpath[i]);
  }
  for (j = 0; j < env.current; j++)
  {
    printf("clusterpath: %d\n", env.current_cluster_path[j]);
  }
  printf("\n");
}