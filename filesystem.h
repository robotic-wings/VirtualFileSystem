#ifndef FILESYSTEM_H_INCLUDED
#define FILESYSTEM_H_INCLUDED

#define STRING_SIZE 1024
#define MAX_ARGS_COUNT 3
#define MAX_CHILDREN_COUNT 200
#define STORAGE_SIZE 2048

#include <stdio.h>
#include <string.h>
#include "utils.h"

typedef enum
{
    REGULAR_FILE,
    DIRECTORY
} file_type;

typedef struct
{
    void *parent;   // parent node
    void *children[MAX_CHILDREN_COUNT];   // children linear list
    int children_tail;   // the tail of children linear list
    char file_name[STRING_SIZE];   // name of the node
    file_type type;   // type of the node
    int is_root;   // is the node a root directory 
    int has_content;   // does the node have content?
} file_node;

typedef struct
{
    file_node root_node;   // the specifically assigned root node space
    file_node *pwd;    // the reference of the current working directory
} environment;

typedef struct
{
    char *path; // the path input by the user
    int success; // whether the target node is found
    int term_pos;   // the halting position of path reading
    int parent_folder_path_endpos;    // the end character position of the parent directory
    file_node *file; // the target node
    file_node *parent_folder;   // the target node's parent directory, which is used for touch / mkdir
} find_file_by_path_result;

typedef struct
{
    int *argc;
    file_node *storage;
    int *storage_tail;
    environment *env;
} handle;

/**
 * @brief Initalize a file node space into a root node.
 * 
 * @param root The reference of the root node
 */
void init_root_node(file_node *root);

/**
 * @brief Initialize a node space into a file. Used by touch & mkdir.
 * 
 * @param file The reference of the file to initialize
 * @param file_name The name of the file
 * @param type The type of the file
 * @param parent The parent directory of the node
 * @return file_node* The reference of the initialized file node.
 */
file_node *init_file(file_node *file, char file_name[STRING_SIZE], file_type type, file_node *parent);

/**
 * @brief Tell the user the specified file cannot be found.
 * 
 * @param r The result of the function find_file_by_path
 */
void raise_file_not_found(find_file_by_path_result *r);

/**
 * @brief Find the reference of node that represents a file and its parent directory. 
 * The function is able to parse the path of a non-existent file to know its parent directory. 
 * 
 * @param path The path input by the user
 * @param env The reference of environment variable set (pwd + root node)
 * @param r The memory space to store the finding
 */
void find_file_by_path(char *path, environment *env, find_file_by_path_result *r);

/**
 * @brief Create a file by path input by the user
 * 
 * @param hdl The context of the current operation
 * @param filepath The path input by the user
 * @param type The type of the file (regular file / directory) 
 */
void create_file_by_path(handle *hdl, char *filepath, file_type type);

/**
 * @brief Get the path string as per the node of a file. 
 * 
 * @param dest_str The memory space to store the file path result
 * @param file The target file node
 */
void get_file_path(char dest_str[STRING_SIZE], file_node *file);

#endif