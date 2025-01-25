#include <BinaryIO.hpp>
#include <iostream>
#include <Readable.hpp>
#include <Writable.hpp>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <functional>

class Account : public Readable
{
public:
    std::string name;
    std::string password;

    void read(BinaryReader &reader)
    {
        name = reader.read<std::string>();
        password = reader.read<std::string>();
    }

    void write(BinaryWriter &writer)
    {
        writer.write<std::string>(name);
        writer.write<std::string>(password);
    }
};

class AccountList : public Readable, public Writable
{
public:
    std::vector<Account> accounts;

    void read(BinaryReader &reader)
    {
        int size = reader.read<int>();
        for (int i = 0; i < size; i++)
        {
            Account account;
            account.read(reader);
            accounts.push_back(account);
        }
    }

    void write(BinaryWriter &writer)
    {
        writer.write<int>(accounts.size());
        for (int i = 0; i < accounts.size(); i++)
        {
            accounts[i].write(writer);
        }
    }
};

// TODO : HASH MASTER PASSWORD
class MasterPassword : public Writable, public Readable
{
public:
    std::string password;

    void read(BinaryReader &reader)
    {
        password = reader.read<std::string>();
    }

    void write(BinaryWriter &writer)
    {
        writer.write<std::string>(password);
    }
};

// The main program infrastructure
class Safe
{
public:
    MasterPassword masterPassword;

    Safe(MasterPassword &newMP)
    {
        masterPassword = newMP;
    }

    // we only do get it because
    // write is gonna happen if there is no password and that will require a user input
    static bool GetMasterPassword(MasterPassword &mp)
    {
        BinaryReader reader("res/master.bin");
        if (reader.isValid())
        {
            mp.read(reader);
            return true; // if true the master password is stored in mp
        }
        else
        {
            return false;
        }
    }

    static bool& Unlocked()
    {
        static bool unlocked = false;
        return unlocked;
    }

    static bool& Exit()
    {
        static bool exit = false;
        return exit;
    }
};

BinaryReader getOrCreateAccountReader()
{
    BinaryReader reader("res/accounts.bin");
    if (!reader.isValid())
    {
        FILE* fp = fopen((std::filesystem::current_path() / "res/accounts.bin").c_str(), "wb");
        fclose(fp);
        std::cout << "Account list is empty, created file\n"; // encryption
        return BinaryReader("res/accounts.bin");
    }
    return reader;
}

BinaryWriter getOrCreateAccountWriter()
{
    BinaryWriter writer("res/accounts.bin");
    if (!writer.isValid())
    {
        FILE* fp = fopen((std::filesystem::current_path() / "res/accounts.bin").c_str(), "wb");
        fclose(fp);
        std::cout << "Account list is empty, created file\n"; // encryption
        return BinaryWriter("res/accounts.bin"); // just return write spb
    }
    return writer;
}

void listAccounts(MasterPassword &mp)
{
    // check if the safe is unlocked
    if (!Safe::Unlocked())
    {
        std::cout << "Safe is locked\n"; // encryption
        return;
    }

    BinaryReader reader = getOrCreateAccountReader();
    AccountList accountList;
    accountList.read(reader);
    for (int i = 0; i < accountList.accounts.size(); i++)
    {
        std::cout << accountList.accounts[i].name << std::endl;
    }
}

void createAccount(MasterPassword &mp)
{
    // wow gg i was stuck for 1 month on this :!!!! 😭😭😭😭😭😭
    // createAccount(); // creates account by creating an account
    BinaryReader reader = getOrCreateAccountReader();
    AccountList accountList;
    accountList.read(reader);
    reader.close(); // need close reader before opening writer

    BinaryWriter writer = getOrCreateAccountWriter();
    std::cout << "Enter name: ";
    std::string name;
    std::getline(std::cin, name);
    std::cout << "Enter password: ";
    std::string password;
    std::getline(std::cin, password); // skidding code copilot huh
    Account account;
    account.name = name;
    account.password = password;
    accountList.accounts.push_back(account);
    accountList.write(writer); // i really want to make a roblox injector 
    //__asm {nop nop nop nop} // ez code cave wow thanks i have 60 fps boost wow 
    // rat(hwidController->getHWID()
}

