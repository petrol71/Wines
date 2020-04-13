#include <stdio.h>

#define FALSE 0
#define TRUE 1
#define IN_STOCK 1
#define DRINKED 0

typedef struct Node {
  int status;
  char *data;
  struct Node *next;
} Node;

int fileCheck(const char *fileName);
int insertNewNode(Node **head, char *buffer, size_t dataSize);
int markWinesDrinked(Node *head, char *wine);
void deleteNodes(Node **head);
void deleteList(Node **head);
void printWinelist(Node *head);
void printDrinkedWines(Node *head);
int addNewWine(Node **head);
void deleteWines(Node **head);
// static int getLine (char *prmpt, char *buff, size_t sz);
static size_t getLine(char *prmpt, char *buff, size_t sz);

size_t size = sizeof(char) * 34;