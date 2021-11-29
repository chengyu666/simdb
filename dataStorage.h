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
    int WritePage(int frame_id, bFrame frm);
    //int Seek(int offset, int pos); nouse
    fstream GetFile(); //nouse?
    void IncNumPages();
    int GetNumPages();
    void SetUse(int page_id, bool use_bit);
    bool GetUse(int page_id);

private:
    int numPages;
    bool pageused[MAXPAGES]; //record if a page is used
    int diskRead;            //read num
    int diskWrite;           //write num
    fstream dbfile;          //physic file pointer
};
#endif