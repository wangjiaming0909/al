#pragma once
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>

using std::unordered_map;
using std::vector;
using std::find;
//4ms
vector<int> two_sum(vector<int> nums, int target) {
	vector<int> result{};
	int a = 0;
	unordered_map<int, int> map{};
	for (size_t i = 0; i < nums.size(); i++) {
		map.emplace(nums[i], i);
		a = target - nums[i];
		auto iterator = map.find(a);
		if (iterator != map.end() && iterator->second != i) {
			result.push_back(i);
			result.push_back(iterator->second);
			break;
		}
	}
	return result;
}


void two_sum_test() {
	vector<int> nums = { 3,2,4 };
	int target = 6;
	auto result = two_sum(nums, target);
	if ((result[0] == 1 && result[1] == 2) || (result[0] == 2 && result[1] == 1))
		assert(true);
	else
		assert(false);
}