#ifndef __FILE_HPP_
#define __FILE_HPP_
#include<cstdint>

#include "DataTypes.h"
#include "SuperBlock.hpp"
#include "BlockIO.hpp"
#include "UInt8Array.hpp"
#include "Allocator.hpp"
#include "Inode.hpp"

class File{
protected:
	SuperBlockData& super;
	Inode inode;
public:
	File(uint16_t inode_id, SuperBlockData& s)
		:super(s),inode(Inode(inode_id, s)) {
	}
	class iterator {
	private:
		uint16_t offset_2st, offset_3st;
		BlockIO io_1st,io_2st,io_3st;
		void updateIO(){

		}
		void nxtIO(){

		}
		void preIO(){

		}
	public:
		Inode& inode;
		uint16_t block_id;
		iterator(Inode& inod,SuperBlockData& s,uint16_t x = 0)
			: inode(inod),block_id(x){ 
			updateIO();
		}
		iterator& operator--() {
			--block_id; preIO();
			return *this;
		}
		iterator& operator++() {
			++block_id; nxtIO();
			return *this;
		}
		bool operator==(const iterator &arg) const {
			return arg.block_id == block_id;
		}
		bool operator!=(const iterator &arg) const {
			return arg.block_id == block_id;
		}
	};
};

#endif