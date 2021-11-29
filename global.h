#ifndef __DEFINES__
#define __DEFINES__
#define MAXPAGES 50000 //TODO
#define BUF_SIZE 1024  //size of a buffer size
#define FRAME_NUM BUF_SIZE
#define FRAME_SIZE 1024 //TODO change to size of BCB
#define DISK_DELAY 0    //add latency on disk operations(ms)
#endif

#ifndef __DEP__
#define __DEP__
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#endif

#ifndef __TOOLS__
#define __TOOLS__
using namespace std;
static void log(int level, string msg)
{
    switch (level)
    {
    case 0:
        cout << "[info] ";
        break;
    case 1:
        cout << "[error] ";
        break;
    default:
        cout << "[unknown] ";
        break;
    }
    cout << msg << endl;
}
#endif