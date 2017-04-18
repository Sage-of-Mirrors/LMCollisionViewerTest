#ifndef BSTREAM_H
#define BSTREAM_H
#endif // BSTREAM_H
#include <iostream>
#include <cstdint>
#include <fstream>

enum Endianess{
    Big, Little
};

class bStream {
private:
    //TODO: Implement
    std::fstream base;
    int stream_pos;
    int last_pos;
    Endianess order;


public:

    void readStruct(void*, size_t);

    //read functions
    int8_t readInt8();
    uint8_t readUInt8();
    int16_t readInt16();
    uint16_t readUInt16();
    int32_t readInt32();
    uint32_t readUInt32();
    float readFloat();
    std::string readString(int);


    //utility functions
    void seek(long);
    void seekRet();

    char peekI8(int);
    short peekI16(int);
    int peekI32(int);

    std::fstream *getStream();

    unsigned char peekU8(int);
    unsigned short peekU16(int);
    unsigned int peekU32(int);

    bStream(std::string, Endianess);
    bStream() {}
    ~bStream() {this->base.close();}
};
