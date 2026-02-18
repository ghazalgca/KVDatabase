#include <iostream>
#include <optional>
#include <unordered_map>
#include <string>
#include <thread>
#include <sstream>
#include "KVDatabase.h"

int main()
{
    int sel{};
    bool f{ true };

    KVDatabase db;
    std::string file_name{ "kvd.bin" };

    db.deserialize(file_name);

    std::string line;
    while (f && std::getline(std::cin, line)) {

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "set") {
            std::string key, value;
            iss >> key >> value;

            std::thread([&db, key, value] {
                db.set(key, value);
                //std::cout << "[set] done\n";
                }).detach();
        }

        else if (cmd == "get") {
            std::string key;
            iss >> key;

            std::thread([&db, key] {
                std::cout << db.get(key).value_or("not find\n") << "\n\n";
                }).detach();
        }

        else if (cmd == "del") {
            std::string key;
            iss >> key;

            std::thread([&db, key] {
                if (db.del(key))
                    std::cout << "[del] done\n";
                else
                    std::cout << "[del] not found\n";
                }).detach();
        }

        else if (cmd == "show") {
            std::thread([&db] {
                //std::cout << "[show]\n";
                db.show();
                }).detach();
        }

        else if (cmd == "exit") {
            std::thread([&db, file_name] {
                db.serialize(file_name);
                //std::cout << "[exit] database saved\n";
                }).detach();

            f = false; // main thread exits
        }

        else {
            std::cout << "unknown command\n";
        }
    }
}


