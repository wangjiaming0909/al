#ifndef _AL_MERGE_H_
#define _AL_MERGE_H_
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include "utils/timer.h"
#include "MinHeap.h"
namespace al
{

/**
 * Merge K sorted array(could be with different size)
 */

//Add all the elements into one vector and sort it
std::vector<double> mergeMultiSortedArraySlow(const std::vector<std::vector<double>>& arrays)
{
    std::vector<double> ret{};
    for(const auto &v : arrays)
    {
        for(auto d : v)
        {
            ret.push_back(d);
        }
    }

    std::sort(ret.begin(), ret.end());// O(Nlog(N))
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

std::vector<double> mergeMultiSortedArrayWithMinHeap(std::vector<std::vector<double>> &arrays)
{
    std::vector<double> ret{};
    if(arrays.size() == 0) return ret;
    al::MinHeap<MergeNode> minHeap{};

    std::vector<std::decay<decltype(arrays[0])>::type::const_iterator> its;
    for(size_t i = 0; i < arrays.size(); i++ ) //O(k)
    {
        its.push_back(arrays[i].cbegin());
    }

    for(size_t i = 0; i < arrays.size(); i++)//O(k)
    {
        minHeap.emplace(*its[i], i);
    }
    
    while(true)// 在 MinHeap中, 一共有K个元素, 一共进行了pop或者emplace N次 (N为所有的元素个数), 因此复杂度为: O(2Nlog(K))
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
    std::vector<double> v4{};
    std::vector<double> v5{};
    std::vector<double> v6{};
    for(int i = 0; i < 999999; i++)
    {
        v4.push_back((i + 0.1) * 2);
        v5.push_back((i + 0.5) * 2.4);
        v6.push_back((i * 1.2) + 2.5 );
    }

    std::vector<std::vector<double>> arrays{};

    arrays.push_back(v1);
    arrays.push_back(v2);
    arrays.push_back(v3);
    arrays.push_back(v4);
    arrays.push_back(v5);
    arrays.push_back(v6);

    std::vector<double> ret{};

    {
        utils::timer _{"mergeMultiSortedArraySlow"};//1.0551276s
        ret = mergeMultiSortedArraySlow(arrays);
    }


    double lastValue = -1;
    for(size_t i = 0; i < ret.size(); i++)
    {
        assert(ret[i] >= lastValue);
        lastValue = ret[i];
    }

    {
        utils::timer _{"mergeMultiSortedArrays with minheap"};//1.069112s
        ret = mergeMultiSortedArrayWithMinHeap(arrays);
    }

    lastValue = -1;
    for(size_t i = 0; i < ret.size(); i++)
    {
        assert(ret[i] >= lastValue);
        lastValue = ret[i];
    }

}

}//namespace al
#endif //_AL_MERGE_H_