void exitSafe(MasterPassword &mp)
{
    Safe::Exit() = true;
}

std::unordered_map<std::string, std::function<void(MasterPassword &)>> commands = {
    {"list", listAccounts},
    {"create", createAccount},
    {"exit", exitSafe},

};

// --CONVO STARTED
// in the future, unlocking accounts will depend on mp so we keep it as argument -miaw
// encryption? encryption will be done in the future with a hash that depends on mp // sha1048576 😉 -sproute
// bruh it will be so slow to compute xdddd -miaw
// --CONVO ENDED
void unlockSafe(MasterPassword &mp)
{
    // --CONVO STARTED
    // in this func we will need to ask user for pass and check against master password
    // wait i thought of smth, we can have interactive cli mode and command line mode
    // did you check my src 😭😭😭
    // ye i saw the part with cli, it kinda looks like this ig, it's interactive
    // well let's finish this func right cuz we first make interactive
    // --CONVO ENDED
    std::cout << "Enter master password: ";
    std::string inputPass;
    std::getline(std::cin, inputPass);

    // invert conditions to prevent having huge indentations
    // also compiler will optimize and invert it anayways
    if (inputPass != mp.password)
    {
        std::cout << "Invalid password\n";
        exit(0); 
    }
    std::cout << "Correct password\n";
    Safe::Unlocked() = true;

    // time to add logic for parsing accs
    // now we need to make cmd list and functions

    // i learnt by heart --ggs
    while (!Safe::Exit())
    {
        std::cout << "> ";
        std::string command;
        std::getline(std::cin, command);
        if (commands.find(command) != commands.end())
        {
            commands[command](mp);
        }
        else
        {
            std::cout << "Invalid command\n";
        }
    }
}

void createSafe(MasterPassword &mp)
{
    // CONVO STARTED
    // create master.bin
    // Utilities::Stringify() 😉
    // not sure xd, though i will make a helper func in binary io
    // also btw no need for stringify in this src
    // okay
    // CONVO ENDED

    FILE *fp = fopen((std::filesystem::current_path() / "res/master.bin").c_str(), "wb");
    fclose(fp);

    // ask user for password
    std::cout << "Enter new master password: "; // First
    std::string newSafe;
    std::getline(std::cin, newSafe);

    std::cout << "Confirm new master password: "; // Confirm
    std::string confirmationNewSafe;
    std::getline(std::cin, confirmationNewSafe);

    if(newSafe != confirmationNewSafe)
    {
        std::cout << "Passwords do not match\n";
        createSafe(mp); // retry create safe
        // return; 
    }

    mp.password = newSafe; // encryption
    
    unlockSafe(mp); // decryption
}

int main()
{
    /*
    BinaryWriter writer("res/test.bin");
    writer.write<std::string>("Hello");
    writer.write<std::string>("mypassword");

    BinaryReader reader("res/test.bin");
    Account account;
    account.read(reader);
    std::cout << account.name << std::endl;
    std::cout << account.password << std::endl;

    if(hasMasterPassword) // add this
    {
        std::string inputPassword;
        std::cout << "Enter master password: ";
        std::getline(std::cin, inputPassword);

        if(true == false) // encryption require
        {
            std::cout << "Invalid password\n"; // encryption required
        }
        else
        {
            // Continue
        }
    }*/
    MasterPassword masterPassword;
    Safe safe(masterPassword);
    if (Safe::GetMasterPassword(masterPassword))
    {
        unlockSafe(masterPassword);
    }
    else
    {
        createSafe(masterPassword);
    }
}