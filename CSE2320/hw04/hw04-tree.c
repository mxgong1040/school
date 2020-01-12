/*Michelle Gong
UTA ID 1001671203
CSE 2320, Homework 4, Program 2*/

/********************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define LINE_SIZE 20

struct Node
{
    char *element; /*data*/
    struct Node *left;
    struct Node *right;
};

struct Node* childNode(char *element)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));

    node->element = element;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/********************/

static int strComp(const void* a, const void* b);
void sort(char* arr[], int n);
struct Node* balance(char* arr[], int first, int last);
void preOrder(struct Node* node);
void inOrder(struct Node* node);
void postOrder(struct Node* node);

int main()
{
    int i;
    int arrIndex;
    int counter;
    char **nameArray = malloc(arrIndex*sizeof(nameArray));
    char line[LINE_SIZE];
    struct Node *root;
    FILE *fp;

    i = 0;
    arrIndex = 2; /*starting size*/
    counter = 0; /*track number of names*/


    /*****FILE INPUT*****/

    fp = fopen("data-tree.txt","r");

    while(fgets(line,LINE_SIZE,fp))
    {
        char *noNewline;
        int len;
        noNewline = strtok(line,"\n"); /*take out newLines*/
        len = strlen(noNewline);

        if(i<(arrIndex-1))
           {
               nameArray[i] = malloc(len*sizeof(char));
           }
        else
        {
            nameArray = realloc(nameArray, (arrIndex*2)*sizeof(*nameArray));
            nameArray[i] = malloc(strlen(noNewline)*sizeof(char));
            printf("reallocating to %d addresses\n", arrIndex*2);
            arrIndex = arrIndex*2;
        }
        strcpy(nameArray[i], noNewline);
        counter++;
        /*printf("%s\n", nameArray[i]);*/
        i++;
    }
    fclose(fp);

    /**********/
    /*Value check
    for(i=0;i<counter;i++)
    {
        printf("%d. %s\n", i, nameArray[i]);
    }*/
    /**********/

    /*sort with qsort()*/
    sort(nameArray, counter);

    /**********/
    /*Sorted check
    for(i=0;i<counter;i++)
    {
        printf("%d. %s\n", i, nameArray[i]);
    }*/


    /*****BST FUNCTIONS*****/
    root = balance(nameArray, 0, counter-1);

    printf("\n");
    printf(" preorder:");
    preOrder(root);
    printf(" \n\n");

    printf("  inorder:");
    inOrder(root);
    printf(" \n\n");

    printf("postorder:");
    postOrder(root);
    printf(" \n");

    return 0;
}

/*needed for qsort()*/
static int strComp(const void* a, const void* b)
{
    return strcmp(*(const char**)a, *(const char**)b);
}

void sort(char* arr[], int n)
{
    qsort(arr, n, sizeof(const char*), strComp);
}

/*given function for BST*/
struct Node* balance(char* arr[], int first, int last)
{
    int mid;
    struct Node *root;

    if(first<=last)
    {
        mid = first + (last-first)/2;
        root = childNode(arr[mid]);
        root->left = balance(arr, first, mid-1);
        root->right = balance(arr, mid+1, last);

        return root;
    }
    else
    {
        return NULL;
    }
}

void preOrder(struct Node* node)
{
    if(node==NULL)
    {
        return;
    }
    else
    {
        printf(" %s", node->element);
        preOrder(node->left);
        preOrder(node->right);
    }
}

void inOrder(struct Node* node)
{
    if(node==NULL)
    {
        return;
    }
    else
    {
        inOrder(node->left);
        printf(" %s", node->element);
        inOrder(node->right);
    }
}

void postOrder(struct Node* node)
{
    if(node==NULL)
    {
        return;
    }
    else
    {
        postOrder(node->left);
        postOrder(node->right);
        printf(" %s", node->element);
    }
}
