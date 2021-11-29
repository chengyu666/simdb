#include "dataStorage.h"
using namespace std;

DSMgr::DSMgr()
{
    diskRead = 0;
    diskWrite = 0;
    return;
}

int DSMgr::OpenFile(string filename)
{
    dbfile.open(filename, ios::in | ios::out);
    if (dbfile.is_open())
    {
        log(0, "data.dbf opened.");
        return 0;
    }
    else
    {
        log(1, "data.dbf open failed.");
        exit(-1);
    }
}

int DSMgr::CloseFile()
{
    dbfile.close();
    log(0, "data.dbf closed.");
    return 0;
}

//并没有真正读取文件
void DSMgr::ReadPage(int page_id)
{
    diskRead++;
    if (DISK_DELAY > 0)
    {
        this_thread::sleep_for(chrono::milliseconds(DISK_DELAY));
    }
}

//并没有真正写入文件
void DSMgr::WritePage(int page_id)
{
    diskWrite++;
    if (DISK_DELAY > 0)
    {
        this_thread::sleep_for(chrono::milliseconds(DISK_DELAY));
    }
}