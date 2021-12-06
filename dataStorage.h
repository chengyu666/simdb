#include "global.h"
using namespace std;

#ifndef DS_H
#define DS_H
class DSMgr
{
public:
    DSMgr();
    int OpenFile(string filename);
    int CloseFile();
    bFrame ReadPage(int page_id);
    int WritePage(int page_id, bFrame frm);
    //int Seek(int offset, int pos); nouse
    fstream GetFile(); //nouse?
    void IncNumPages();
    int GetNumPages();
    void SetUse(int page_id, bool use_bit);
    bool GetUse(int page_id);
    int GetDiskRead();
    int GetDiskWrite();
    int diskRead;  //read num
    int diskWrite; //write num

private:
    int numPages;
    bool pageused[MAXPAGES]; //record if a page is used
    fstream dbfile;          //physic file pointer
};
#endif