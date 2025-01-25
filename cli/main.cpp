#include <BinaryIO.hpp>
#include <iostream>
#include <Readable.hpp>

class Account : public Readable {
    std::string name;
    std::string password;
public:
    void read(BinaryReader& reader) {
        name = reader.read<std::string>();
        password = reader.read<std::string>();
    }
};

int main()
{
    BinaryWriter writer("res/test.bin");
    writer.write<std::string>("Hello");

    BinaryReader reader("res/test.bin");
    std::cout << reader.read<std::string>() << std::endl;

    return 0;
}