#ifndef _AL_MERGE_H_
#define _AL_MERGE_H_
#include <vector>
#include <algorithm>
#include <iostream>
#include "utils/timer.h"
#include "MinHeap.h"
namespace al
{

/**
 * Merge K sorted array(could be with different size)
 */

//Add all the elements into one vector and sort it
std::vector<double> mergeMultiSortedArray1(const std::vector<std::vector<double>>& arrays)
{
    std::vector<double> ret{};
    for(const auto &v : arrays)
    {
        for(auto d : v)
        {
            ret.push_back(d);
        }
    }

    std::sort(ret.begin(), ret.end());
    return ret;
}

struct MergeNode
{
    MergeNode(double value, int index) : value(value), indexInArray(index){}
    double value;
    int indexInArray;
    bool operator>(const MergeNode& node) const {return value > node.value;}//have to be const for std::greater
    bool operator>=(const MergeNode& node) const {return value >= node.value;}
    bool operator<(const MergeNode& node) const {return value < node.value;}
    bool operator<=(const MergeNode& node) const {return value <= node.value;}
    bool operator==(const MergeNode& node) const {return value == node.value;}
};

std::vector<double> mergeMultiSortedArray2(std::vector<std::vector<double>> &arrays)
{
    std::vector<double> ret{};
    if(arrays.size() == 0) return ret;
    al::MinHeap<MergeNode> minHeap{};

    std::vector<std::decay<decltype(arrays[0])>::type::const_iterator> its;
    for(size_t i = 0; i < arrays.size(); i++ )
    {
        its.push_back(arrays[i].cbegin());
    }

    for(size_t i = 0; i < arrays.size(); i++)
    {
        minHeap.emplace(*its[i], i);
    }
    
    while(true)
    {
        MergeNode minNode = minHeap.top();
        ret.push_back(minNode.value);
        minHeap.pop();
        if(arrays[minNode.indexInArray].cend() != (its[minNode.indexInArray] + 1))
        { 
            minHeap.emplace(*(its[minNode.indexInArray] + 1), minNode.indexInArray);
            ++its[minNode.indexInArray];
        }
        if(minHeap.size() == 0) break;
    }
    return ret;
}

void test_mergeMultiSortedArray()
{
    std::vector<double> v1 = {1,2,3,4,5,6,7, 8, 9, 10, 11, 12 , 13, 15};
    std::vector<double> v2 = {1,2,8,9,15,16,17, 19, 21, 22, 23, 24, 25, 27};
    std::vector<double> v3 = {1,2,3,4,15,26,27, 31, 99, 120, 222, 333, 444, 555, 666, 777, 8888, 100000};

    std::vector<std::vector<double>> arrays{};

    arrays.push_back(v1);
    arrays.push_back(v2);
    arrays.push_back(v3);

    std::vector<double> ret{};

    {
        utils::timer _{"mergeMultiSortedArray1"};
        ret = mergeMultiSortedArray1(arrays);
    }

    for(size_t i = 0; i < ret.size(); i++)
    {
        std::cout << "index: " << i << " " << ret[i] << std::endl;
    }

    {
        utils::timer _{"mergeMultiSortedArrays with minheap"};
        ret = mergeMultiSortedArray2(arrays);
    }

    for(size_t i = 0; i < ret.size(); i++)
    {
        std::cout << "index: " << i << " " << ret[i] << std::endl;
    }

}

}//namespace al
#endif //_AL_MERGE_H_