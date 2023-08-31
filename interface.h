#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "filesystem.h"

/**
 * @brief Change the current working directory. Used by cd.
 * 
 * @param filepath The path of new working directory
 * @param env The reference of environment variable set (pwd + root node)
 */
void change_dir(char *filepath, environment *env);

/**
 * @brief Sort all child nodes in a directory in ASCII order. Used by ls.
 * 
 * @param folder The target directory
 */
void sort_children(file_node *folder);

/**
 * @brief List all files in a directory.
 * 
 * @param filepath The target directory
 * @param env The reference of environment variable set (pwd + root node)
 */
void ls(char *filepath, environment *env);

/**
 * @brief Remove a regular file / directory. Used by rm & rmdir.
 * 
 * @param filepath The path of the regular file
 * @param env The reference of environment variable set (pwd + root node)
 * @param type The required file type. The function works for the command rm when type = REGULAR_FILE.
 * and rmdir when type = DIRECTORY.
 */
void remove_file(char *filepath, environment *env, file_type type);

/**
 * @brief Append a line to the content of a file. Used by append.
 * 
 * @param filepath The path of the regular file
 * @param content The line of string to append
 * @param env The reference of environment variable set (pwd + root node)
 */
void append_file(char *filepath, char *content, environment* env);

/**
 * @brief Print the content of a file. Used by cat.
 * 
 * @param filepath The path of the regular file
 * @param env The reference of environment variable set (pwd + root node)
 */
void print_file(char *filepath, environment* env);


#endif