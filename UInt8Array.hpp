#ifndef __UINT8ARRAY_HPP_
#define __UINT8ARRAY_HPP_
#include<cstdint>
#include<cstring>
#include<iostream>
using namespace std;
class UInt8Array{
private:
	uint16_t size;
	uint8_t *data_arr;
public:
	UInt8Array(){size = 0;}
	UInt8Array(uint16_t arr_size){
		size = arr_size;
		cout<<"new array "<<size<<endl;
		data_arr = new uint8_t[size];
	}
	~UInt8Array(){
		delete [] data_arr;
	}
	uint16_t getSize(){
		return size;
	}
	uint8_t get(uint16_t i){
		if(i>=size||i<0) throw "ArrData out of range";
		return data_arr[i];
	}
	void set(uint16_t i,uint8_t x){
		if(i>=size||i<0) throw "ArrData out of range";
		data_arr[i] = x;
	}
	UInt8Array sub(uint16_t offset,uint16_t length){
		if(offset+length>size||offset<0) throw "ArrData out of range";
		UInt8Array res = UInt8Array(length);
		for(int i=0;i<length;i++) 
			res.set(i,data_arr[offset+i]);
	}
	template <typename T>
	void loadToStruct(T *x,uint16_t offset = 0){
		if(offset+sizeof(x)>size) throw "ArrData out of range";
		memcpy((char*)x,(char*)data_arr+offset,sizeof(T));
	}
	template <typename T>
	void loadFromStruct(T *x,uint16_t offset = 0){
		if(offset+sizeof(T)>size) throw "ArrData out of range";
			int i=0;
		cout<<"loadFromStruct "<<sizeof(T)<<endl;
		memcpy((char *)data_arr+offset,(char*)x,sizeof(T));
//		for(int i=0;i<size;i++) cout<<(int)data_arr[i]<<endl;

	}
};
#endif