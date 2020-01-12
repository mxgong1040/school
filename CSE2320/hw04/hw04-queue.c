/*Michelle Gong
UTA ID 1001671203
CSE 2320, Homework 4, Program 1*/

/********************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define LINE_SIZE 50

struct Data
{
    char *element; /*data*/
    struct Data *next; /*address*/
}*head;

struct Data *tail;

void init();
void display();
void add(char *element);
int rem(char *element);
int rem2(char *element);
void flush(char *element);
int search(char *key);
void error(char *e);

/********************/

int main(int argc, char *argv[])
{
    char line[LINE_SIZE];
    /*Testing
    FILE *fp;
    /*fp=fopen("data-queue.csv", "r");*/

    init();

    /*Since data is given to start with add/remove/flush,
    safe to just read line[0] this time around*/
    while(fgets(line,LINE_SIZE,stdin))
    {
        if(line[0]=='a')
        {
            char *toss; /*parse out the "add,"*/
            char *tempName; /*actual name after comma*/
            int len;
            char name[50];
            int i, s;

            toss = strtok(line,","); /*the "add,"*/
            tempName = strtok(NULL,"\n"); /*get name without newline*/

            len = strlen(tempName);

            /*for(i=0;i<len;i++)
            {
                name[i]=tempName[i];
            }*/
            add(tempName);

        }
        else if(line[0]=='r')
        {
            rem(line);
        }
        else if(line[0]=='f')
        {
            flush(line);
        }
    }
    printf("flushing remaining names\n");
    while(tail!=NULL)
    {
        rem2("1");/*rem2() for final flush without printf*/
    }

    /*fclose(fp);*/
}


/*initialise linked list*/
void init()
{
    tail = NULL;
}


void display()
{
    struct Data *ptr, *ptr2;

    /*print nothing if empty*/
    if(!tail)
    {
        /*printf("\0");*/
        return;
    }

    head = ptr = tail->next;
    ptr2 = ptr;

    do{
        ptr2 = ptr->next;
        /*loop for last-object formatting*/
        if(ptr2 == head)
        {
            printf(" %s", ptr->element);
            break;
        }
        printf (" %s,", ptr->element);
        ptr = ptr->next;
    }while (ptr != head);
}


void add(char *element)
{
    struct Data *ptr;
    char *cp;
    int repeat;

    if((ptr=(struct Data *)malloc(sizeof(struct Data)))==NULL)
    {
        error("Error, cannot allocate memory.\n");
    }
    if((cp=(char*)malloc(strlen(element)+1))==NULL)
    {
        error("Error, cannot allocate memory.\n");
    }

    /*repeat = search(element);
    if(repeat=1)
    {
        printf("Repeated.\n");
    }*/

    strcpy(cp, element);
    ptr->element = cp;

    if(tail==NULL)
    {
        ptr->next=ptr;
    }
    else
    {
        ptr->next=tail->next;
        tail->next=ptr;
    }
    tail=ptr;

    printf("   add:");
    display();
    printf("\n");
}


/*for removing items from queue*/
int rem(char *element)
{
    struct Data *ptr;
    char *cp;

    if(!tail)
    {
        printf("remove: queue is empty\n");
        return -1;
    }

    ptr=tail->next; /*get head*/
    cp=ptr->element;

    if(ptr==tail)
    {
        tail=NULL;
    }
    else
    {
        tail->next=ptr->next; /*move ptr to next in q*/
    }

    free(ptr);
    free(cp); /*free element*/

    printf("remove:");
    display();
    printf("\n");
}


int rem2(char *element) /*version without printfs for flush()*/
{
    struct Data *ptr;
    char *cp;

    if(!tail)
    {
        return 0;
    }

    ptr=tail->next; /*get head*/
    cp=ptr->element;

    if(ptr==tail)
    {
        tail=NULL;
    }
    else
    {
        tail->next=ptr->next; /*move ptr to next in q*/
    }

    free(ptr);
    free(cp); /*free element*/
    return 0;
}


void flush(char *element)
{
    if(!tail)
    {
        printf(" flush: queue is empty\n");
    }
    else
    {
        while(tail!=NULL)
        {
            rem2(element);/*empty queue one by one*/
        }
        printf(" flush:");
        display();
        printf("\n");
    }

}


int search(char *key)
{

    struct Data *ptr;
    char *cp;

    ptr=tail->next;

    while(ptr!=NULL)
    {
        cp=ptr->element;
        if(strcmp(cp,key)==1)
        {
            return 1;
        }
        else
        {
            ptr=ptr->next;
        }
    }
    return 0;
}


void error (char *e)
{
    perror (e);
    exit (1);
}
