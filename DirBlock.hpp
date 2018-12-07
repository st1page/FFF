#ifndef __DIRBLOCK_HPP_
#define __DIRBLOCK_HPP_

#include <cstdint>

#include "BlockTypes.h"
#include "SuperBlock.hpp"
#include "BlockIO.hpp"
#include "UInt8Array.hpp"

class DirBlock{
private:
	DirBlockHeader header;
	BlockIO io;
public:	
	struct iterator;
	DirBlock(uint16_t id, const SuperBlockData& super) {
		uint16_t size = super.block_size;
		uint32_t offset = id * size;
		offset += super.first_data_block*size;
		io = BlockIO(size, offset);
		read();
	}
	void format(const SuperBlockData& super){
		header.entry_count = 0;
		header.free_size = super.block_size - sizeof(DirBlockHeader);
		write();
	}
	void write(){
		io.data.loadFromStruct(&header);
		io.write();
	}
	void read(){
		io.read();
		io.data.loadToStruct(&header);
	}
	uint16_t getFreeSize(){
		return header.free_size;
	}
	iterator find(UInt8Array name){
		iterator iter = begin();
		for(;iter!=end();++iter){
			if(iter.getName() == name) return iter;
		}
		return end();
	}
	void push_back(Entry &entry){
		uint16_t offset = rbegin().getEntryHeader().offset;
		offset -= entry.name.getSize(); 
		
		iterator iter = rbegin();
		entry.header.offset = offset;
		iter.setEntryHeader(entry.header);
		iter.setName(entry.name);
		header.entry_count++;
		header.free_size += entry.size();
	}
	void del(iterator iter){
		EntryHeader entry_header = iter.getEntryHeader();
		uint16_t d_offset = entry_header.name_len;
		for(uint16_t i = entry_header.offset-1;
			i >= rbegin().getEntryHeader().offset ;i--)
			 io.data.set(i+d_offset,io.data.get(i));
		for(iterator it = iter;it!=rbegin();++it) {
			EntryHeader cur_header = (it+1).getEntryHeader();
			cur_header.offset += d_offset;
			it.setEntryHeader(cur_header);
		}
		header.entry_count -=1 ;
		header.free_size -= entry_header.name_len + sizeof(EntryHeader);
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
	iterator begin() {
		return iterator(io.data, 0);
	}
	iterator end() {
		return iterator(io.data, header.entry_count);
	}
	iterator rbegin() {
		return iterator(io.data, header.entry_count-1);
	}

};
#endif