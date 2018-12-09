#ifndef __DIRECTORY_HPP_
#define __DIRECTORY_HPP_

#include <cstdint>

#include "DataTypes.h"
#include "SuperBlock.hpp"
#include "BlockIO.hpp"
#include "UInt8Array.hpp"
#include "Block.hpp"
#include "Allocator.hpp"
#include "Inode.hpp"
#include "File.hpp"
class Directory:public File {
private:
public:	
	Directory(uint16_t inode_id, SuperBlockData& s)
		:File(inode_id, s) {
	}
	void addEntry(Entry &entry){

	}
	Entry getEntry(const UInt8Array& name){

	}
	void modifyEntry(Entry &entry){

	}
	void removeEntry(const UInt8Array& name){
		
	}

};
#endif