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
std::vector<double> mergeMultiSortedArray1(std::vector<std::vector<double>> arrays)
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

std::vector<double> mergeMultiSortedArray2(std::vector<std::vector<double>> arrays)
{
    std::vector<double> ret{};
    al::MinHeap<double> minHeap{};

    for()

    for(const auto& v : arrays)
    {

    }

}

void test_mergeMultiSortedArray()
{
    std::vector<double> v1 = {1,2,3,4,5,6,7};
    std::vector<double> v2 = {1,2,8,9,15,16,17};
    std::vector<double> v3 = {1,2,3,4,15,26,7};

    std::vector<std::vector<double>> arrays{};

    arrays.push_back(v1);
    arrays.push_back(v2);
    arrays.push_back(v3);

    std::vector<double> ret{};

    {
        utils::timer _{"mergeMultiSortedArray1"};
        ret = mergeMultiSortedArray1(arrays);
    }

    for(auto d : ret)
    {
        std::cout << d << std::endl;
    }
}

}//namespace al
#endif //_AL_MERGE_H_