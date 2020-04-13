// Implement a winelist program in C that has following features:
// The user can add wines to the winelist
// The user can remove any wine from the list by checking it as drinked
// The user can print out the winelist to the screen
// The winelist must not be static.

//  Created by Petri Ollonen on 02/11/2017.
//  Copyright © 2017 Petri Ollonen. All rights reserved.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define FALSE 0
#define TRUE  1
#define SIZE  32+1
#define IN_STOCK 1
#define DRINKED 0

typedef struct Node
{
    int status;
    char *data;
    struct Node *next;
} Node;

int fileCheck(const char *fileName);
size_t stringInput(char *buffer);
int insertNewNode(Node **head, char *buffer, size_t dataSize);
int markWinesDrinked(Node *head, char *wine);
void deleteNodes(Node **head);
void deleteList(Node *head);
void printWinelist(Node *head);
void printDrinkedWines(Node *head);

int main(void)
{
    char buf[SIZE];
    char userChoice;
    char *fName = "winelist.txt";
    char markWines = '\0';

    int success = 0;
    int drinkedWines;
    
    size_t dataArraySize = 0;
    
    Node *head = NULL;
    Node *ptr = NULL; 
    
    FILE *f;
    
    printf("\nThis program handles winelist.\n");
    printf("The user may add new wines or select them as drinked.\n"); 
    printf("\nValid user choices of action :\n");
    printf("\t\'a\' - Add a wine to the winelist\n");
    printf("\t\'d\' - Mark a wine as drinked\n");
    printf("\t\'p\' - Print the winelist\n");
    printf("\t\'e\' - Exit the program\n");
    
    if(fileCheck(fName))
    {
        f = fopen("winelist.txt", "r");
        printf("\nUp to date winelist:\n");
        while(fgets(buf, SIZE, f) != NULL)
        {
            printf("%s", buf);
            dataArraySize = strlen(buf);
            success = insertNewNode(&head, buf, dataArraySize);
            if(success == FALSE)
            {
                printf("Something went wrong - new wine was not added due to memory allocation failure");
                deleteList(head);
                fclose(f);

                return -1;
            }
        }
        fclose(f);
    }

    for(;;)
    {
        printf("\nPlease give your choice of action (\'a\', \'d\', \'p\' or \'e\'): ");
        userChoice = getchar();
        while (getchar() != '\n')
            ;
        
        switch(userChoice)
        {
            case 'a':
                printf("\nAdd a new wine to the winelist:\n");
                dataArraySize = stringInput(buf);
                success = insertNewNode(&head, buf, dataArraySize);
                if(success == FALSE)
                {
                    printf("Something went wrong - new wine was not added due to memory allocation failure");
                    deleteList(head);
                    return -1;
                }
                f = fopen("winelist.txt", "a");
                fprintf(f, "%s", buf);
                fclose(f);
                break;
            case 'd':
                printWinelist(head);
                printf("\nMark wines as drinked (to be removed from the list):\n");
                do
                {
                    if((markWines == 'y') || (markWines == 'Y'))
                        printf("\nMark next wine as drinked:\n");
                    dataArraySize = stringInput(buf);
                    if(!(drinkedWines = markWinesDrinked(head, buf)))
                        printf("The wine was not found from the list!\n");
                    //actually any other than 'y' terminates the loop
                    printf("Do you wish to mark more wines as drinked (y/n)?\n");
                    markWines = getchar();
                    while (getchar() != '\n')
                        ;
                } while((markWines == 'y') || (markWines == 'Y')); 
                
                printDrinkedWines(head);
                deleteNodes(&head);
                ptr = head;
                f = fopen("winelist.txt", "w");
                while (ptr != NULL)
                {
                    fprintf(f, "%s", ptr->data);
                    ptr = ptr->next;
                }
                fclose(f);
                break;
            case 'p':
                if(head == NULL)
                    printf("The wine list is empty\n");
                else
                    printWinelist(head);
                break;
            case 'e':
                deleteList(head);
                return 0;
            default:
                printf("\nNot a valid choice of action!\n");
                while (getchar() != '\n')
                    ;
                break;
        }
    }
}

int fileCheck(const char *fileName)
{
    int file;

    if(!access(fileName, F_OK ))
        file = TRUE;
    else
    {
        printf("\nThe wine list is empty\n");
        file = FALSE;
    }

    return file;
}

size_t stringInput(char *buffer)
{
    size_t arraySize;

    fgets(buffer, SIZE, stdin);
    while (getchar() != '\n')
        ;
    arraySize = strlen(buffer);
    if(arraySize == SIZE-1)
        buffer[SIZE-2] = '\n';

    return arraySize;
}

int insertNewNode(Node **head, char *buffer, size_t dataSize)
{
    char *ptr = buffer;

    Node *newNode = (Node *) malloc(sizeof(Node));
    if(!newNode)
        return FALSE;
    
    newNode->data = (char *) malloc(sizeof(char) * dataSize + 1);
    if(!(newNode->data))
        return FALSE;
    
    newNode->status = IN_STOCK;
    strcpy(newNode->data, ptr);
    newNode->next = *head;
    *head = newNode;
    
    return TRUE;
}

int markWinesDrinked(Node *head, char *buffer)
{
    Node *ptr = head;
    Node *prev = NULL;

    if (ptr != NULL && (strcmp(ptr->data, buffer) == 0))
    {
        ptr->status = DRINKED;
        return TRUE;
    }

    while (ptr != NULL && !(strcmp(ptr->data, buffer) == 0))
    {
        prev = ptr;
        ptr = ptr->next;
    }

    if (ptr == NULL)
        return FALSE;
    
    ptr->status = DRINKED;
    
    return TRUE;
}

void deleteNodes(Node **head)
{
    Node *ptr = *head;
    Node *prev = NULL;
    
    while (ptr != NULL && ptr->status == DRINKED) 
    { 
        *head = ptr->next;
        free(ptr->data);
        free(ptr);
        ptr = *head;
    } 

    while (ptr != NULL) 
    { 
        while (ptr != NULL && ptr->status != DRINKED) 
        { 
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
void deleteList(Node *head)
{
    Node *ptr = head;
    Node *tmp;

    while (ptr != NULL)
    {
        tmp = ptr->next;
        free(ptr);
        ptr = tmp;
    }
    head = NULL;
}

void printWinelist(Node *head)
{
    Node *ptr = head;

    printf("\nUp to date winelist:\n");
    while (ptr != NULL)
    {
        printf("%s", ptr->data);
        ptr = ptr->next;
    }
}

void printDrinkedWines(Node *head)
{
    Node *ptr = head;

    printf("\nListed wines removed from the winelist:\n");
    while (ptr != NULL)
    {
        if(ptr->status == DRINKED)
            printf("%s", ptr->data);
        ptr = ptr->next;
    }
}