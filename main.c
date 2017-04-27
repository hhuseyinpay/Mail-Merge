//
// Created by Hasan Hüseyin Pay on 16/04/2017.
//
#include <unistd.h>

#include "dbg.h"
#include "parser.h"
#include "mail_merge.h"

#define EXTENTION "xml"
int main(int argc, char *argv[])
{
    if (argc != 7) err_msg(USAGE);

    char *template;
    char *variables;
    char *outputs;

    // getting arguments from argv
    template = arg_parse(argc, argv, "-t");
    variables = arg_parse(argc, argv, "-v");
    outputs = arg_parse(argc, argv, "-o");

    debug("argparse OK");

    // checking files and file extentions

    if (ext_parse(template, EXTENTION) == 1) {
        if (access(template, R_OK) == -1) {
            log_err("%s can not open for read ", template);
        }
    } else err_msg(USAGE);

    debug("teplate parse OK");

    if (ext_parse(variables, EXTENTION) == 1) {
        if (access(variables, R_OK) == -1) {
            log_err("%s can not open for read", variables);
        }
    } else err_msg(USAGE);

    debug("variables parse OK");

    mail_merge(template, variables, outputs);

    return 0;
}
