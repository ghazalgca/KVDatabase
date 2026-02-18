#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include <optional>
#include <mutex>
#include "LRUCache.h"

class KVDatabase {

    std::unordered_map<std::string, std::string> store;
    LRUCache<std::string> cache{ 3 };
    mutable std::mutex mtx;

public:
    void set(const std::string& k, const std::string& v);
    std::optional<std::string> get(const std::string& k);
    bool del(const std::string& k);
    void show();
    void serialize(const std::string& name) const;
    void deserialize(const std::string& name);
};