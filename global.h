#ifndef __DEFINES__
#define __DEFINES__
#define MAXPAGES 50000 //max number of pages
#define BUF_SIZE 1024  //number of buffer blocks
#define PAGE_SIZE 10   //number of bytes in a page
#define FRAME_SIZE 10  //size of a frame, equal to PAGE_SIZE
#define DISK_DELAY 1   //add latency on disk operations(ms)
#define PRINTDBG false //print debug messages

struct BCB
{
public:
    int page_id; //if ==-1, means the frame is unused
    int frame_id;
    bool dirty;
    bool latch;
    int count;
    BCB *next;
};

struct bFrame
{
    char data[FRAME_SIZE] = ".........";
};

struct hashitem
{
    int page_id;
    BCB *bcb;
    hashitem *next;
};

#endif

#ifndef __DEP__
#define __DEP__
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <cstring>
#include <time.h>
#endif

#ifndef __TOOLS__
#define __TOOLS__
using namespace std;
static void log(int level, string msg)
{
    if (level == 2 && !PRINTDBG)
        return;
    switch (level)
    {
    case 0:
        cout << "[info] ";
        break;
    case 1:
        cout << "[error] ";
        break;
    case 2:
        cout << "[debug] ";
        break;
    default:
        cout << "[unknown] ";
        break;
    }
    cout << msg << endl;
}
static bool ptrisnull(void *ptr)
{
    return ptr == nullptr;
}
#endif