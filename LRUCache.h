#pragma once
#include "Node.h"

template<typename K>
class LRUCache {
    int capacity{};
    Node* head;
    Node* tail;
    std::unordered_map<K, Node*> cache;

    void show() {
        for (auto it = cache.begin(); it != cache.end(); it++)
            std::cout << it->second->key << ": " << it->second->value << "\n";
    }

    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void hit(Node* node) {

        remove(node);

        head->prev->next = node;
        head->prev = node;
        node->prev = head->prev;
        node->next = head;

    }

    void add(Node* node) {
        node->next = tail->next;
        tail->next->prev = node;
        node->prev = tail;
        tail->next = node;
    }

    void remove_tail() {
        cache.erase(tail->next->key);
        tail->next->prev = nullptr;
    }

public:
    LRUCache(int c) {
        capacity = c;
        head = new Node("k", "v");
        head->prev = tail;
        tail = new Node("k", "v");
        tail->next = head;
    }

    std::optional<std::string> get(K key) {

        if (cache.contains(key)) {
            auto node = cache.find(key);
            hit(node->second);
            return node->second->value;
        }
        else {
            return std::nullopt;
        }
    }

    void put(K key, Node* value) {

        if (cache.size() >= capacity) {
            remove_tail();
        }
        add(value);
        cache.insert({ key, value });
    }

};

