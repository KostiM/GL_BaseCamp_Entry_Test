#include <iostream>
#include <string> // stoi()
using namespace std;
int main(int argc, char* argv[])
{
    if (argv[1] && argv[2]) 
    {
        int N = 0, 
            K = 0;
        try 
        {
            N = stoi(argv[1]),
            K = stoi(argv[2]);
        }
        catch (std::exception ex) 
        {
            cout << "Incorrect parameter(s). " << ex.what() << endl;
            return 1;
        }
        if (N<1||K<0)
        {
            cout << "Incorrect parameter(s)." << endl;
            return 1;
        }
        int res = N / ++K; // the giant can step k+1 steps by one move
        if (N % K) // if there are additional steps left
        {
            ++res;
        }
        cout << res << endl;
    }
    else 
    {
        cout << "Program should be run with command line parameters." << endl;
        return 1;
    }
    return 0;
}