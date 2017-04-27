//
// Created by Hasan Hüseyin Pay on 16/04/2017.
//

#include <string.h>

/*
 * arg_parse() function parse the vector by token
 */
char *arg_parse(int counter, char **vector, char *token)
{
    for (int i = 1; i < counter; i++)
        if (strcmp(vector[i], token) == 0)
            return (i + 1 < counter) ? vector[i + 1] : NULL; // if counter overflow, return null

    return NULL;
}

/*
 * ext_parse() function parse file extention(filename) by dot(.)
 * and compare token and extention. if file extention equals to token
 * return 1 else return 0
 */

int ext_parse(char *filename, char *token)
{
    if (filename == NULL || token == NULL)
        return 0;

    int len = (int) strlen(filename);
    char *tmp = NULL;

    // tmp points out after dot
    for (int i = len; i > 0; i--) {
        if (filename[i] == '.') {
            tmp = filename + (i + 1);
            break;
        }
    }

    if (tmp == NULL)
        return 0;
    else if (strcmp(tmp, token) == 0) {
        return 1;
    }
    return 0;
}