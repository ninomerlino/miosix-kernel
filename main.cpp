#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/ioctl.h>
#include "miosix/filesystem/ioctl.h"
#include "miosix.h"
#include "miosix/filesystem/file_access.h"

using namespace std;
using namespace miosix;

void printCharAsBinary(char c);
void deviceTest();
void mkfsTest();
int *resultToInt(char *c, int size);

const char* filename = "/sd/test.txt";

int main(int argc, char* argv[])
{
    deviceTest();
    mkfsTest();
    return 1;
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

void mkfsTest(){
    //Format sd card
    int result = miosix::getFileDescriptorTable().mkfs("/sd", 1, 0);
    if(result == 0){
        printf("SD card formatted\n");
    }else{
        printf("Error formatting SD card\n");
    }

    //Create file
    FILE* file = fopen(filename, "w");
    if(file == NULL){
        printf("Error creating file\n");
        return;
    }else{
        printf("File created\n");
    }

    //Write to file
    char text[] = "This is a test";
    int written = fwrite(text, 1, sizeof(text), file);
    if(written == sizeof(text)){
        printf("Text written to file\n");
    }

    //Close file
    fclose(file);
}