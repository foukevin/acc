#ifndef ERROR_H
#define ERROR_H

void error(const char *, ...);
void warning(const char *, ...);
void die(const char *, ...);

void setprogname(const char *);
#define ERROR_INIT \
	do { \
		if (argv[0] != NULL) \
			setprogname(argv[0]); \
	} while(0)


#endif /* ERROR_H */
