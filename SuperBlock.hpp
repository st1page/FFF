#ifndef __SUPERBLOCK_HPP_
#define __SUPERBLOCK_HPP_
#include<cstdint>

#include "BlockTypes.h"
#include "BlockIO.hpp"

#include<iostream>
#define _debug(x) std::cout<<#x<<" "<<x<<std::endl;

class SuperBlock{
private:
	BlockIO io;
public:
	SuperBlockData data;
	SuperBlock() {
		io = BlockIO(1024,0);
		read();
	}
	void write(){
		io.fromStruct(data);
		io.write();
	}
	void read(){
		io.read();
		io.toStruct<SuperBlockData>(data);
	}
	void format(){
		data.first_inodes_bitmap = INODEMAPSTART;
		data.inodes_bitmap_count = INODEMAPCOUNT;
		data.first_blocks_bitmap = BLOCKMAPSTART;
		data.blocks_bitmap_count = BLOCKMAPCOUNT;
		data.first_inode = INODESTART;
		data.inodes_count = INODECOUNT;     
		data.inode_size = INODESIZE;     
		data.block_size = BLOCKSIZE;

		data.blocks_count = BLOCKCOUNT;      
		data.free_blocks_count = BLOCKCOUNT; 
		data.free_inodes_count = INODECOUNT; 
		data.first_data_block = BLOCKSTART;  

		data.magic = MAGIC;
		data.dirs_count = 0;
	}
	void debug(){
		std::cout<<"debug SuperBlock"<<std::endl;
		_debug(data.first_inodes_bitmap );
		_debug(data.inodes_bitmap_count );
		_debug(data.first_blocks_bitmap );
		_debug(data.blocks_bitmap_count );
		_debug(data.first_inode );
		_debug(data.inodes_count );     
		_debug(data.inode_size );     
		_debug(data.block_size );

		_debug(data.blocks_count );      
		_debug(data.free_blocks_count ); 
		_debug(data.free_inodes_count ); 
		_debug(data.first_data_block );  

		_debug(data.magic );
		_debug(data.dirs_count );
		std::cout<<"over"<<std::endl;
	}
};

#endif