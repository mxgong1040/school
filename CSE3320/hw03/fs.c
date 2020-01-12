//Michelle Gong
//UTA ID 1001671203
//CSE 3320, Assignment 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <pwd.h>
#include "fs.h"

struct MyFileSystem fs;
struct FileNameTable fnt;
struct DirectAttPtrTable dabpt;
struct BlockPtrTable bpt;

int num_files;

int main()
{
    menu();
    return 0;
}

void menu()
{
    int choice;
    int cont = 1;

    while(cont!=0)
    {
    printf("[MAIN MENU]\n");
    printf("[1] Create File System\n");
    printf("[2] Format File System\n");
    printf("[3] Save File System\n");
    printf("[4] Open File System\n");
    printf("[5] List Files\n");
    printf("[6] Remove File\n");
    printf("[7] Rename File\n");
    printf("[8] Put Host File Into Disk\n");
    printf("[9] Get Disk File Into Host\n");
    printf("[10] User Info\n");
    printf("[11] Link File\n");
    printf("[12] Unlink File\n");
    printf("[13] Set File Permissions\n");
    printf("[14] Check Disk\n");
    printf("[0] Quit\n");
    scanf("%d", &choice);

        switch(choice)
        {
        case 0:
            exit(0);
        case 1: //file system
            printf("Creating disk...\n");
            init_FS();
            break;
        case 2:
            printf("Formatting disk...\n");
            formatFS();
            break;
        case 3:
            printf("Saving disk...\n");
            saveFS();
            break;
        case 4:
            printf("Opening disk...\n");
            break;
        case 5:
            printf("Listing files on disk...\n");
            listFile();
            break;
        case 6:
            printf("Removing file...\n");
            break;
        case 7:
            printf("Renaming file...\n");
            break;
        case 8:
            printf("Putting file on disk...\n");
            putFile();
            break;
        case 9:
            printf("Getting file from disk...\n");
            getFile();
            break;
        case 10:
            printf("Getting user info...\n");
            break;
        case 11:
            printf("Linking file...\n");
            break;
        case 12:
            printf("Unlinking file...\n");
            break;
        case 13:
            printf("Setting file permissions...\n");
            break;
        case 14:
            printf("Checking disk...\n");
            break;
        }
    }
}

/******************************/
/********Main Functions********/
/******************************/

//initialise FS with #datablocks
void init_FS()
{
    int num_blocks;
    int i, j;
    char **data; //hold data blocks

    printf("Enter number of data blocks: ");
    scanf("%d", &num_blocks);

    fs.num_blocks = num_blocks;
    data=(char**)malloc(SIZE_BLOCKS*sizeof(char*));
    for(i=0;i<num_blocks;i++)
    {
        data[i]=(char*)malloc(num_blocks*sizeof(char));
    }

    fs.index_data=malloc(num_blocks*sizeof(int*));
    fs.dBlocks=data;

    printf("Disk created with %d data blocks.\n\n", num_blocks);

}

//allocate memory, set up temp name for later use
void formatFS()
{
    int num_entries;
    char tempName[SIZE_FILENAME];

    printf("Enter a temporary name for the file system:\n");
    scanf("%s", tempName);
    strcpy(fs.diskName, tempName);

    printf("Enter number of files and DABPT entries in the following format:\n");
    printf("#num_files #num_entries\n");
    scanf("%d %d", &num_files, &num_entries);

    fs.free_FNT=num_files;
    fs.free_DABPT=num_entries;

    fs.flist=malloc(num_files*sizeof(struct FileNameTable));
    fs.dabpt=malloc(num_entries*sizeof(struct DirectAttPtrTable));
    fs.bpt=malloc(num_entries*sizeof(struct BlockPtrTable));

    fs.index_FNT=malloc(num_files*sizeof(int*));
    fs.index_DABPT=malloc(num_entries*sizeof(int*));
    fs.index_BPT=malloc(num_entries*sizeof(int*));

    printf("Disk formatted with %d files and %d DABPT entries.\n", num_files, num_entries);

}

