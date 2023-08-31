#include "filesystem.h"

void init_root_node(file_node *root)
{
    root->is_root = 1;
    root->type = DIRECTORY;
    root->children_tail = -1;
    root->parent = NULL;
    root->file_name[0] = '\0';
    root->has_content = 0;
}

file_node *init_file(file_node *file, char file_name[STRING_SIZE], file_type type, file_node *parent)
{
    strcpy(file->file_name, file_name);
    file->type = type;
    file->is_root = 0;
    file->children_tail = -1;
    file->has_content = 0;
    // establish the parent-child relationship with the current node and specified parent node
    file->parent = parent;
    parent->children[++(parent->children_tail)] = file;
    return file;
}

void raise_file_not_found(find_file_by_path_result *r)
{
    char err_msg[STRING_SIZE];
    get_substr(err_msg, r->path, 0, r->term_pos);
    printf("%s: No such file or directory\n", err_msg);
}

void find_file_by_path(char *path, environment *env, find_file_by_path_result *r)
{
    file_node *cur_node_folder = NULL, *cur_node = NULL;
    // initialize result struct
    r->path = path;   
    r->success = 0;   
    r->file = NULL;   
    r->parent_folder = NULL;  
    r->term_pos = 0;   
    r->parent_folder_path_endpos = 0;  
    int i = 0, needs_path_type_check = 1;
    char buffer[256];
    int buffer_tail = -1;
    int stop = 0;
    while (!stop)
    {
        if (needs_path_type_check)
        {
            if (path[i] == '/')   // the path is an absolute one
            {
                cur_node = &env->root_node;
                i++;   // i increases from 0 to 1
            }
            else   // the path is a relative one
            {
                cur_node = env->pwd;
            }
            needs_path_type_check = 0;   
        }
        else  // enter path parsing process
        {
            if (path[i] == '/' || path[i] == '\0')   // if a path level is terminating
            {
                if (path[i] == '\0')   // if the whole path string is terminating
                {
                    if (buffer_tail > -1)
                    {
                        // if buffer is not empty, stop in next turn to dump the buffer
                        stop = 1;
                    }
                    else
                    {
                        // buffer empty, stopping
                        break;
                    }
                }
                buffer[++buffer_tail] = '\0';
                int found = 0;   // whether the current path level's corresponding level is found
                if (strcmp(".", buffer) == 0 || buffer[0] == '\0')
                {
                    // stay around, do not jump to anywhere
                    found = 1;
                }
                else if (strcmp("..", buffer) == 0)
                {
                    // if the current node is not root, jump to the higher level
                    if (!cur_node->is_root)
                    {
                        file_node *parent = (file_node *)(cur_node->parent);
                        if (parent->is_root)
                        {
                            cur_node_folder = NULL;
                        }
                        else
                        {
                            cur_node_folder = (file_node *)parent->parent;
                        }
                        cur_node = parent;
                    }
                    found = 1;
                }
                else
                {
                    // check if the current path level exists in the children list of the current node
                    for (int j = 0; j <= cur_node->children_tail; j++)
                    {
                        file_node *child = (file_node *)cur_node->children[j];
                        if (strcmp(child->file_name, buffer) == 0)
                        {
                            cur_node_folder = cur_node;
                            cur_node = child;
                            found = 1;
                        }
                    }
                }
                if (!found)
                {
                    // no node corresponds with the current path level, terminating 
                    r->success = 0;
                    r->file = NULL;
                    r->parent_folder = cur_node;
                    r->term_pos = i;
                    return;
                }
                else
                {
                    // node found, updating the end character position of the parent directory
                    r->parent_folder_path_endpos = i;
                }
                // empty the buffer
                buffer_tail = -1;
            }
            else
            {
                // append the buffer
                buffer[++buffer_tail] = path[i];
            }
            i++;
        }
    }
    // normal ending means finding the corresponding node of the file path successfully
    r->success = 1;
    r->file = cur_node;
    r->parent_folder = cur_node_folder;
    r->term_pos = i;
}

void create_file_by_path(handle *hdl, char *filepath, file_type type)
{
    handle h = *hdl;
    find_file_by_path_result result;
    find_file_by_path(filepath, h.env, &result);
    if (result.success == 0)   // the file to create SHOULD NOT exist
    {
        if (result.file == NULL && result.parent_folder != NULL)
        {     // file to create does not exist...
            if (result.term_pos == strlen(filepath) && result.parent_folder->type)
            { // and target directory exists
                char filename[STRING_SIZE];
                // get the starting position of the filename in the path
                int filename_startpos;
                if (filepath[result.parent_folder_path_endpos] == '/')
                {
                    filename_startpos = result.parent_folder_path_endpos + 1;
                }
                else
                {
                    filename_startpos = result.parent_folder_path_endpos;
                }
                // get the filename of the file to create
                get_substr(filename, filepath, filename_startpos, result.term_pos);
                // set the file metadata
                init_file(&h.storage[++(*h.storage_tail)], filename, type, result.parent_folder);
            }
            else
            {
                char err_msg[STRING_SIZE];
                get_substr(err_msg, filepath, 0, result.term_pos);
                printf("%s: No such file or directory\n", err_msg);
            }
        }
    }
}


void get_file_path(char dest_str[STRING_SIZE], file_node *file)
{
    char *ancestors_names[STRING_SIZE];
    int ancestor_names_tail = -1;
    // get names of all ancestors by tracking from the target file to the root
    file_node *file_ptr = file; 
    while (file_ptr->parent != NULL)
    {
        ancestors_names[++ancestor_names_tail] = file_ptr->file_name;
        file_ptr = file_ptr->parent;
    }
    // concatenate all ancestor names to form a full path
    int dest_str_pos = 0;
    dest_str[dest_str_pos++] = '/';
    for (int i = ancestor_names_tail; i >= 0; i--)
    {
        char *ancestor_filename = ancestors_names[i];
        int j = 0;
        while (ancestor_filename[j] != '\0')
        {
            dest_str[dest_str_pos++] = ancestor_filename[j];
            j++;
        }
        if (i > 0)
        {
            dest_str[dest_str_pos++] = '/';
        }
    }
    dest_str[dest_str_pos++] = '\0';
}

