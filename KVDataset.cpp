#include <fstream>
#include <unordered_map>
#include <string>
#include <iostream>
#include <optional>
#include "KVDatabase.h"

void KVDatabase::set(const std::string& k, const std::string& v) {
    std::lock_guard<std::mutex> lock(mtx);
    store.insert({ k, v });
}

std::optional<std::string> KVDatabase::get(const std::string& k) {
    std::lock_guard<std::mutex> lock(mtx);
    auto result = cache.get(k);
    if (result.has_value()) {
        return result;
    }
    else {
        if (store.contains(k)) {
            auto node = store.find(k);
            Node* n = new Node(node->first, node->second);
            cache.put(k, n);
            return node->second;
        }
        else
            return std::nullopt;
    }
}

bool KVDatabase::del(const std::string& k) {
    std::lock_guard<std::mutex> lock(mtx);
    if (store.contains(k)) {
        store.erase(k);
        return true;
    }
    else
        return false;
}

void KVDatabase::show() {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto it = store.begin(); it != store.end(); it++)
        std::cout << it->first << ": " << it->second << "\n";
}

void KVDatabase::serialize(const std::string& name) const {
    std::lock_guard<std::mutex> lock(mtx);

    std::ofstream os(name, std::ios::binary);

    size_t count = store.size();
    os.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& [key, value] : store) {
        size_t keyLen = key.size();
        os.write(reinterpret_cast<const char*>(&keyLen), sizeof(keyLen));
        os.write(key.data(), keyLen);

        size_t valLen = value.size();
        os.write(reinterpret_cast<const char*>(&valLen), sizeof(valLen));
        os.write(value.data(), valLen);
    }
}


void KVDatabase::deserialize(const std::string& name) {
    std::lock_guard<std::mutex> lock(mtx);

    std::ifstream is(name, std::ios::binary);
    if (!is) {
        return;
    }

    size_t count;
    is.read(reinterpret_cast<char*>(&count), sizeof(count));

    for (size_t i = 0; i < count; i++) {
        size_t keyLen;
        is.read(reinterpret_cast<char*>(&keyLen), sizeof(keyLen));

        std::string key(keyLen, '\0');
        is.read(&key[0], keyLen);

        size_t valLen;
        is.read(reinterpret_cast<char*>(&valLen), sizeof(valLen));

        std::string value(valLen, '\0');
        is.read(&value[0], valLen);

        store.emplace(std::move(key), std::move(value));
    }
}