#include "BinaryIO.hpp"
#include <filesystem>
#include <exception>
#include <iostream>

namespace fs = std::filesystem;

#define FATAL_CHECK_FILE(fp, ...) if (fp == NULL) {\
    std::cout << __FUNCTION__ << " Invalid FILE object FATAL ERROR" << std::endl; \
    std::__throw_logic_error("FILE was null"); \
    } 

#define NON_FATAL_CHECK_FILE(fp, ...) if (fp == NULL) { \
    std::cout << __FUNCTION__ << " Invalid FILE object, operation canceled" << std::endl; \
    return __VA_ARGS__; \
    }

// we still have some hope about the file existing on disk !
#define HOPE_CHECK_FILE(fp, ...) if (fp == NULL) { \
    std::cout << __FUNCTION__ << "WARNING : Invalid file, attempting a reopen" << std::endl; \
    this->open(this->path, "rb");\
    FATAL_CHECK_FILE(this->fp, 0);\
    }

#ifndef ERROR_LEVEL
    #define ERROR_LEVEL -1
#endif

#if ERROR_LEVEL==0
    #define CHECK_FILE NON_FATAL_CHECK_FILE
#elif ERROR_LEVEL==1
    #define CHECK_FILE FATAL_CHECK_FILE
#elif ERROR_LEVEL==2
    #define CHECK_FILE HOPE_CHECK_FILE
#else
    #define CHECK_FILE
#endif

#define FCALL(func, ...) if (func(__VA_ARGS__) != 0) {\
    std::cout << "Calling " << #func << " failed inside " << __FUNCTION__ <<std::endl; \
    std::__throw_logic_error("Invalid call"); \
    }

BinaryReader::BinaryReader(std::string path)
{
    this->open(path, "rb");
}

BinaryReader::BinaryReader()
{
    #ifdef BINARY_WARN_EMPTY
    std::cout << "WARNING, AN EMPTY BINARY READER WAS CREATED" << std::endl;
    #endif
    this->cur_pos = -1;
    this->fp = 0;
}

BinaryReader::~BinaryReader()
{
    CHECK_FILE(this->fp);
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryReader::~BinaryReader()] " << std::endl;
    #endif
    FCALL(fclose, this->fp);
    this->fp = NULL;
    this->cur_pos = -1;
}

bool BinaryReader::open(std::string path, std::string mode)
{
    if (fs::exists(path))
    {
        this->cur_pos = 0;
        this->fp = fopen(path.c_str(), mode.c_str());
    } else if (fs::exists(fs::current_path()/path)) {
        // we still have some hope about it
        BinaryReader(fs::current_path()/path);
    }
    else {
        std::__throw_invalid_argument("Invalid path");
    }
}

int BinaryReader::get_pos()
{
    CHECK_FILE(this->fp, -1);
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryReader::GetPos()] " << this->cur_pos << std::endl;
    #endif
    return this->cur_pos;
}

int BinaryReader::set_pos(int newPos)
{
    CHECK_FILE(this->fp, -1);
    int oldPos = get_pos();
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryReader::SetPos( " << newPos << ")] " << oldPos << std::endl;
    #endif

    this->cur_pos = newPos;
    fpos_t pos = newPos;
    FCALL(fsetpos, this->fp, &pos);
    return oldPos;
}

template <>
std::uint8_t BinaryReader::read<std::uint8_t>()
{
    FATAL_CHECK_FILE(this->fp, 0);
    std::uint8_t buff;
    fread(&buff, sizeof(std::uint8_t), 1, this->fp);
    fpos_t pos;
    FCALL(fgetpos, this->fp, &pos);
    this->cur_pos = pos;
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryReader::Read()] " << std::hex << buff << " pos : " << std::dec << pos << std::endl;
    #endif
    return buff;
}

void BinaryReader::close()
{
    CHECK_FILE(this->fp);
    FCALL(fclose, this->fp);
    this->fp = NULL;
    this->cur_pos = -1;
}

bool BinaryReader::isValid()
{
    return this->fp != NULL && this->cur_pos != -1;
}


template <typename T>
T BinaryReader::read()
{
    std::__throw_logic_error("Arbitrary type reading is not supported");
}

template <>
int BinaryReader::read<int>()
{
    FATAL_CHECK_FILE(this->fp, 0);
    int buff;
    fread(&buff, sizeof(int), 1, this->fp);
    fpos_t pos;
    FCALL(fgetpos, this->fp, &pos);
    this->cur_pos = pos;
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryReader::Read()] " << std::hex << buff << " pos : " << std::dec << pos << std::endl;
    #endif
    return buff;
}

