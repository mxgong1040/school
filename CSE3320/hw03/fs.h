#ifndef FS_H
#define FS_H

/***Constants***/
#define SIZE_BLOCKS 512
#define SIZE_TIME 100
#define SIZE_FILENAME 50
#define SIZE_FNT 54 //filename size + int (4)

/***Main Functions***/
void menu();
void init_FS();
void formatFS();
void saveFS();
void openFS();
void listFile();
void removeFile();
void renameFile();
void putFile();
void getFile();
void getUser();
void linkFile();
void unlinkFile();
//void setPerm();
//void checkDisk();


/***Helper Functions***/
int findFree();
int findInode();
int makeFile();
int getSize(const char *name);
char *dateMod(char *name);
int fileFile(const char *name);

/***Structs***/
struct DirectAttPtrTable
{
    int filesize;
    char dateTime[SIZE_TIME]; //last modified date/time
    int dBlockptr; //data block pointer
    char user[50]; //user who owns file
};

struct FileNameTable
{
    char fileName[SIZE_FILENAME];
    struct DirectAttPtrTable inode; //inode to DABPT

};

struct BlockPtrTable
{
    int directBlockptr; //direct ptr to data block
    struct BlockPtrTable *ptr2; //extra pointer for big files
};

struct FileLink
{
    char link[SIZE_FILENAME]; //max size link same as filename max
    struct BlockPtrTable *linkedFile;
};

struct MyFileSystem
{
    struct FileNameTable *flist;
    struct DirectAttPtrTable *dabpt;
    struct BlockPtrTable *bpt;
    struct fLink *flink[SIZE_FILENAME];

    char diskName[SIZE_FILENAME];
    int num_blocks; //all blocks in system

    int free_DABPT; //free blocks in DABPT, FNT, BPT
    int free_FNT;
    int free_BPT;
    char **dBlocks; //data blocks

    int *index_DABPT; //tracks free/taken blocks
    int *index_FNT;
    int *index_BPT;
    int *index_data;

};

#endif // FS_H
