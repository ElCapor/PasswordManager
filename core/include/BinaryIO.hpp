#include <stdio.h>
#include <cstdint>
#include <string>

class BinaryReader {
    FILE* fp; // C file to read from
    int cur_pos; // current cursor position
public:
    BinaryReader(std::string path);
    BinaryReader();
    ~BinaryReader();

    int get_pos();
    int set_pos(int newPos);

    template <typename T> 
    T read();

    template <>
    std::string read<std::string>();

    template <>
    std::uint8_t read<std::uint8_t>();

    template <>
    int read<int>();

    template <>
    char read<char>();

    template <>
    float read<float>();

    // some types like arrays we only want to read them with a size
    template <typename T>
    T read(size_t size);

    template <>
    char* read<char*>(size_t size);

    bool isValid();

    bool open(std::string path, std::string mode);
    void close();
    
};

class BinaryWriter {
    FILE* fp; // C file to read from
    int cur_pos; // current cursor position
public:
    BinaryWriter(std::string path);
    BinaryWriter();
    ~BinaryWriter();

    int get_pos();
    int set_pos(int newPos);

    template <typename T>
    void write(T data);

    template <>
    void write<std::uint8_t>(std::uint8_t data);

    template <>
    void write<std::string>(std::string data);

    template <>
    void write<int>(int data);

    bool isValid();


    bool open(std::string path, std::string mode);
    void close();
    
};