#ifndef __BLOCK_HPP_
#define __BLOCK_HPP_

#include <cstdint>

#include "DataTypes.h"
#include "SuperBlock.hpp"
#include "BlockIO.hpp"
#include "UInt8Array.hpp"

class Block{
protected:
	BlockIO io;
public:	
	Block(){}
	Block(uint16_t id, const SuperBlockData& super) {
		uint16_t size = super.block_size;
		uint32_t offset = id * size;
		offset += super.first_data_block*size;
		io = BlockIO(size, offset);
		read();
	}
	virtual void write(){
		io.write();
	}
	virtual void read(){
		io.read();
	}
	UInt8Array& getData(){
		return io.data;
	}
	void setData(UInt8Array& data){
		io.data = data;
	}
};
#endif