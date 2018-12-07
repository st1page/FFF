#ifndef __INODE_HPP_
#define __INODE_HPP_
#include<cstdint>

#include "BlockTypes.h"
#include "BlockIO.hpp"
#include<iostream>
#define _debug(x) std::cout<<#x<<" "<<x<<std::endl;

class Inode{
private:
	BlockIO io;
public:
	uint16_t id;
	InodeData data;
	Inode(uint16_t id, const SuperBlockData& super) {
		uint16_t size = super.inode_size;
		uint32_t offset = id * super.inode_size;
		offset += super.first_inode*super.block_size;
		io = BlockIO(size, offset);
		read();
	}
	void write(){
		io.fromStruct(data);
		io.write();
	}
	void read(){
		io.read();
		io.toStruct<InodeData>(data);
	}
	void debug(){
		_debug(data.uid);
		_debug(data.links_count);
		_debug(data.mode);
		_debug(data.size);
		_debug(data.atime);
		_debug(data.ctime);
		_debug(data.mtime);
		for(int i=0;i<8;i++) _debug(data.block[i]);
	}
};

#endif