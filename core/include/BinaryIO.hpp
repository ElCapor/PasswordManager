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

    bool isValid();

    bool open(std::string path, std::string mode);
    void close();
    
};