template <>
char BinaryReader::read<char>()
{
    FATAL_CHECK_FILE(this->fp, 0);
    char buff;
    fread(&buff, sizeof(char), 1, this->fp);
    fpos_t pos;
    FCALL(fgetpos, this->fp, &pos);
    this->cur_pos = pos;
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryReader::Read()] " << std::hex << buff << " pos : " << std::dec << pos << std::endl;
    #endif
    return buff;
}

template <>
float BinaryReader::read<float>()
{
    FATAL_CHECK_FILE(this->fp, 0);
    float buff;
    fread(&buff, sizeof(float), 1, this->fp);
    fpos_t pos;
    FCALL(fgetpos, this->fp, &pos);
    this->cur_pos = pos;
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryReader::Read()] " << std::hex << buff << " pos : " << std::dec << pos << std::endl;
    #endif
    return buff;
}

template <>
char* BinaryReader::read<char*>(size_t size)
{
    FATAL_CHECK_FILE(this->fp, 0);
    char* buff = new char[size];
    fread(buff, sizeof(char), size, this->fp);
    fpos_t pos;
    FCALL(fgetpos, this->fp, &pos);
    this->cur_pos = pos;
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryReader::Read()] " << std::hex << buff << " pos : " << std::dec << pos << std::endl;
    #endif
    return buff;
}

template <>
std::string BinaryReader::read<std::string>()
{
    FATAL_CHECK_FILE(this->fp, "");
    int size;
    char* buff = nullptr;
    size = read<int>();
    buff = read<char*>(size);
    std::string str(buff);
    delete[] buff;
    return str;
}


BinaryWriter::BinaryWriter(std::string path)
{
    if (!this->open(path, "wb"))
        std::__throw_invalid_argument("Invalid path");
}

BinaryWriter::BinaryWriter()
{
    #ifdef BINARY_WARN_EMPTY
    std::cout << "WARNING, AN EMPTY BINARY WRITER WAS CREATED" << std::endl;
    #endif
    this->cur_pos = -1;
    this->fp = 0;
}

BinaryWriter::~BinaryWriter()
{
    CHECK_FILE(this->fp);
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryWriter::~BinaryWriter()] " << std::endl;
    #endif
    FCALL(fclose, this->fp);
    this->fp = NULL;
    this->cur_pos = -1;
}

bool BinaryWriter::open(std::string path, std::string mode)
{
    if (fs::exists(path))
    {
        this->cur_pos = 0;
        this->fp = fopen(path.c_str(), mode.c_str());
        if (this->fp == NULL)
            return false;
        return true;
    } else if (fs::exists(fs::current_path()/path)) {
        // we still have some hope about it
        return this->open(fs::current_path()/path, mode);
    }
    else {
        return false;
    }
}

int BinaryWriter::get_pos()
{
    CHECK_FILE(this->fp, -1);
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryWriter::GetPos()] " << this->cur_pos << std::endl;
    #endif
    return this->cur_pos;
}

int BinaryWriter::set_pos(int newPos)
{
    CHECK_FILE(this->fp, -1);
    int oldPos = get_pos();
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryWriter::SetPos( " << newPos << ")] " << oldPos << std::endl;
    #endif

    this->cur_pos = newPos;
    fpos_t pos = newPos;
    FCALL(fsetpos, this->fp, &pos);
    return oldPos;
}

template <typename T>
void BinaryWriter::write(T data)
{
    std::__throw_logic_error("Arbitrary type writing is not supported");
}

template <>
void BinaryWriter::write<std::uint8_t>(std::uint8_t data)
{
    FATAL_CHECK_FILE(this->fp);
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryWriter::Write( " << data << " )] " << this->cur_pos << std::endl;
    #endif
    fwrite(&data, sizeof(std::uint8_t), 1, this->fp);
    fpos_t pos;
    FCALL(fgetpos, this->fp, &pos);
    this->cur_pos = pos;
}

template <>
void BinaryWriter::write<std::string>(std::string data)
{
    FATAL_CHECK_FILE(this->fp);
    int size = data.size();
    fwrite(&size, sizeof(int), 1, this->fp);
    fwrite(data.c_str(), sizeof(char), size, this->fp);
    fpos_t pos;
    FCALL(fgetpos, this->fp, &pos);
    this->cur_pos = pos;
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryWriter::Write( " << data << " )] " << pos << std::endl;
    #endif
}

template <>
void BinaryWriter::write<int>(int data)
{
    FATAL_CHECK_FILE(this->fp);
    fwrite(&data, sizeof(int), 1, this->fp);
    fpos_t pos;
    FCALL(fgetpos, this->fp, &pos);
    this->cur_pos = pos;
    #ifdef BINARY_VERBOSE
    std::cout << "[BinaryWriter::Write( " << data << " )] " << pos << std::endl;
    #endif
}

bool BinaryWriter::isValid()
{
    return this->fp != NULL && this->cur_pos != -1;
}