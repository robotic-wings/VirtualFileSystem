#include "interface.h"

void change_dir(char* filepath, environment *env)
{
    find_file_by_path_result result;
    find_file_by_path(filepath, env, &result);
    if (result.success && result.file->type == DIRECTORY)
    {
        env->pwd = result.file;
    }
    else
    {
        printf("%s: No such file or directory\n", filepath);
    }
}

void sort_children(file_node *folder)
{
    for (int i = 0; i < folder->children_tail; i++)
    {
        for (int j = 0; j < folder->children_tail; j++)
        {
            file_node *a = folder->children[j];
            file_node *b = folder->children[j + 1];
            enum {
                UNDETERMINED = -1,
                NO_SWAP = 0,
                SWAP = 1
            } c = UNDETERMINED;
            char *a_name = a->file_name;
            char *b_name = b->file_name;
            int a_name_len = strlen(a_name);
            int b_name_len = strlen(b_name);
            int n;   // common length
            // common length = min(length of string a, length of string b)
            if (a_name_len > b_name_len)
            {
                n = b_name_len;
            }
            else
            {
                n = a_name_len;
            }
            // compare ASCII within the common length
            for (int k = 0; k < n; k++)
            {
                if (a_name[k] > b_name[k])
                {
                    c = SWAP;
                    break;
                }
                else if (a_name[k] < b_name[k])
                {
                    c = NO_SWAP;
                    break;
                }
            }
            /* If one name is longer than another but the words are identical for their common length,
               then the shorter name comes first. */
            if (c == UNDETERMINED)
            {
                if (a_name_len > b_name_len)
                {
                    c = SWAP;
                }
                else
                {
                    c = NO_SWAP;
                }
            }
            if (c == SWAP)
            {
                folder->children[j] = b;
                folder->children[j + 1] = a;
            }
        }
    }
}

void ls(char *dir_path, environment *env)
{
    file_node *target_dir;
    find_file_by_path_result result;
    if (strlen(dir_path) > 0)   // get the target directory if the directory path is specified
    {
        find_file_by_path(dir_path, env, &result);
        if (result.success && result.file->type == DIRECTORY)
        {
            target_dir = result.file;
        }
        else
        {
            printf("%s: No such file or directory\n", dir_path);
            return;
        }
    }
    else
    {
        target_dir = env->pwd;   // dir path is not specified, use current working directory
    }
    // only sort children as per ls requests
    sort_children(target_dir);
    // print a list of children in the directory
    for (int i = 0; i <= target_dir->children_tail; i++)
    {
        file_node *child = (file_node *)target_dir->children[i];
        printf("%s", child->file_name);
        if (i < target_dir->children_tail)
        {
            printf(" ");
        }
    }
    // print a new line character if something is printed
    if (target_dir->children_tail > -1)
    {
        printf("\n");
    }
}

void remove_file(char *filepath, environment *env, file_type type)
{
    find_file_by_path_result result;
    find_file_by_path(filepath, env, &result);
    if (result.success && result.file->type == type)
    {
        file_node *file = result.file;
        if (file->is_root)
        {
            return;
        }
        file_node *file_parent = file->parent;
        // the child does not claim the parent anymore
        int child_pos;
        for (int i = 0; i <= file_parent->children_tail; i++)
        {
            if (file_parent->children[i] == file)
            {
                child_pos = i;
                break;
            }
        }
        for (int i = child_pos + 1; i <= file_parent->children_tail; i++)
        {
            file_parent->children[i - 1] = file_parent->children[i];
        }
        file_parent->children_tail--;
        // the parent does not claim the child anymore
        file->parent = NULL;
    }
    else
    {
        printf("%s: No such file or directory\n", filepath);
    }
}

void append_file(char *filepath, char *content, environment* env) {
    find_file_by_path_result result;
    find_file_by_path(filepath, env, &result);
    if (result.success && result.file->type == REGULAR_FILE) {
        // get the virtual file metadata
        file_node* virtual = result.file;
        // get the real file name
        char real_file_name[STRING_SIZE];
        strcpy(real_file_name, virtual->file_name);
        strcat(real_file_name, ".file");
        // append the new line to the real file
        FILE* actual = fopen(real_file_name, "a");
        fputs(content, actual);
        fputc('\n', actual);
        fclose(actual);
        virtual->has_content = 1;
    } else {
        printf("%s: No such file or directory\n", filepath);
    }
}

void print_file(char *filepath, environment* env) {
    // find the target file
    find_file_by_path_result result;
    find_file_by_path(filepath, env, &result);
    if (result.success && result.file->type == REGULAR_FILE) {
        file_node* virtual = result.file;
        if (virtual->has_content) {
            // read and print the content of the file line by line
            char real_file_name[STRING_SIZE];
            strcpy(real_file_name, virtual->file_name);
            strcat(real_file_name, ".file");
            FILE* actual = fopen(real_file_name, "r");
            char current_line[STRING_SIZE];
            while (1) {
                char* rtn = fgets(current_line, STRING_SIZE, actual);
                if (rtn != NULL) {
                    printf("%s", rtn);
                } else {
                    break;
                }
            }
            fclose(actual);
        }
    } else {
        printf("%s: No such file or directory\n", filepath);
    }
}