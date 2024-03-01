#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include "miosix.h"

using namespace std;
using namespace miosix;

const char* filename = "/sd/test.txt";

int main(int argc, char* argv[])
{
    printf("Testing file system\n");
    ofstream output_file = ofstream(filename);
    if(output_file.is_open()){
        printf("File system is working\n");
    }else{
        printf("File system is not working\n");
        return -1;
    }

    printf("Writing test\n");
    for(int i = 1; i < 11; i++){
        output_file << "This is line " << i << endl;
        printf("Writing line %d\n", i);
    }
    output_file.close();
    printf("File closed\n");

    return 1;
}
