// Implement a winelist program in C that has following features:
// The user can add wines to the winelist
// The user can remove any wine from the list by checking it as drinked
// The user can print out the winelist to the screen
// The winelist must not be static.

//  Created by Petri Ollonen on 02/11/2017.
//  Copyright © 2017 Petri Ollonen. All rights reserved.

#include "wines.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  char buf[size];
  char userChoice;
  char *fName = "winelist.txt";
  int success = 0;
  size_t dataArraySize = 0;
  FILE *f;
  Node *head = NULL;
  Node *ptr = NULL;

  printf("\nThis program handles winelist.\n");
  printf("The user may add new wines or select them as drinked.\n\n");

  if (fileCheck(fName)) {
    f = fopen("winelist.txt", "r");
    while (fgets(buf, size, f) != NULL) {
      dataArraySize = strlen(buf);
      success = insertNewNode(&head, buf, dataArraySize);
      if (success == FALSE) {
        printf("Something went wrong - new wine was not added due to memory "
               "allocation failure");
        deleteList(&head);
        fclose(f);

        return -1;
      }
    }
    fclose(f);
  }

  do {
    printf("\nMenu - choise of actions:\n");
    printf("\t\'a\' - Add a wine to the winelist\n");
    printf("\t\'d\' - Mark a wine as drinked\n");
    printf("\t\'p\' - Print the winelist\n");
    printf("\t\'q\' - Exit the program\n");

    printf(
        "\nPlease give your choice of action (\'a\', \'d\', \'p\' or \'q\'): ");
    userChoice = getchar();
    while (getchar() != '\n')
      ;

    switch (userChoice) {
    case 'a':
      if (!addNewWine(&head)) {
        return -1;
      }
      break;
    case 'd':
      deleteWines(&head);
      break;
    case 'p':
      printWinelist(head);
      break;
    case 'q':
      deleteList(&head);
      break;
    default:
      printf("\nNot a valid choice of action!\n");
    }
  } while (userChoice != 'q');

  printf("Programn closed - Bye!\n");
}

int fileCheck(const char *fileName) {
  int file;

  if (!access(fileName, F_OK))
    file = TRUE;
  else {
    printf("\nThe wine list is empty\n");
    file = FALSE;
  }

  return file;
}

static size_t getLine(char *prmpt, char *buff, size_t sz) {
  int ch;

  if (prmpt != NULL) {
    printf("%s", prmpt);
    fflush(stdout);
  }
  if (fgets(buff, sz, stdin) == NULL)
    return strlen(buff);

  if (buff[strlen(buff) - 1] != '\n') {
    while (((ch = getchar()) != '\n') && (ch != EOF))
      buff[strlen(buff) - 1] = '\n';
    return strlen(buff);
  }
  buff[strlen(buff) - 1] = '\n';

  return strlen(buff);
}

int insertNewNode(Node **head, char *buffer, size_t dataSize) {
  char *ptr = buffer;

  Node *newNode = (Node *)malloc(sizeof(Node));
  if (!newNode)
    return FALSE;

  newNode->data = (char *)malloc(sizeof(char) * dataSize + 1);
  if (!(newNode->data))
    return FALSE;

  newNode->status = IN_STOCK;
  strcpy(newNode->data, ptr);
  newNode->next = *head;
  *head = newNode;

  return TRUE;
}

int markWinesDrinked(Node *head, char *buffer) {
  Node *ptr = head;
  Node *prev = NULL;

  if (ptr != NULL && (strcmp(ptr->data, buffer) == 0)) {
    ptr->status = DRINKED;
    return TRUE;
  }

  while (ptr != NULL && !(strcmp(ptr->data, buffer) == 0)) {
    prev = ptr;
    ptr = ptr->next;
  }

  if (ptr == NULL)
    return FALSE;

  ptr->status = DRINKED;

  return TRUE;
}

void deleteNodes(Node **head) {
  Node *ptr = *head;
  Node *prev = NULL;

  while (ptr != NULL && ptr->status == DRINKED) {
    *head = ptr->next;
    free(ptr->data);
    free(ptr);
    ptr = *head;
  }

  while (ptr != NULL) {
    while (ptr != NULL && ptr->status != DRINKED) {
      prev = ptr;
      ptr = ptr->next;
    }

    if (ptr == NULL)
      return;

    prev->next = ptr->next;

    free(ptr->data);
    free(ptr);

    ptr = prev->next;
  }
}
void deleteList(Node **head) {
  Node *ptr = *head;
  Node *tmp;

  while (ptr != NULL) {
    tmp = ptr->next;
    free(ptr->data);
    free(ptr);
    ptr = tmp;
  }
  head = NULL;
}

void printWinelist(Node *head) {
  Node *ptr = head;

  if (head == NULL) {
    printf("The wine list is empty\n");
  } else {
    printf("\nWinelist:\n");
    while (ptr != NULL) {
      printf("%s", ptr->data);
      ptr = ptr->next;
    }
  }
}

void printDrinkedWines(Node *head) {
  Node *ptr = head;

  printf("\nListed wines removed from the winelist:\n");
  while (ptr != NULL) {
    if (ptr->status == DRINKED)
      printf("%s", ptr->data);
    ptr = ptr->next;
  }
}

int addNewWine(Node **head) {
  char buf[size];
  int success = 0;
  size_t dataArraySize;
  FILE *f;

  printf("\nAdd a new wine to the winelist\n(Max len 32 characters):\n");
  dataArraySize = getLine("\nEnter new wine: ", buf, size);
  printf("BUF_LEN: %lu", strlen(buf));
  success = insertNewNode(head, buf, dataArraySize);
  if (success == FALSE) {
    printf("Something went wrong - new wine was not added due to memory "
           "allocation failure");
    deleteList(head);
    return FALSE;
  }
  f = fopen("winelist.txt", "a");
  fprintf(f, "%s", buf);
  fclose(f);

  return TRUE;
}

void deleteWines(Node **head) {
  char markWines = '\0';
  char buf[size];
  int drinkedWines;
  size_t dataArraySize;
  FILE *f;
  Node *ptr = NULL;

  printWinelist(*head);
  printf("\nMark wines as drinked (to be removed from the list):\n");
  do {
    if ((markWines == 'y') || (markWines == 'Y')) {
      printf("\nMark next wine as drinked:\n");
    }
    dataArraySize = getLine("\nEnter drinked wine: ", buf, size);
    if (!(drinkedWines = markWinesDrinked(*head, buf)))
      printf("The wine was not found from the list!\n");
    // actually any other than 'y' terminates the loop
    printf("Do you wish to mark more wines as drinked (y/n)?\n");
    markWines = getchar();
    while (getchar() != '\n')
      ;
  } while ((markWines == 'y') || (markWines == 'Y'));

  printDrinkedWines(*head);
  deleteNodes(head);
  ptr = *head;
  f = fopen("winelist.txt", "w");
  while (ptr != NULL) {
    fprintf(f, "%s", ptr->data);
    ptr = ptr->next;
  }
  fclose(f);
}
