#ifndef __INODE_HPP_
#define __INODE_HPP_
#include<cstdint>

#include "DataTypes.h"
#include "BlockIO.hpp"
#include<iostream>
#define _debug(x) std::cout<<#x<<" "<<x<<std::endl;

class Inode{
private:
	BlockIO io;
public:
	SuperBlockData& super;
	uint16_t id;
	InodeData data;
	Inode(SuperBlockData& s): super(s){};
	Inode(uint16_t i, SuperBlockData& s)
		: super(s), id(i){
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
	struct iterator {
		UInt8Array& arr;
		uint16_t id;
		iterator(UInt8Array& a,uint16_t x = 0): arr(a){ 
			id = x;
		}
		EntryHeader getEntryHeader() const {
			uint16_t offset = id*sizeof(EntryHeader) + sizeof(DirBlockHeader);
			EntryHeader res;
			arr.loadToStruct(&res, offset);
			return res;
		}
		void setEntryHeader(EntryHeader &entry_header ){
			uint16_t offset = id*sizeof(EntryHeader) + sizeof(DirBlockHeader);
			arr.loadFromStruct(&entry_header, offset);
			return ;
		}
		UInt8Array getName() const {
			EntryHeader entry_header = getEntryHeader();
			uint16_t length = entry_header.name_len;
			uint16_t offset = entry_header.offset;
			UInt8Array res = arr.sub(offset,length);
			return res;
		}
		void setName(UInt8Array &name){
			EntryHeader entry_header = getEntryHeader();
			uint16_t offset = entry_header.offset;
			for(size_t i=0;i<name.getSize();i++)
				arr.set(offset+i, name.get(i));
		}
		iterator& operator--() {
			--id;
			return *this;
		}
		iterator& operator++() {
			++id;
			return *this;
		}
		iterator operator+(const uint16_t &arg) const {
			return iterator(arr, id+arg);
		}
		bool operator==(const iterator &arg) const {
			return arg.id == id;
		}
		bool operator!=(const iterator &arg) const {
			return arg.id == id;
		}
	};

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