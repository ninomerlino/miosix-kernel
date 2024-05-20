#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/ioctl.h>
#include "miosix/filesystem/ioctl.h"
#include "miosix.h"
#include "miosix/filesystem/file_access.h"
#include "miosix/filesystem/fat32/fat32.h"

using namespace std;
using namespace miosix;

bool deviceTest();
void mkfsTest();

const char* filename = "/sd/test.txt";

int main(int argc, char* argv[])
{
    if(deviceTest()) mkfsTest();
    return 1;
}

bool deviceTest(){
    FILE* deviceSd = fopen("/dev/sda", "r");

    if(deviceSd == NULL){
        printf("Device not found\n");
        return false;
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

    printf("SDIODriver::ioctl() - Card size: %lluKB, %lluGB\n", deviceSize/1024, deviceSize/(1024*1024*1024));
    printf("SDIODriver::ioctl() - Erase size: %dB\n",eraseSize);
    printf("SDIODriver::ioctl() - Write count: %dB\n",writeSize);
    printf("SDIODriver::ioctl() - Read size: %dB\n",readSize);
    return true;       
}

void mkfsTest(){
    FilesystemManager& fileSystemManager = FilesystemManager::instance();
    const char* deviceName = "/dev/sda";
    StringPart sp(deviceName);

    int result = fileSystemManager.mkfat32(deviceName);

    if(result != 0){
        printf("Error formatting SD card %d\n", result);
        return;
    }

    printf("SD card formatted\n");

    // Mount fs
    intrusive_ref_ptr<FileBase> device;
    intrusive_ref_ptr<DevFs> devFs = fileSystemManager.getDevFs();
    devFs.get()->open(device, sp, O_RDWR, 0);
    if(device.get() == NULL){
        printf("Error opening device\n");
        return;
    }
    Fat32Fs* fat32 = new Fat32Fs(device);
    if(fat32->mountFailed()){
        printf("Error mounting SD card\n");
        return;
    }
    printf("SD card mounted\n");

    // This is useless if at boot there already a fat32 fs
    StringPart sd("sd");
    int mkdirResult = mkdir("/sd", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(mkdirResult != 0){
        printf("Failed to create directory /sd, it may already exist, error %d\n", mkdirResult);
    } else printf("Created /sd folder");
    intrusive_ref_ptr<FilesystemBase> fsRef(fat32);

    // Also this is useless if at boot there already a fat32 fs
    int kmountResult = fileSystemManager.kmount("/sd", fsRef);
    if(kmountResult!=0)
    {
        printf("Failed to mount fat32, error %d\n", kmountResult);
    }else printf("Fat 32 mounted");

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