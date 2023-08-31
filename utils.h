#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdio.h>

/**
 * @brief Get the substring of a given string. Range is [start, end)
 * 
 * @param dest destination space to store the substring
 * @param src the reference of the original string
 * @param start starting position
 * @param end ending position. The character at the position end is not included 
 */
void get_substr(char *dest, char *src, int start, int end);

#endif