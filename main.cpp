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

void deviceTest();
void mkfsTest();

const char* filename = "/sd/test.txt";

int main(int argc, char* argv[])
{
    deviceTest();
    mkfsTest();
    return 1;
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
    unsigned int readSize, eraseSize, writeSize;
    unsigned long long deviceSize;
    ioctl(fileDescriptor, IOCTL_GET_READ_SIZE, &readSize);
    ioctl(fileDescriptor, IOCTL_GET_VOLUME_SIZE, &deviceSize);
    ioctl(fileDescriptor, IOCTL_GET_ERASE_SIZE, &eraseSize);
    ioctl(fileDescriptor, IOCTL_GET_WRITE_SIZE, &writeSize);

    printf("SDIODriver::ioctl() - Card size: %lluKB, %lluGB\n",deviceSize/1024, deviceSize/(1024*1024*1024));
    printf("SDIODriver::ioctl() - Erase size: %dB\n",eraseSize);
    printf("SDIODriver::ioctl() - Write count: %dB\n",writeSize);
    printf("SDIODriver::ioctl() - Read size: %dB\n",readSize);           
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