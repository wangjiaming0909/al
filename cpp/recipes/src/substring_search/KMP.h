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

        if(valueIWant == 1 && pattern[compareIndex - 1] == *pattern)
        {
            currentValue = valueIWant;
        }

        if(valueIWant > 1 && pattern[compareIndex] == pattern[valueIWant])
            currentValue = valueIWant;

        // while(pattern[valueIWant] == pattern[compareIndex] 
        //     && valueIWant >= 0)
        // {
        //     valueIWant--;
        //     compareIndex--;
        // }
        // if(valueIWant < 0) currentValue = search_table->back() + 1;

        search_table->push_back(currentValue);
        i++;
    }
    return search_table;
}

void test_build_search_table()
{
    const char* pattern = "ABABABAB";
    size_t len = strlen(pattern);
    auto table = build_search_table(pattern, len);
    std::cout << "-";
    std::cout << pattern << std::endl;
    for(auto i : *table)
    {
        std::cout << i;
    }
    std::cout << std::endl;


    std::vector<int> expectedTable = {-1, 0, 0, 1, 2, 3, 4, 5};
    assert(*table == expectedTable);

    pattern = "ABCDEFG";
    len = strlen(pattern);
    table = build_search_table(pattern, len);

    expectedTable.clear();
    expectedTable = {-1, 0, 0, 0, 0, 0, 0};
    assert(*table == expectedTable);

    pattern = "chinchinla";
    len = strlen(pattern);
    table = build_search_table(pattern, len);

    expectedTable.clear();
    expectedTable = {-1, 0, 0, 0, 0, 1, 2, 3, 0, 0};
    assert(*table == expectedTable);

}


void test_kmp()
{
    test_build_search_table();
    std::cout << 123 << std::endl;
}

}
#endif




