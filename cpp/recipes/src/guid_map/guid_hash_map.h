#ifndef _GUID_HASH_MAP_H_
#define _GUID_HASH_MAP_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <stack>

using guid_t = std::string;

struct guid_node{
    guid_node()
        : referenced_guid()
        , removed(false)
    {}
    guid_node(guid_t referenced_guid)
        : referenced_guid(referenced_guid)
        , removed(false)
    {}
    guid_t 	referenced_guid;
    bool 	removed;
};

using guid_hash_map =  std::unordered_map<guid_t, guid_node>;

struct guid_map_t
{
    guid_map_t(guid_t key, guid_t referenced) : key_guid(key), referenced_guid(referenced){}
    guid_t key_guid;
    guid_t referenced_guid;
};

std::vector<guid_map_t> sort_guid_map(guid_hash_map& guid_hash_map)
{
    std::vector<guid_map_t> 		result{};
    std::stack<guid_map_t> 			guid_stack{};
    for (guid_hash_map::iterator it = guid_hash_map.begin(); it != guid_hash_map.end(); it++) {
        //this guid has not been added into result
        if(!it->second.removed)
        {
            guid_map_t current_guid{it->first, it->second.referenced_guid};
            guid_stack.push(current_guid);
            while (current_guid.referenced_guid != "") {
                current_guid.key_guid = current_guid.referenced_guid;
                current_guid.referenced_guid = guid_hash_map[current_guid.key_guid].referenced_guid;
                if(guid_hash_map[current_guid.key_guid].removed == true)
                    break;
                guid_stack.push(current_guid);
            }

            while(!guid_stack.empty())
            {
                auto& guid = guid_stack.top();
                result.push_back(guid_map_t(guid.key_guid, guid.referenced_guid));
                guid_hash_map[guid.key_guid].removed = true;
                guid_stack.pop();
            }
        }
    }
    return result;
}

#endif

