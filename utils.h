// ref: http://elm-chan.org/docs/fat_e.html#notes

#ifndef UTILS_H
#define UTILS_H

#define MAX_LIMIT1 0x0FFFFFF8
#define MAX_LIMIT2 0x0FFFFFFF
#define MIN_LIMIT1 0x00000000

typedef struct
{
    unsigned char BS_jmpBoot[3];
    char BS_OEMName[8];
    unsigned short BPB_BytsPerSec;
    unsigned char BPB_SecPerClus;
    unsigned short BPB_RsvdSecCnt;
    unsigned char BPB_NumFATs;
    unsigned short BPB_RootEntCnt;
    unsigned short BPB_TotSec16;
    unsigned char BPB_Media;
    unsigned short BPB_FATSz16;
    unsigned short BPB_SecPerTrk;
    unsigned short BPB_NumHeads;
    unsigned int BPB_HiddSec;
    unsigned int BPB_TotSec32;
    unsigned int BPB_FATSz32;
    unsigned short BPB_ExtFlags;
    unsigned short BPB_FSVer;
    unsigned int BPB_RootClus;
    unsigned short BPB_FSInfo;
    unsigned short BPB_BkBootSec;
    unsigned char BPB_Reserved[480];
    unsigned char BS_DrvNum;
    unsigned char BS_Reserved1;
    unsigned char BS_BootSig;
    unsigned int BS_VolID;
    unsigned char BS_VolLab[20];
    unsigned char BS_FilSysType[20];
} __attribute__((packed)) bootsector;

typedef struct
{
    int size;
    char **items;
} __attribute__((packed)) tokenlist;

typedef struct
{
    unsigned int current_cluster_number;
    unsigned char name[100];

    char currentpath[50][100];
    int current_cluster_path[50];
    int current;

} __attribute__((packed)) environment;

typedef struct
{
    unsigned char LDIR_Ord;
    unsigned char LDIR_Name1[10];
    unsigned char LDIR_Attr;
    unsigned char LDIR_Type;
    unsigned char LDIR_Chksum;
    unsigned char LDIR_Name2[12];
    unsigned short LDIR_FstClusLO;
    unsigned int LDIR_Name3;

    char name[11];
    unsigned char attr;
    unsigned char ntres;
    unsigned char crtTimeTenth;
    unsigned short crtTime;	
    unsigned short crtDate;	
    unsigned short lstAccDate;
    unsigned short fstClusHI;
    unsigned short wrtTime;	
    unsigned short wrtDate;	
    unsigned short fstClusLO;
    unsigned int fileSize;
} __attribute__((packed)) directory;

typedef struct
{
    char *user;
    char *machine;
    char *pwd;
} __attribute__((packed)) prompt;

struct Node
{
    char *data;
    struct Node* next;
};

void insert_node(char* new_data);
int delete_node(char* key);

/**
 * @brief method creates a tokenlist object.
 *
 * @return intialised token object
 */
tokenlist *new_tokenlist(void);
bootsector bs;
environment env;
directory dir;
struct Node *fat_node; 


/**
 * @brief This is the parser method
 * This method parses the string input based on the provided delimiter.
 *
 * @param char input and delimter
 * @return token object that contains parsed strings data and number of arguments.
 */
tokenlist *get_tokens(char *input, char *delimiter);

/**
 * @brief This method appends each parsed string into an array of strings
 *
 * @return Nothing.
 */
void add_token(tokenlist *tokens, char *item);

/**
 * @brief Clean intialised objects
 *  This method uses free() to clean all the allocated bytes in the memory.
 *  The method runs a for loop over the tokenlist objects and free's each allocation.
 *
 * @param tokenlist object tokens to be cleaned
 */
void free_tokens(tokenlist *tokens);

/**
 * @brief print tokens
 *  For the developer, to list down the list of arguments inputed by the user.
 *
 * @param tokenlist object tokens
 * @return Nothing
 */
void print_tokens(tokenlist *tokens);

/**
 * @brief Print prompt in the shell
 *  Each time this method is called a prompt in this format
 *  "USER@MACHINE : PWD >" will be printed in the shell.
 *  All the variables are fetched from the environment using the getenv() built-in fucntion.
 *
 * @return Nothing
 */
void print_prompt(const char *img_name);

void print_bootsector(bootsector bs);

void execute(const char *prompt, bootsector bs, FILE *img_file);

int get_first_sector_of_cluster(int n);

void move_to_prev_path(void);

int cd(int current_dir_cluster_num, char *DIRNAME, FILE *img_file);

void ls(int current_cluster_number, FILE *img_file);

void lsDir(int current_cluster_number, char *DIRNAME, FILE *img_file);

int sizeOf(int current_dir_cluster_num, char *DIRNAME, FILE *img_file);

void add_to_path(int current_cluster, char *name);

void print_env(void);

int sizeOf(int current_dir_cluster_num, char *DIRNAME, FILE *img_file);

int find_empty_cluster(FILE *img_file);

void createFile(int current_dir_cluster_num, char *DIRNAME, FILE *img_file);

void makeDir(int current_dir_cluster_num, char * DIRNAME, FILE *img_file);

directory zeroOutDir(void);

int rm(int current_dir_cluster_num, char * DIRNAME, FILE *img_file);

void rmDir(int current_dir_cluster_num, char *DIRNAME, FILE *img_file);

int readFile(int current_dir_cluster_num, char *DIRNAME, FILE *img_file);

int openFile(int current_dir_cluster_num, char *DIRNAME, FILE *img_file);

int closeFile(int current_dir_cluster_num, char *DIRNAME, FILE *img_file);

int renameFile(int current_dir_cluster_num, char *DIRNAME, char *new_name, FILE *img_file);

#endif