#ifndef _ORANGES_LOG_H_
#define _ORANGES_LOG_H_

void sprintf(const char *fmt, ...);

void spanic(const char *file, int line, const char *func, const char *cond);

#define STR_DEFAULT_LEN 1024

#endif /* _ORANGES_LOG_H_ */