void saveFS()
{
    struct dabpt* proc;
    char fsName[SIZE_FILENAME];
    int i, j;

    printf("Enter disk name:\n");
    scanf("%s", fsName);
    if(sizeof(fsName)>50)
    {
        printf("Invalid file name, keep under 50 characters.\n");
    }

    FILE *ofile=fopen(fsName, "w");
    if(ofile==NULL)
    {
        printf("Error, file could not be opened.");
        exit(1);
    }

    //details into file
    //fprintf(ofile,"!%s",fs.diskName);
    fprintf(ofile,"!%d",fs.free_FNT);
    fprintf(ofile,"!%d",fs.free_DABPT);
    fprintf(ofile,"!%d",fs.free_BPT);
    fprintf(ofile,"!%d",fs.num_blocks);
    fprintf(ofile,"\n");

    for(i=0;i<fs.free_FNT;i++)
    {
        //fprintf(ofile, "!%s", fs.flist[i].diskName);

        //inodes
   /*fprintf(ofile,"!%d",fs.files[i].inode->filesize);
    fprintf(ofile,"!%s",fs.files[i].inode->dateMod);
    fprintf(ofile,"!%d",fs.files[i].inode->dBlockptr);
    fprintf(ofile,"!%s",fs.files[i].inode->pNum);*/
    }

    for(j=0;i<SIZE_BLOCKS;j++)
    {
       //fs.dBlocks[fs.flist[i].inode->dBlockptr][j];
    }

    fclose(ofile);

}

void openFS()
{

}

void listFile()
{
    DIR *d;
    struct dirent *dir;
    int i=0;

    printf("[FILES]\n");

    //current dir
    d=opendir(".");
    while((dir=readdir(d)))
    {
        if(((dir->d_type)&DT_REG))
        {
            printf("%d. %s", i, dir->d_name);
        }
        i++;
    }
    closedir(d);

}

void removeFile()
{
    printf("Enter name of file to remove:\n");

}

void renameFile()
{
    DIR *d;
    struct dirent *dir;
    int i=0;

    char nameCurrent[SIZE_FILENAME];
    char nameNew[SIZE_FILENAME];

    printf("Enter name of file to rename:\n");
    scanf("%s", nameCurrent);

    printf("Enter new filename:\n");
    scanf("%s", nameNew);

    d=opendir(".");
    /*while((dir=readdir(d)))
    {
        if(((dir->d_type)&DT_REG))
        {
            printf("%d. %s", i, dir->d_name);
        }
        i++;
    }*/
    closedir(d);

}

void putFile()
{
    char fileName[SIZE_FILENAME];
    DIR *d;
    struct dirent *dir;
    d=opendir(".");

    printf("Enter file to move from Host OS to disk:\n");
    scanf("%s", fileName);

    bool valid = true;
    while((dir=readdir(d)))
    {
        if(((dir->d_type)&DT_REG))
        {
            if(!strcmp(fileName,dir->d_name))
            {
                printf("Moving file %s to disk...\n", fileName);
                makeFile(fileName);
                valid=true;
                closedir(d);
                return;

            }
        }
    }
    if(!valid)
    {
        printf("Error, file not found.\n");
    }
    closedir(d);
}


void getFile()
{
    char fsName[SIZE_FILENAME];

    printf("Enter file to move to Host OS:\n");
    scanf("%s", fsName);

}

void getUser()
{
    struct stat statBuf;
    uid_t uid = statBuf.st_uid; //user id.

    struct passwd *p;

    //get user name
    p = getpwuid(uid);
    printf("User: %s\n", p->pw_name);
}

void linkFile()
{
    //int symlink(const char *path1, const char *path2);
    /*char *path1=
    char *path2=
    int status;
    status = link(path1, path2)
    */


}

