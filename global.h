#ifndef __DEFINES__
#define __DEFINES__
#define MAXPAGES 50000 //TODO
#define BUF_SIZE 1024  //number of buffer blocks
#define PAGE_SIZE 10   //number of bytes in a page
#define FRAME_SIZE 10  //size of a frame, equal to PAGE_SIZE
#define DISK_DELAY 0   //add latency on disk operations(ms)

struct BCB
{
public:
    int page_id; //if is -1, means the frame is unused
    int frame_id;
    bool dirty;
    bool latch;
    int count;
};

struct bFrame
{
    char data[FRAME_SIZE];
};

struct LRUEle
{
    /* data */
    int page_id;
    int frame_id;
    int count;
    LRUEle *next;
};

#endif

#ifndef __DEP__
#define __DEP__
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <cstring>
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