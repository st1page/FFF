#ifndef __DISK_HPP_
#define __DISK_HPP_
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <unistd.h>

#include "UInt8Array.hpp"
#include <iostream>
using namespace std;
class Disk{
private:
	const char *PATH = "data";
	uint16_t size;
	uint16_t num;
public:
	Disk(){
		chdir(PATH);
		FILE *fp = fopen("property", "r");
		fscanf(fp,"%hu%hu",&size,&num);
		std::cout<<size<<" "<<num<<std::endl;
		fclose(fp);
	}
	uint16_t getSectorSize(){
		return size;
	}
	UInt8Array read(uint16_t sector){
		char filename[20];
		memset(filename,0,sizeof(filename));
		sprintf(filename, "%d", sector);
		strcat(filename, ".sector");
		FILE *fp = fopen(filename, "r");

		UInt8Array res = UInt8Array(size);
		for(int i=0;i<size;i++) {
			uint8_t x;
			fscanf(fp,"%hhx",&x);
			res.set(i,x);
		}
		fclose(fp);
		return res;
	}
	void write(uint16_t sector,UInt8Array data){
		char filename[20];
		memset(filename,0,sizeof(filename));
		sprintf(filename, "%d", sector);
		strcat(filename, ".sector");
		FILE *fp = fopen(filename, "w");
		for(int i=0;i<size;i++) 
			fprintf(fp,"%-3hhx",data.get(i));
		fclose(fp);
	}
};
#endif

