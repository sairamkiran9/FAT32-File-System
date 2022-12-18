# project-3-group-9

### FAT32 File System

- <b>Course</b> &nbsp;&nbsp;&nbsp; : COP 4610 Operating Systems
- <b>Professor</b>  : Dr. Andy Wang
- <b>Group</b>  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; : 9
- <b>Group Members</b>  :
    - SM22BT &nbsp;: Sri Sai Ram Kiran Muppana 
    - SG22BX &nbsp;: Shanmukh Vishnuvardhan Rao Gongalla
    - MET19D &nbsp;: Matthew Torres

#### Divison of Labor

| NAME     | PART       |
|------    |----------- |
| Muppana  |  all parts        |
| Gongalla |    ls, cd               |
| Torres   | info, open, close |


#### List of Files

```
C files
---------
execute.c         :  This method parses the input and executes the shell commands "\n" is entered
execute_cd.c      :  This file contains methods for moving the directory back and the method to change the directory if exists.
execute_creat.c   :  This file has method to find the empty allocation in the fat img and another method that generates a file based on the space avaliability.
execute_ls.c      :  This file contains the methods ls and ls DIRNAME
execute_mkdir.c   :  This file creates a directory folder in the current path
execute_rm.c      :  Method to remove the file from the fat image
execute_rmdir.c   :  cleans the complete structure
execute_size.c    :  Iterates to all files and fetches the file size if the file exists.
main.c            :  Main file that loads the fat32 image and runs the shell in it.
utils.c           :  This file contains the common methods that are used my other methods.


Header Files
-------------
utils.h           : This file has all the intializations and struct definitions for the whole source code
 
```
#### Makefile Description

- `all` Basically runs all trgts one by one
- `shell` This target compiles all the files in that
- `clean` Removes all executable files 
- `main` This target compiles the main.c file 
- `utils` This target compiles the utils.c file
- `execute` This target compiles the execute.c file
- `execute_ls` This target compiles the execute_ls.c file
- `execute_cd` This target compiles the execute_cd.c file
- `execute_size` This target compiles the execute_size.c file
- `execute_creat` This target compiles the execute_creat.c file
- `execute_mkdir`	This target compiles the execute_mkdir.c file
- `execute_rm` This target compiles the execute_rm.c file
- `execute_rmdir` This target compiles the execute_rmdir.c file
- `execute_read` This target compiles the execute_read.c file
- `execute_open` This target compiles the execute_open.c file
- `execute_close` This target compiles the execute_close.c file
- `execute_rename` This target compiles the execute_rename.c file



#### Commands to execute
- To execute run `make` and than run `./shell fat32.img`


#### Known Bugs and Unfinished Portions
- rm function is not working
