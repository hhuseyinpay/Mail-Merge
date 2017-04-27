//
// Created by Hasan Hüseyin Pay on 18/04/2017.
//
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <libxml/tree.h>
#include "dbg.h"
#include "mail_merge.h"

size_t buf_size = 512;
char *buffer;

/*
 * add_buffer() adds up string to buffer which is defined globally.
 * */
void add_buffer(char *string)
{
    if (string == NULL)
        return;

    size_t str_len = strlen(string);
    size_t buf_len = strlen(buffer);

    /* if buffer and string length larger then buf_size, buf_size will double*/
    if (buf_size < (buf_len + str_len)) {
        buf_size *= 2;
        buffer = realloc(buffer, sizeof(char) * buf_size);
    }
    strncpy((buffer + buf_len), string, str_len);
}

/*
 * variable_parser() parse variable's tree find necessary variables
 * by given tag_name within the function
 * */
char *variable_parser(xmlNode *var_root, const char *tag_name)
{
    xmlNode *cur_node = NULL;

    for (cur_node = var_root->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (strcmp((char *) cur_node->name, tag_name) == 0) {
                return cur_node->children == NULL ? NULL : (char *) cur_node->children->content;
            }
        }
    }

    xmlChar *prop = xmlGetProp(var_root, BAD_CAST tag_name);
    if (prop != NULL) {
        return (char *) prop;
    }
    debug("%s", prop);
    return NULL;
}

/*
 * template_parser matches template tags with variable tags by using roots of them
 * */
int template_parser(xmlNode *tmp_root, xmlNode *var_root)
{
    xmlNode *cur_node = NULL;
    char *tmp = NULL;
    for (cur_node = tmp_root->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            tmp = variable_parser(var_root, (char *) cur_node->name);
            debug("template_parser OK \ttmp: %s", tmp);

            if (tmp == NULL)
                return 0;
            add_buffer(tmp);

        } else if (cur_node->type == XML_TEXT_NODE) {
            add_buffer((char *) cur_node->content);
        }
    }

    return 1;
}

/*
 * _mkdir()  recursively create directory.
 */
int _mkdir(const char *dir)
{
    /* this fuction taken from
     * http://nion.modprobe.de/blog/archives/357-Recursive-directory-creation.html
     */
    char tmp[256];
    char *p = NULL;
    size_t len;
    errno = 0;

    snprintf(tmp, sizeof(tmp), "%s", dir);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++)
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    mkdir(tmp, S_IRWXU);

    /* in success case,return 0*/
    if (errno == 0 || errno == EEXIST)
        return 0;

    return 1;
}

/* writer() writes buffer to file if file does not exist it call _mkdir() and
*create a file
*/
void writer(const char *filename)
{
    if (filename == NULL)
        return;

    printf("%s\n", buffer);

    size_t len = strlen(filename);
    char f[len + 4];
    strcpy(f, filename);
    strcat(f, ".txt");

    FILE *fd = fopen(f, "w");
    fprintf(fd, "%s", buffer);
    debug("writer OK");
}


char *mail_merge(const char *template, const char *variables, const char *outputs)
{
    debug("into mail_merge");
    xmlDoc *tmp_doc = NULL;
    xmlDoc *var_doc = NULL;
    tmp_doc = xmlReadFile(template, NULL, 0);
    var_doc = xmlReadFile(variables, NULL, 0);

    if (tmp_doc == NULL || var_doc == NULL) {
        printf("error: could not parse .xml files\n");
        exit(EXIT_FAILURE);
    }

    /* Get the root element node*/
    xmlNode *tmp_root = xmlDocGetRootElement(tmp_doc);
    xmlNode *var_root = xmlDocGetRootElement(var_doc);

    buffer = (char *) calloc(buf_size, sizeof(char));
    char *filename = NULL;
    xmlNode *cur_node = NULL;

    /* create directory*/
    if (_mkdir(outputs) == 1) log_err("outputs directory can not open");

    chdir(outputs);

    /* rootun bir altındaki çocukları dolacak, idlerine göre dosya ismini
     * alacak
     * */
    for (cur_node = var_root->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {

            filename = (char *) xmlGetProp(cur_node, BAD_CAST "id");
            if (template_parser(tmp_root, cur_node) == 1)
                writer(filename);

            debug("get id OK");

            // clean up
            filename = NULL;
            memset(buffer, 0, strlen(buffer));
        }
    }
    debug("mail_merge OK");


    /*free the document*/
    xmlFreeDoc(tmp_doc);
    xmlFreeDoc(var_doc);
    return buffer;
}