#ifndef H_LOGGER
#define H_LOGGER

// headers
#include<stdio.h>
#include<Windows.h>

//log file
extern FILE* gLogfile;
void openFileLogger();
void logStaticData(const char data[]);
void closeFileLogger();
#endif // !H_LOGGER
