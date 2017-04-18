#include "stdafx.h"
#include "bstream.h"

bStream::bStream(std::string path, Endianess ord){
    base.open(path, std::ios::in);
    stream_pos = 0;
    last_pos = 0;
    order = ord;
}


std::fstream *bStream::getStream(){
    return &base;
}
void bStream::seek(long pos){
    base.seekg(pos, base.beg);
}
uint32_t bStream::readUInt32(){
    uint32_t r;
    base.read((char*)&r, sizeof(uint32_t));
    if(order == Endianess::Big){
        return ( ((r>>24)&0xFF) | ((r<<8) & 0xFF0000) | ((r>>8)&0xFF00) | ((r<<24)&0xFF000000));
    }
    else{
        return r;
    }
}

int32_t bStream::readInt32(){
    int32_t r;
    base.read((char*)&r, sizeof(int32_t));
    if(order == Endianess::Big){
        return ( ((r>>24)&0xFF) | ((r<<8) & 0xFF0000) | ((r>>8)&0xFF00) | ((r<<24)&0xFF000000));
    }
    else{
        return r;
    }
}

uint16_t bStream::readUInt16(){
    uint16_t r;
    base.read((char*)&r, sizeof(uint16_t));
    if(order == Endianess::Big){
        return ( ((r<<8)&0xFF00) | ((r>>8)&0x00FF) );
    }
    else{
        return r;
    }
}

int16_t bStream::readInt16(){
    int16_t r;
    base.read((char*)&r, sizeof(int16_t));
    if(order == Endianess::Big){
        return ( ((r<<8)&0xFF00) | ((r>>8)&0x00FF) );
    }
    else{
        return r;
    }
}


uint8_t bStream::readUInt8(){
    uint8_t r;
    base.read((char*)&r, sizeof(uint8_t));
    return r;
}

int8_t bStream::readInt8(){
    int8_t r;
    base.read((char*)&r, sizeof(int8_t));
    return r;
}

float bStream::readFloat(){
    char buff[sizeof(float)];
    base.read(buff, sizeof(buff));
    if(order == Endianess::Big){
        char t[sizeof(float)];
        t[3] = buff[0];
        t[2] = buff[1];
        t[1] = buff[2];
        t[0] = buff[3];
    }
    return *((float*)buff);
}

std::string bStream::readString(int len){
    char* str = (char*)std::malloc(len);
    base.read(str, len);
    std::string stdstr(str);
    std::free(str);
    return stdstr;
}

void bStream::readStruct(void* out, size_t size){
    base.read((char*)out, size);
}
