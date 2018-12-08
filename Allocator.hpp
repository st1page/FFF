#ifndef __ALLOCATOR_HPP_
#define __ALLOCATOR_HPP_

#include <cstdint>

#include "DataTypes.h"
#include "SuperBlock.hpp"
#include "BlockIO.hpp"
#include "UInt8Array.hpp"

class Allocator{
private:
	SuperBlock& super;
	BlockIO inode_map_io;
	BlockIO block_map_io;
public:
	Allocator(SuperBlock& s):super(s) {
		uint16_t inode_map_size = super.data.inodes_count / 8;
		uint16_t block_map_size = super.data.blocks_count / 8;
		
		uint32_t offset = super.data.first_inodes_bitmap * 
										super.data.block_size;
		inode_map_io = BlockIO(inode_map_size, offset);
		
		offset = super.data.first_blocks_bitmap * 
										super.data.block_size;
		block_map_io = BlockIO(block_map_size, offset);
	}
	uint16_t allocInode(){
		if(!super.data.free_inodes_count) return (uint16_t)(-1);
		super.data.free_inodes_count--;
		uint8_t full = 0xff;
		UInt8Array &arr = inode_map_io.data;
		for(uint16_t i=0;i<arr.getSize();i++){
			if(arr.get(i)!=full){
				uint8_t x = arr.get(i);
				uint8_t j = 0;
				uint8_t mask = 1;
				while(x&mask){
					j++; mask<<=1;
				}
				x |= mask;
				arr.set(i,x);
				inode_map_io.write();
				return i*8+j;
			}
		}
	}
	void freeInode(uint16_t id){
		super.data.free_inodes_count++;
		uint16_t i = id / 8;
		uint8_t j = id % 8;
		uint8_t x = inode_map_io.data.get(i);
		x &= ~(1<<j);
		inode_map_io.data.set(i,x);
		inode_map_io.write();
	}
	uint16_t allocBlock(){
		if(!super.data.free_blocks_count) return (uint16_t)(-1);
		super.data.free_blocks_count--;
		uint8_t full = 0xff;
		UInt8Array &arr = block_map_io.data;
		for(uint16_t i=0;i<arr.getSize();i++){
			if(arr.get(i)!=full){
				uint8_t x = arr.get(i);
				uint8_t j = 0;
				uint8_t mask = 1;
				while(x&mask){
					j++; mask<<=1;
				}
				x |= mask;
				arr.set(i,x);
				block_map_io.write();
				return i*8+j;
			}
		}
	}
	void freeBlock(uint16_t id){
		super.data.free_blocks_count++;
		uint16_t i = id / 8;
		uint8_t j = id % 8;
		uint8_t x = block_map_io.data.get(i);
		x &= ~(1<<j);
		block_map_io.data.set(i,x);
		block_map_io.write();
	}
};

#endif
