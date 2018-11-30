#ifndef __BLOCKTYPES_H_
#define __BLOCKTYPES_H_
#include<cstdint>
struct SuperBlock{
	uint16_t inodes_count;      /* inodes 计数 */
	uint16_t blocks_count;      /* blocks 计数 */
	uint16_t free_blocks_count; /* 空闲的 blocks 计数 */
	uint16_t free_inodes_count; /* 空闲的 inodes 计数 */
	uint16_t first_data_block;  /* 第一个数据 block */
	uint16_t block_size;    /* block 的大小 */
	uint16_t first_inode;         /* First non-reserved inode */
	uint16_t inode_size;        /* size of inode structure */
	uint32_t reserved1[10];
	uint16_t magic;             /* Magic 签名 0xFFFF*/
	uint16_t def_resuid;        /* Default uid for reserved blocks */
	uint32_t reserved2[241];
};
struct Inode{
	uint8_t i_uid;     
	uint8_t i_links_count;          
	uint16_t i_mode;    
	uint32_t i_size;    /* byte */
	uint32_t i_atime;   /* Access time */
	uint32_t i_ctime;   /* Creation time */
	uint32_t i_mtime;   /* Modification time */
	uint16_t i_block[8];  /* 6个直接 1个间接 1个双间接*/
	uint32_t i_reserved[7];         
};
#endif