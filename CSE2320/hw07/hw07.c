/*Michelle Gong
UTA ID 1001671203
CSE 2320, Homework 07*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct eligible /*eligible info*/
{
    char *subject;
    char *catalogue;
    struct eligible *next;
};

struct available
{
    int section;
    char *subject;
    char *catalogue;
    char *prof;
    char *building;
    char *room;
    char *startTime;
    char *endTime;
    int mon;
    int tues;
    int comp_start;
    int comp_end;
    struct available *next;
};

/*struct schedule
{
    int section;
    char *subject;
    int catalogue;
    char *prof;
    char *building;
    int room;
    char *startTime;
    char *endTime;
    struct schedule *next;
};*/

typedef struct eligible eList;
typedef struct available aList;
/*typedef struct schedule Monday;
typedef struct schedule Tuesday;*/

aList *swap(aList *ptr1, aList *ptr2)
{
    aList *temp;
    temp = ptr2->next;
    ptr2->next = ptr1;
    ptr1->next = temp;
    return ptr2;
}

int bubbleSort(aList *head, int counter)
{
    aList *h;
    int i, j, swapped;

    for(i=0;i<counter;i++)
    {
        h = head;
        swapped = 0;
        for(j=0;j<counter-i-1;j++)
        {
            aList *p1 = h;
            aList *p2 = p1->next;

            if(p1->comp_start > p2->comp_start)
            {
                h = swap(p1, p2);
                swapped = 1;
            }
            h = h->next;
        }
        if(swapped==0)
        {
            break;
        }
    }
}

void printSchedule(aList *c)
{
            if(strlen(c->startTime) < 5)
            {
                printf("0");
            }
            printf("%s  ", c->startTime);

            if(strlen(c->subject)<4)
            {
                int pad;
                pad = 0;
                while(pad<4-strlen(c->subject))
                {
                    printf(" ");
                    pad++;
                }
            }
            printf("%s ", c->subject);

            printf("%s.", c->catalogue);

            if(c->section<10)
            {
                printf("00");
            }
            else if(c->section<100)
            {
                printf("0");
            }
            printf("%d  ", c->section);

            if(strlen(c->startTime) < 5)
            {
                printf("0");
            }
            printf("%s--", c->startTime);

            if(strlen(c->endTime) < 5)
            {
                printf("0");
            }
            printf("%s  ", c->endTime);

            printf("%s ", c->building);
            printf("%s", c->room);
            if((strlen(c->building)+strlen(c->room))<8)
            {
                int pad;
                pad = 0;
                while(pad<8-strlen(c->building)-strlen(c->room))
                {
                    printf(" ");
                    pad++;
                }
            }
            printf("%s\n", c->prof);
}

void buildTuesday (aList *c, aList *h)
{
    int classStart;
    int classEnd;

    printf("\n***  Tuesday schedule  ***\n");

    classStart = 800;
    for(c = h; c; c=c->next)
    {
        int s;
        s = c->comp_start;
        s = s - classStart;
        if(s >= 0 && (c->tues) == 1)
        {
            printSchedule(c);
            classStart = c->comp_end;
        }
    }
    for(c = h; c; c=c->next)
    {
        free(c);
    }
}

void buildMonday(aList *c, aList *h)
{
    int classStart;
    int classEnd;

    printf("***  Monday schedule  ***\n");

    classStart = 800;
    for(c = h; c; c=c->next)
    {
        int s;
        s = c->comp_start;
        s = s - classStart;
        /*printf("CLASS START: %d ||", classStart);
        printf("S-VALUE: %d\n", s);*/
        if(s >= 0 && (c->mon) == 1)
        {
            printSchedule(c);
            c->tues = 0;
            classStart = c->comp_end;
        }
    }
    buildTuesday(c, h);
}

void buildAvailable(aList *c, aList *h)
{
    /*Largely utility function for testing

    int index = 1;

    printf("[ TESTING:: AVAILABLE LIST ]\n");
    for(c = h; c; c=c->next)
    {
        printf("%d) ", index);
        printf("%d, ", c->section);
        printf("%s, ", c->subject);
        printf("%s, ", c->catalogue);
        printf("%s, ", c->prof);
        printf("%s, ", c->building);
        printf("%s, ", c->room);
        printf("%s--%s, ", c->startTime, c->endTime);
        printf("MON: %d ", c->mon);
        printf("TUES: %d ", c->tues);
        printf("START: %d ", c->comp_start);
        printf("END: %d ", c->comp_end);
        printf("\n");
        index++;
    }*/
    buildMonday(c, h);
}

