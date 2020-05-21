#include <iostream>
#include <fstream>
#include <string>
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
    
    ifstream file(argv[1], ios::in | ios::binary);
    if (file.is_open())
    {
        
        file.seekg(2, ios::beg); // skip fist two bytes which describe header::vermagic
        uint16_t entnum = 0; 
        file.read((char*)&entnum, 2); // read 2 bytes to header::entnum 
        unsigned* fsize = new unsigned[entnum];
        unsigned* foffset = new unsigned[entnum];
        string dirname("");
        if (argc == 3)
        {
            dirname = argv[2];
            if (dirname.find('/', dirname.size()-1) == string::npos)
            {
                dirname += "/"; // add "/" in the end of directory name if needed
            }
            mkdir(dirname.c_str(), 0777); // create defined directory if it doesn`t exist
        }
        //read FENTRY TABLEs for each file
        for (unsigned i = 0; i < entnum; ++i)
        {
            char buff[4];
            file.read(buff, 3); // read 3 bytes that describe fentry_table::fsize
            unsigned* buffp = reinterpret_cast<unsigned*>(buff);
            fsize[i] = *buffp;
            fsize[i] = (*buffp & 0x00FFFFFF); // erase 4th byte
            file.read(buff, 4); // read 4 bytes that describe fentry_table::foffset
            foffset[i] = *buffp;
        }
        // read each FILE NODES and create the files
        for (unsigned i = 0; i < entnum; ++i)
        {
            file.seekg(foffset[i]+fsize[i], ios::beg); //set get pointer to start of file path
            string filename;
            getline(file, filename, '\0'); // read file path
            if (dirname.size())
            {
                filename = dirname + filename; // change file name if destination directory entered
            }
            if (filename.find('/') != string::npos)
            {
                mkdir(filename.substr(0, filename.find_last_of("/")).c_str(),0777); // create subdirectory is needed
            }
            ofstream newfile(filename, ios::out | ios::binary);
            if (newfile.is_open())
            {
                file.seekg(foffset[i],ios::beg); // set get pointer to start of file data
                string buff(fsize[i],'\0');
                file.read((char*)buff.c_str(), fsize[i]); 
                newfile.write((char*)buff.c_str(), fsize[i]);
                newfile.close();
            }
        }
        delete[] fsize;
        delete[] foffset;
        file.close();
    }
    else 
    {
        cerr << "File read error."<<endl;
        exit(1);
    }
    return 0;
}