#ifndef __FILE_HPP_
#define __FILE_HPP_
#include<cstdint>

#include "DataTypes.h"
#include "SuperBlock.hpp"
#include "BlockIO.hpp"
#include "UInt8Array.hpp"
#include "Block.hpp"
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
		Inode& inode;
		uint16_t id;
		uint16_t block_id;
		struct ptr {
			uint16_t offset;
			Block block; 
			uint16_t getId(){
				return ((uint16_t)block.getData().get(offset)<<8 ) |
							block.getData().get(offset+1) ;
			}
		}p_arr[3];
		uint8_t ptr_id; 
		void update(){
			if(id >= inode.data.block_count) return ;
			uint16_t per_ptr_count = inode.super.block_size / 2;
			if(id < inode.data.direct_count) {
				block_id = inode.data.block[id];
			} else if(id - inode.data.direct_count < per_ptr_count){
				ptr_id = 0;
				p_arr[ptr_id].block = Block(inode.data.block[6], inode.super);
				p_arr[ptr_id].offset = (id - inode.data.direct_count) * 2;
				block_id = p_arr[ptr_id].getId();
			} else {
				uint16_t x = id - inode.data.direct_count - per_ptr_count;
				ptr_id = 0;
				p_arr[ptr_id].block = Block(inode.data.block[7], inode.super);
				p_arr[ptr_id].offset = x / per_ptr_count;
				++ptr_id;
				p_arr[ptr_id].block = Block(p_arr[ptr_id-1].getId(), inode.super);
				p_arr[ptr_id].offset = x % per_ptr_count;
				
				block_id = p_arr[ptr_id].getId();
			}
		}
		void nxt(){
			++id; if(id == inode.data.block_count) return ;
			if(id < inode.data.direct_count) {
				block_id = inode.data.block[id];
			} else if(id == inode.data.direct_count) {
				ptr_id = 0;
				p_arr[ptr_id].block = Block(inode.data.block[6], inode.super);
				p_arr[ptr_id].offset = (id - inode.data.direct_count) * 2;
				block_id = p_arr[ptr_id].getId();
			} else if(p_arr[ptr_id].offset + 2 <inode.super.block_size){
				p_arr[ptr_id].offset += 2;
				block_id = p_arr[ptr_id].getId();
			} else {
				if(ptr_id == 0){
					uint16_t per_ptr_count = inode.super.block_size / 2;
					uint16_t x = id - inode.data.direct_count - per_ptr_count;
					ptr_id = 0;
					p_arr[ptr_id].block = Block(inode.data.block[7], inode.super);
					p_arr[ptr_id].offset = x / per_ptr_count;
					++ptr_id;
					p_arr[ptr_id].block = Block(p_arr[ptr_id-1].getId(), inode.super);
					p_arr[ptr_id].offset = x % per_ptr_count;
					
					block_id = p_arr[ptr_id].getId();
				} else {
					ptr_id--;
					p_arr[ptr_id].offset += 2;
					++ptr_id;
					p_arr[ptr_id].block = Block(p_arr[ptr_id-1].getId(), inode.super);
					p_arr[ptr_id].offset = 0;

					block_id = p_arr[ptr_id].getId();
				}
			}
		}
		void pre(){
			--id; 
			if(id < inode.data.direct_count) {
				block_id = inode.data.block[id];
			} else if(id == inode.data.direct_count) {
				ptr_id = 0;
				p_arr[ptr_id].block = Block(inode.data.block[6], inode.super);
				p_arr[ptr_id].offset = (id - inode.data.direct_count) * 2;
				block_id = p_arr[ptr_id].getId();
			} else if(p_arr[ptr_id].offset - 2 > 0){
				p_arr[ptr_id].offset -= 2;
				block_id = p_arr[ptr_id].getId();
			} else {
				if(p_arr[0].offset -2 < 0){
					ptr_id = 0;
					p_arr[ptr_id].block = Block(inode.data.block[6], inode.super);
					p_arr[ptr_id].offset = (id - inode.data.direct_count) * 2;
					block_id = p_arr[ptr_id].getId();
				} else {
					ptr_id--;
					p_arr[ptr_id].offset -= 2;
					++ptr_id;
					p_arr[ptr_id].block = Block(p_arr[ptr_id-1].getId(), inode.super);
					p_arr[ptr_id].offset = 0;

					block_id = p_arr[ptr_id].getId();
				}
			}
		}
		
	public:
		iterator(Inode& inod,SuperBlockData& s,uint16_t x = 0)
			: inode(inod),id(x){ 
			update();
		}
		iterator& operator--() {
			pre();
			return *this;
		}
		iterator& operator++() {
			nxt();
			return *this;
		}
		bool operator==(const iterator &arg) const {
			return arg.id == id;
		}
		bool operator!=(const iterator &arg) const {
			return arg.id != id;
		}
	};

	iterator begin(){
		return iterator(inode ,super,0);
	}
	iterator end(){
		return iterator(inode ,super, inode.data.block_count);
	}
};

#endif