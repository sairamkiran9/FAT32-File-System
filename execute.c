/**
 * @file execute.c
 * @brief This method parses the input and executes the shell commands
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define DEFAULT_BUFSIZE 5

char *get_input(void)
{
    /**
     * @brief This method fetches the input from the user until "\n" is entered
     *
     * @return user input in string format
     */
    int bufsize = 0;
    char *buffer = NULL;
    char cmd_input[DEFAULT_BUFSIZE];

    while (fgets(cmd_input, DEFAULT_BUFSIZE, stdin))
    {
        /*
         * fgets takes in the input in the size of DEFAULT_BUFSIZE and,
         * get concatinated to the buffer variable during each iteration.
         */
        char *eol = strchr(cmd_input, '\n');
        int cur_size = 0;

        if (eol != NULL)
        {
            cur_size = eol - cmd_input;
        }
        else
        {
            cur_size = DEFAULT_BUFSIZE - 1;
        }
        /*
         * reallocating the memorry size to add new input string.
         */
        buffer = (char *)realloc(buffer, bufsize + cur_size);
        memcpy(&buffer[bufsize], cmd_input, cur_size);
        bufsize += cur_size;

        if (eol != NULL)
        {
            /*
             * Break the loop if there is a new line entered by the user
             */
            break;
        }
    }

    buffer = (char *)realloc(buffer, bufsize + 1);
    buffer[bufsize] = 0;
    return buffer;
}

void execute(const char *img_name, bootsector bs, FILE *img_file)
{
    /**
     * @brief Method that compares the input command and starts executing
     *
     */

    env.current = 0;
    add_to_path(bs.BPB_RootClus, "/");
    while (1)
    {
        print_prompt(img_name);
        char *input = get_input();
        tokenlist *tokens = get_tokens(input, " ");
        if (tokens->size == 0)
        {
            continue;
        }
        else if (strcmp(tokens->items[0], "exit") == 0)
        {
            exit(0);
        }
        else if (strcmp(tokens->items[0], "info") == 0)
        {
            print_bootsector(bs);
        }
        else if (strcmp(tokens->items[0], "ls") == 0)
        {
            if (tokens->size == 2 && strcmp(tokens->items[1], ".") != 0)
            {
                if (strcmp(tokens->items[1], "..") == 0)
                {
                    if (env.current - 2 == 0)
                    {
                        ls(bs.BPB_RootClus, img_file);
                    }
                    else
                    {
                        ls(env.current_cluster_path[env.current - 2], img_file);
                    }
                }
                else
                {
                    lsDir(env.current_cluster_number, tokens->items[1], img_file);
                }
            }
            else
            {
                ls(env.current_cluster_number, img_file);
            }
        }
        else if (strcmp(tokens->items[0], "cd") == 0)
        {
            if (tokens->items[1] == NULL)
            {
                while (env.current > 1)
                {
                    move_to_prev_path();
                }
            }
            else
            {
                if (strcmp(tokens->items[1], ".") == 0)
                {
                    continue;
                }
                else if (strcmp(tokens->items[1], "..") == 0)
                {
                    move_to_prev_path();
                }
                else
                {
                    int new_cluster_number = cd(env.current_cluster_number,
                                                tokens->items[1], img_file);

                    if (env.current_cluster_number != new_cluster_number)
                    {
                        add_to_path(cd(env.current_cluster_number,
                                       tokens->items[1], img_file),
                                    tokens->items[1]);
                    }
                }
            }
        }
        else if (strcmp(tokens->items[0], "size") == 0)
        {
            if (tokens->items[1] == NULL)
            {
                printf("Error: missing argument for command size\n");
            }
            else
            {
                printf("\"%s\" filename size is %d\n", tokens->items[1],
                       sizeOf(env.current_cluster_number, tokens->items[1], img_file));
            }
        }
        else if (strcmp(tokens->items[0], "creat") == 0)
        {
            if (tokens->items[1] != NULL)
            {
                createFile(env.current_cluster_number, tokens->items[1], img_file);
            }
            else
            {
                printf("Error: missing argument for command creat\n");
            }
        }
        else if (strcmp(tokens->items[0], "mkdir") == 0)
        {
            if (tokens->items[1] != NULL)
            {
                makeDir(env.current_cluster_number, tokens->items[1], img_file);
            }
            else
            {
                printf("Error: missing argument for command mkdir\n");
            }
        }
        else if (strcmp(tokens->items[0], "rm") == 0)
        {
            if (tokens->items[1] != NULL)
            {
                rm(env.current_cluster_number, tokens->items[1], img_file);
            }
            else
            {
                printf("Error: missing argument for command rm\n");
            }
        }
        else if (strcmp(tokens->items[0], "rmdir") == 0)
        {
            if (tokens->items[1] != NULL)
            {
                rmDir(env.current_cluster_number, tokens->items[1], img_file);
            }
            else
            {
                printf("Error: missing argument for command rmdir\n");
            }
        }
        else if (strcmp(tokens->items[0], "open") == 0)
        {
            if (tokens->size < 3)
            {
                printf("Invalid argument\n");
            }
            else if (strcmp(tokens->items[2], "r") == 0)
            {
                openFile(env.current_cluster_number, tokens->items[1], img_file);
            }
            else
            {
                printf("File can be accessed via read mode only.\n");
            }
        }
        else if (strcmp(tokens->items[0], "close") == 0)
        {
            if (tokens->items[1] != NULL)
            {
                closeFile(env.current_cluster_number, tokens->items[1], img_file);
            }
            else
            {
                printf("Error: missing argument for command close\n");
            }
        }
        else if (strcmp(tokens->items[0], "read") == 0)
        {
            if (tokens->items[1] != NULL)
            {
                readFile(env.current_cluster_number, tokens->items[1], img_file);
            }
            else
            {
                printf("Error: missing argument for command read\n");
            }
        }
        else if (strcmp(tokens->items[0], "rename") == 0)
        {
            if(tokens->size < 3){
                break;
            }
            else if (tokens->items[1] != NULL && tokens->items[2] != NULL)
            {
                // renameFile(env.current_cluster_number, tokens->items[1], tokens->items[2], img_file);
            }
            else
            {
                printf("Error: missing argument for command read\n");
            }
        }
        else
        {
            printf("Invalid command\n");
        }
    }
}