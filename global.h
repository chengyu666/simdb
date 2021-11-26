#ifndef __DEFINES__
#define __DEFINES__

#define MAXPAGES 50000 //TODO
#define BUF_SIZE 1024  //size of a buffer size
#define FRAME_NUM BUF_SIZE
#define FRAME_SIZE 1024  //TODO change to size of BCB
#define DISK_DELAY false //whether to add latency on disk operations

#endif

#ifndef __VARS__
#define __VARS__

int diskRead = 0;
int diskWrite = 0;

#endif

#ifndef __DEP__
#define __DEP__

#include <string>
#include <iostream>

#endif

#ifndef __TOOLS__
#define __TOOLS__
#endif