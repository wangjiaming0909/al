#ifndef _RECIPES_KMP_H_
#define _RECIPES_KMP_H_
#include <cstdint>
#include <vector>
#include <memory>
#include <cassert>
#include <cstring>
#include <iostream>

namespace substring_search
{
size_t kmp_search(const char* text, const char* pattern)
{
    if(text == nullptr || pattern == nullptr)
    {
        return -1;
    }
    size_t text_len = strlen(text);
    size_t pattern_len = strlen(pattern);
    // auto search_table = build_search_table(pattern, pattern_len);
}

std::shared_ptr<std::vector<int>>
build_search_table(const char* pattern, size_t len)
{
    assert(pattern != nullptr);
    std::shared_ptr<std::vector<int>> search_table
    = std::make_shared<std::vector<int>>();

    search_table->push_back(-1);

    int i = 1;
    while(true)
    {
        if(search_table->size() == len) break;
        int valueIWant = search_table->back() + 1;
        int compareIndex = i;
        int currentValue = 0;

        if(valueIWant == 1 && pattern[compareIndex - 1] == *pattern && pattern[valueIWant] == pattern[i])
        {
            currentValue = valueIWant;
        }

        if(valueIWant > 1 && pattern[compareIndex] == pattern[valueIWant])
            currentValue = valueIWant;

        search_table->push_back(currentValue);
        i++;
    }
    return search_table;
}

std::shared_ptr<std::vector<int>>
build_search_table2(const char* pattern, size_t len)
{
    assert(pattern != nullptr);
    std::shared_ptr<std::vector<int>> search_table
    = std::make_shared<std::vector<int>>();

    search_table->push_back(-1);

    int i = 1;
    while(true)
    {
        if(search_table->size() == len) break;
        int valueIWant = search_table->back() + 1;
        int compareIndex = i;
        int currentValue = 0;

        while(pattern[valueIWant] == pattern[compareIndex] 
            && valueIWant >= 0)
        {
            valueIWant--;
            compareIndex--;
        }
        if(valueIWant < 0) currentValue = search_table->back() + 1;

        search_table->push_back(currentValue);
        i++;
    }
    return search_table;
}
}
#endif




