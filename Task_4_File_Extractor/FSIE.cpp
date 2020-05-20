#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) 
    {
        cerr << "The program should be run with command line parameter." << endl;
        exit(1);
    }
    uint16_t entnum = 0;
    ifstream file(argv[1], ios::in | ios::binary);
    if (file.is_open())
    {
        file.seekg(2, ios::beg);
        file.read((char*)&entnum, 2);
        entnum = entnum & 0x0000FFFF; // ignore 3rd & 4th byte
        vector<unsigned> fsize(entnum);
        vector<unsigned> foffset(entnum);
        for (unsigned i = 0; i < entnum; ++i)
        {
            char buff[4];
            file.read(buff, 3);
            unsigned* buffp = reinterpret_cast<unsigned*>(buff);
            fsize[i] = *buffp;
            fsize[i] = (unsigned)(fsize[i] & 0x00FFFFFF); // ignore 4th byte
            file.read(buff, 4);
            foffset[i] = *buffp;
        }
        for (unsigned i = 0; i < entnum; ++i)
        {
            file.seekg(foffset[i]+fsize[i], ios::beg);
            string filename;
            getline(file, filename, '\0');
            if (argc == 3) 
            {
                filename = argv[2] + filename;
            }
            if (filename.find('/') != string::npos)
            {
                mkdir(filename.substr(0, filename.find_last_of("/")).c_str(),0777);
            }
            
            ofstream newfile(filename, ios::out | ios::binary);
            if (newfile.is_open())
            {
                file.seekg(foffset[i],ios::beg);
                string buff(fsize[i],'\0');
                file.read((char*)buff.c_str(), fsize[i]);
                newfile.write((char*)buff.c_str(), fsize[i]);
                newfile.close();
            }
        }
        file.close();
    }
    else 
    {
        cerr << "File read error."<<endl;
        exit(1);
    }
    return 0;
}