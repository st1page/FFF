#ifndef __BLOCKTYPES_H_
#define __BLOCKTYPES_H_
#include<cstdint>
#include<cstdio>

#include "default.h"
#include "UInt8Array.hpp"
struct SuperBlockData{
	uint16_t first_inodes_bitmap;
	uint16_t inodes_bitmap_count;
	uint16_t first_blocks_bitmap;
	uint16_t blocks_bitmap_count;
	uint16_t first_inode;
	uint16_t inodes_count;      /* inodes 计数 */
	uint16_t inode_size;        /* size of inode structure */
	uint16_t block_size;    /* block 的大小 */

	uint16_t blocks_count;      /* blocks 计数 */
	uint16_t free_blocks_count; /* 空闲的 blocks 计数 */
	uint16_t free_inodes_count; /* 空闲的 inodes 计数 */
	uint16_t first_data_block;  /* 第一个数据 block */
	 
	uint32_t reserved1[8];

	uint16_t magic;             /* Magic 签名 0xFFFF*/
	uint16_t dirs_count;  

	uint32_t reserved2[241];
};
struct InodeData{
	uint8_t uid;     
	uint8_t links_count;          
	uint16_t mode;    
	uint32_t size;    /* byte */
	uint32_t atime;   /* Access time */
	uint32_t ctime;   /* Creation time */
	uint32_t mtime;   /* Modification time */
	uint16_t block[8] = {0,0,0,0,0,0,0,0};  /* 6个直接 1个间接 1个双间接*/
											/* dir 文件名大于64必存于双间接以上 */
	uint16_t direct_count;
	uint16_t indirect_count;
	uint32_t reserved[6];
	
};
struct DirBlockHeader{
	uint16_t entry_count;
	uint16_t free_size;
};
struct EntryHeader{
	uint16_t inode;
	uint16_t name_len;
	uint16_t offset;
	uint16_t reserved;
};
struct Entry{
	EntryHeader header;
	UInt8Array name;
	Entry(){ }
	Entry(EntryHeader h, UInt8Array n):
		header(h), name(n){
	}
	uint16_t size() const {
		return name.getSize() + sizeof(EntryHeader);
	}
};
#endif