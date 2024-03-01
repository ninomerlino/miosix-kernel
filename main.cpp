#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include "miosix.h"

using namespace std;
using namespace miosix;

const char* filename = "test_file";


int read_test(){
    cout << "Opening file:" << filename << endl;
    ifstream input_file = ifstream(filename);
    string line;
    int position = 1;
    if (!input_file.is_open()){
        cout << "Cannot open file" << endl;
        return -1;
    }
    cout << "Reading file line by line" << endl;
    while ( getline( input_file, line )) {
        cout << "[" << position << "]:" << line << endl;
        position += 1;
    }
    cout << "Closing file" << endl;
    input_file.close();
    cout << "File closed" << endl;
    return 0;
}

int write_test(){
    cout << "Opening file:" << filename << endl;
    ofstream output_file = ofstream(filename);
    if (!output_file.is_open()){
        cout << "Cannot open file" << endl;
        return -1;
    }
    cout << "Writing file line by line" << endl;
    for(int i = 1; i < 11; i++){
        output_file << "This is line" << i << endl;
        cout << "Writing line " << i << "" << endl;
    }
    cout << "Eof reached closing file" << endl;
    output_file.close();
    cout << "File closed" << endl;
    return 0;
}

int main(int argc, char* argv[])
{
    if(write_test() == 0){
        return read_test();
    }
}
