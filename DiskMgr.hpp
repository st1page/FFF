#ifndef __DISKMGR_HPP_
#define __DISKMGR_HPP_
#include<cstdint>

#include "DataTypes.h"
#include "Disk.hpp"
#include "Block.hpp"


class DiskMgr{
private:
	Disk disk;
	SuperBlock super;
public:
	DiskMgr(){

	}
	~DiskMgr(){

	}
	void initFormat(){
		formatSuperBlock();
		formatmap();
	}
	
	SuperBlock getSuperBlock() const{
		return super;
	}
private:
	void formatSuperBlock(){
		super.format();
		Block super_block = Block(1024,0);
		super_block.fromStruct(super);
		super_block.write();
	}
	void formatmap(){
		uint16_t size = super.block_size;
		uint32_t offset = super.first_inodes_bitmap * size;
		Block inode_map = Block(size*super.inodes_count, offset );
		inode_map.data.setAll(0);
		inode_map.write();

		offset = super.first_blocks_bitmap * size;
		Block block_map = Block(size*super.inodes_count, offset );
		block_map.data.setAll(0);
		block_map.write();
	}
};

#endif