void unlinkFile()
{
    /*char *path1=
    char *path2=
    int status;
    status = link(path1, path2)
    */
}

/******************************/
/*******Helper Functions*******/
/******************************/

int findFile(const char *name)
{
    DIR *d;
    struct dirent *dir;
    int i=0;

    d=opendir(".");
    /*while((dir=readdir(d)))
    {
        if(((dir->d_type==4)
        {
            if(strcmp(dir->d_name, ".")==0 || strcmp(dir->d_name,"..")==0)
            {
                continue;
            }
            printf
        }
        i++;
    }*/
    closedir(d);
}

int getSize(const char *name)
{
    const char *f=name;

    //get file info
    struct stat *b;
    b=malloc(sizeof(struct stat));

    stat(f,b);
    int size=b->st_size;
    return size;
    free(b);
}


int makeFile(char* fileName)
{
    //taken indexes
    int takenFNT;
    int takenDABPT;
    int takenBPT;
    int takendata;

    if(findFree(fs.index_FNT, fs.free_FNT)==-1)
    {
        printf("No space remaining in FNT.\n");
        return -1;
    }
    else if(findFree(fs.index_data, fs.num_blocks)==-1)
    {
        printf("No space remaining in data blocks.\n");
        return -1;
    }
    else if(findFree(fs.index_DABPT, fs.free_DABPT)==-1)
    {
        printf("No space remaining in DABPT.\n");
        return -1;
    }
    else if((findInode(fs.index_data, fs.num_blocks)*SIZE_BLOCKS)<(getSize(fileName)))
    {
        printf("No space remaining in data.\n");
        return -1;
    }
    else
    {
        printf("Updating FS...\n");
        takenFNT=findFree(fs.index_FNT, fs.free_FNT);
        takenDABPT=findFree(fs.index_DABPT, fs.free_DABPT);
        takenBPT=findFree(fs.index_BPT, fs.free_BPT);
        takendata=findFree(fs.index_data, fs.num_blocks);

        fs.index_FNT[takenFNT]=1;
        fs.index_BPT[takenBPT]=1;
        fs.index_DABPT[takenDABPT]=1;
        fs.index_data[takendata]=1;

        printf("Updating FNT...\n");
        strcpy(fs.flist[findFree(fs.index_FNT, fs.free_FNT)].fileName, fileName);
        fs.flist[takenFNT].inode=fs.dabpt[takenDABPT];



       /* printf("Updating DABPT...\n");
        fs.flist[takenFNT].inode->filesize=getSize(fileName);
        strcpy(fs.flist[takenFNT].inode->dateMod,getDateMod(fileName));
        fs.flist[takenFNT]->inode.dBlockptr=takendata;
        /**********************************
        strcpy(fs.flist[takenFNT].inode->pNum,"000");
        ***********************************

        printf("Writing data to blocks...\n");
        makeData(fileName, takendata, takenFNT, takenBPT, takenDABPT);*/

        return 1;

    }
}

char* dateMod(char* name)
{
    char *f=name;
    struct stat *b;
    b=malloc(sizeof(struct stat));

    //get last mod time from file details
    stat(f,b);
    time_t t=b->st_mtime;
    return ctime(&t);
    free(b);
}

int findFree(int* checkFree, int size) //find free slot in directory
{
  int i;
  int ret = -1;
  for(i=0;i<size;i++)
  {
    if(checkFree[i]==0)
    {
      ret = i;
      break;
    }
  }
  return ret;
}

int findInode(int* checkFree, int size)
{
  int i;
  int ret = 0;
  for(i=0;i<size;i++)
  {
      if(checkFree[i]!=1)
      {
          ret=ret+1;
      }
  }
  return ret;
}

/*int findFNT() //find free slot in FNT
{
  int i;
  int ret = -1;
  for( i = 0; i < num_files; i++ )
  {
    if(fnt[i]->valid == 0 )
    {
      ret = i;
      break;
    }
  }
  return ret;
}*/
