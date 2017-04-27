//
// Created by Hasan Hüseyin Pay on 16/04/2017.
//

#ifndef DBG_H
#define DBG_H

#endif //DBG_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "[DEBUG] %s:%d: " M "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) {fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n",\
                                 __FILENAME__, __LINE__, clean_errno(), ##__VA_ARGS__); exit(EXIT_FAILURE);}
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define USAGE "Usage: ./mm [-t <template>] [-v <variables>] [-o <outputs>]\n"

#define err_msg(msg, ...) {fprintf(stderr, "[ERROR] " msg "\n",##__VA_ARGS__); exit(EXIT_FAILURE);}