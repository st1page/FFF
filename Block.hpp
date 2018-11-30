#ifndef __BLOCK_HPP_
#define __BLOCK_HPP_
#include <cstdint>
#include "Disk.hpp"
#include "UInt8Array.hpp"

#include<iostream>
using namespace std;	

class Block{
private:
	uint16_t size;
	uint16_t offset;
	Disk disk;
public:
	UInt8Array data;
	Block(uint16_t bsize,uint16_t boffset){
		size = bsize;
		offset = boffset;
		data = UInt8Array(size);
		uint16_t sector_size = disk.getSectorSize();
		uint16_t cur_sector_id = offset / sector_size;
		UInt8Array cur_sector = disk.read(cur_sector_id);
		int j = offset % sector_size; 
		for(int i=0;i<size;i++,j++){
			if(j==sector_size) {
				j=0;
				cur_sector_id++;
				cur_sector = disk.read(cur_sector_id);
			}
			data.set(i, cur_sector.get(j));
		}
	}
	void write(){
		uint16_t sector_size = disk.getSectorSize();
		uint16_t cur_sector_id = offset / sector_size;

		UInt8Array cur_sector = disk.read(cur_sector_id);
		int j = offset % sector_size; 
		for(int i=0;i<size;i++,j++){
			if(j==sector_size) {
				disk.write(cur_sector_id, cur_sector);
				j=0;
				cur_sector_id++;	
			}
			cur_sector.set(j, data.get(i));
		}
		disk.write(cur_sector_id, cur_sector);
	}
	template <typename T>
	T toStruct(){
		if(sizeof(T)!=data.getSize()) throw "toStruct different size";
		T res;
		data.loadToStruct(&res);
		return res;
	}

	template <typename T>
	void fromStruct(T x){

//		cout<<sizeof(T)<<" "<<endl;
//		cout<<data.getSize()<<endl;
		if(sizeof(T)!=data.getSize()) {
			printf("Wrong\n");
			throw "fromStruct different size";
		}
		data.loadFromStruct(&x);
		return ;
	}
};
#endif