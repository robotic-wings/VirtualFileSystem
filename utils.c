void get_substr(char *dest, char *src, int start, int end)
{
    int dest_pos = 0;
    for (int i = start; i < end; i++)
    {
        dest_pos = i - start;
        dest[dest_pos] = src[i];
    }
    dest[++dest_pos] = '\0';
}
