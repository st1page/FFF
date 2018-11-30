#include <cstdio>

#include "default.h"
#include "BlockTypes.h"
#include "Disk.hpp"
#include "Block.hpp"

#include<iostream>
using namespace std;
#define debug(x) cout<<#x<<" "<<x<<endl;

void initSuperBlock(){
	SuperBlock super;
	super.inodes_count = INODECOUNT;
	super.blocks_count = BLOCKCOUNT;
	super.free_blocks_count = BLOCKCOUNT;
	super.free_inodes_count = INODECOUNT;
	super.first_data_block = BLOCKSTART;
	super.first_inode = INODESTART;
	super.block_size = BLOCKSIZE;
	super.inode_size = INODESIZE;
	super.magic = MAGIC;
	super.def_resuid = 0;
	Block super_block = Block(1024,0);
	super_block.fromStruct(super);
	super_block.write();
}
SuperBlock getSuperBlock(){
	Block super_block = Block(1024,0);
	SuperBlock super = super_block.toStruct<SuperBlock>();
	debug(super.inodes_count);
	debug(super.blocks_count);
	debug(super.free_blocks_count);
	debug(super.free_inodes_count);
	debug(super.first_data_block);
	debug(super.block_size);
	debug(super.first_inode);
	debug(super.inode_size);
	debug(super.magic);
	debug(super.def_resuid);
	return super;
}
int main(){
	printf("sizeof(super_block) %5u\n",sizeof(SuperBlock));
	printf("sizeof(inode)       %5u\n",sizeof(Inode));
//	initSuperBlock();
	getSuperBlock();
	return 0;
}