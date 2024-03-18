#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/ioctl.h>
#include "miosix/filesystem/ioctl.h"
#include "miosix.h"

using namespace std;
using namespace miosix;

void fileSystemTest();
void printCharAsBinary(char c);
void deviceTest();
int *resultToInt(char *c, int size);

const char* filename = "/sd/test.txt";

int main(int argc, char* argv[])
{
    //fileSystemTest();
    deviceTest();    
    return 1;
}

void fileSystemTest(){
    printf("Testing file system\n");
    ofstream output_file = ofstream(filename);
    if(output_file.is_open()){
        printf("File system is working\n");
    }else{
        printf("File system is not working\n");
        return;
    }

    printf("Writing test\n");
    for(int i = 1; i < 11; i++){
        output_file << "This is line " << i << endl;
        printf("Writing line %d\n", i);
    }
    output_file.close();
    printf("File closed\n");
}

void printCharAsBinary(char c){
    for(int i = 0; i < 8; i++){
        printf("%d", (c << i) & 128? 1 : 0);
    }
    printf(" ");
}

int *resultToInt(char *c, int size){
    int *result = new int[size*8];
    for(int i = 0; i < size; i++){
        for(int j = 0; j < 8; j++){
            result[i*8 + j] = (c[i] << j) & 128? 1 : 0;
        }
    }
    return result;
}

void deviceTest(){
    FILE* deviceSd = fopen("/dev/sda", "r");

    if(deviceSd == NULL){
        printf("Device not found\n");
    } else {
        printf("Device found\n");
    }

    int fileDescriptor = fileno(deviceSd);

    printf("File descriptor: %d\n", fileDescriptor);

    //Get all info
    unsigned int blockSize, deviceSize, sectorSize, sectorCount;
    ioctl(fileDescriptor, IOCTL_GET_BLOCK_SIZE, &blockSize);
    ioctl(fileDescriptor, IOCTL_GET_DEVICE_SIZE, &deviceSize);
    ioctl(fileDescriptor, IOCTL_GET_SECTOR_SIZE, &sectorSize);
    ioctl(fileDescriptor, IOCTL_GET_SECTOR_COUNT, &sectorCount);

    printf("SDIODriver::ioctl() - Card size: %dKB\n",deviceSize);
    printf("SDIODriver::ioctl() - Sector size: %dKB\n",sectorSize);
    printf("SDIODriver::ioctl() - Sector count: %d\n",sectorCount);
    printf("SDIODriver::ioctl() - Block size: %d\n",blockSize);
            
}