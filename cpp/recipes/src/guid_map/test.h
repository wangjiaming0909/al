#ifndef TEST_H
#define TEST_H
#include "guid_hash_map.h"
#include <iostream>
#include <cassert>
#include <algorithm>

namespace guid_map_test
{

void run_tests()
{
    guid_hash_map data{};
    data["13"] = guid_node{"7"};
    data["6"] = guid_node{"1"};
    data["11"] = guid_node{"13"};
    data["4"] = guid_node{"7"};
    data["5"] = guid_node{"6"};
    data["2"] = guid_node{"3"};
    data["12"] = guid_node{"1"};
    data["3"] = guid_node{"4"};
    data["7"] = guid_node{""};
    data["9"] = guid_node{"3"};
    data["10"] = guid_node{"9"};
    data["8"] = guid_node{"1"};
    data["1"] = guid_node{"2"};

    auto result = sort_guid_map(data);
    std::vector<guid_t> keys{};
    std::vector<guid_t> refs{};
    for(size_t i = 0; i < result.size(); i++)
    {
        keys.push_back(result[i].key_guid);
        refs.push_back(result[i].referenced_guid);
    }
    for (size_t i = 0; i < refs.size(); i++) {
        if(refs[i] != "")
        {
            auto it = std::find(keys.begin(), keys.end(), refs[i]);
            size_t index_in_keys = it - keys.begin();
            size_t index_in_refs = i;
            std::cout << *it << " index in keys: " << index_in_keys << " " << refs[i] << " index in refs: " << index_in_refs << std::endl;
            assert(index_in_refs > index_in_keys);
        }
    }
    std::cout << std::endl;
    for(size_t i = 0; i < result.size(); i++)
    {
        std::cout << "key_guid: " << result[i].key_guid << " referenced_guid: " << result[i].referenced_guid << std::endl;
    }

}

} //namespace guid_map_test
#endif // TEST_H