void findEligible(eList *c, eList *h) /*read in CSV and store only the eligible courses*/
{

    FILE *fp;
    aList *current, *head;
    char aLine[75]; /*buffer available courses*/

    /*variables for testing and checking input*/
    int counter = 0;
    int matches = 0;

    fp = fopen("fall2019courses.csv", "r");

    fgets(aLine, sizeof(aLine), fp);
    /*printf("Header tossed\n");*/

    head = current = NULL;

    while(fgets(aLine, sizeof(aLine), fp))
    {
        /*parse out variables*/
        char *temp; /*whole line*/
        char *tempSect;
        char *tempSub;
        char *tempCat;
        char *tempProf;
        char *tempBldg;
        char *tempRoom;
        char *tempStart;
        char *tempEnd;
        char *tempMon;
        char *tempTues;

        int hour;
        int minute;

        temp = (char*)malloc(sizeof(aLine));
        strcpy(temp, aLine);

        tempSect = strtok(temp, ",");
        tempSub = strtok(NULL, ",");
        tempCat = strtok(NULL, ",");
        tempProf = strtok(NULL, ",");
        tempBldg = strtok(NULL, ",");
        tempRoom = strtok(NULL, ",");
        tempStart = strtok(NULL, ",");
        tempEnd = strtok(NULL, ",");
        tempMon = strtok(NULL, ",");
        tempTues = strtok(NULL, "\n");

        /*check against eligible list*/
        for(c = h; c; c=c->next)
        {
            int matchSub;
            int matchCat;

            matchSub = strcmp(tempSub, c->subject);
            matchCat = strcmp(tempCat, c->catalogue);

            /*copy into available courses struct if eligible*/
            if(matchSub == 0 && matchCat == 0)
            {
                aList *node;
                node = malloc(sizeof(aList));

                node->section = atoi(tempSect);

                node->subject = (char*)malloc(sizeof(tempSub));
                strcpy(node->subject, tempSub);

                node->catalogue = (char*)malloc(sizeof(tempCat));
                strcpy(node->catalogue, tempCat);

                node->prof = (char*)malloc(sizeof(tempProf));
                strcpy(node->prof, tempProf);

                node->building = (char*)malloc(sizeof(tempBldg));
                strcpy(node->building, tempBldg);

                node->room = (char*)malloc(sizeof(tempRoom));
                strcpy(node->room, tempRoom);

                /*store an extra copy of start/end time as ints for easier sorting and comp*/
                node->startTime = (char*)malloc(sizeof(tempStart));
                strcpy(node->startTime, tempStart);

                hour = atoi(strtok(tempStart, ":"));
                minute = atoi(strtok(NULL, "\0"));
                node->comp_start = 100*hour + minute;

                node->endTime = (char*)malloc(sizeof(tempEnd));
                strcpy(node->endTime, tempEnd);

                hour = atoi(strtok(tempEnd, ":"));
                minute = atoi(strtok(NULL, "\0"));
                node->comp_end = 100*hour + minute;

                /*store Mon/Tues status as pseudo-bools*/
                if(strcmp(tempMon, "Y")==0)
                {
                    node->mon = 1;
                }
                else
                {
                    node->mon = 0;
                }

                if(strcmp(tempTues, "Y")==0)
                {
                    node->tues = 1;
                }
                else
                {
                    node->tues = 0;
                }

                /*printf("Match\n");
                matches++;*/

                if(head==NULL)
                {
                    current = head = node;
                }
                else
                {
                    current = current->next = node;
                }
            }
        }
        counter++;
    }

    for(c = h; c; c=c->next)
    {
        free(c);
    }
    /*bubbleSort(head, counter);*/
    buildAvailable(current, head);
    /*printf("Total Items: %d\n", counter);*/
    /*printf("Total Matches: %d\n", matches);*/
    fclose(fp);
}

void buildEligible() /*import eligible.txt*/
{
    FILE *fp;
    eList *current, *head;
    char eLine[20]; /*buffer eligible*/

    head = current = NULL;
    fp = fopen("eligible.txt", "r");

    /*no header line for elig file*/
    while(fgets(eLine, sizeof(eLine), fp))
    {
        char *temp;
        eList *node;

        node = malloc(sizeof(eList));
        temp = (char*)malloc(sizeof(eLine));
        strcpy(temp, eLine);

        node->subject = strtok(temp, " ");
        node->catalogue = strtok(NULL, "\n");
        node->next = NULL;

        if(head==NULL)
        {
            current = head = node;
        }
        else
        {
            current = current->next = node;
        }
    }

    fclose(fp);
    /*printf("Moving to Print\n");*/
    findEligible(current, head);
    /*printf("Moving to Available\n");*/
}

int main(void)
{
    /*printf("[ TESTING:: ELIGIBLE LIST ]\n");*/
    buildEligible();

    return 0;
}
