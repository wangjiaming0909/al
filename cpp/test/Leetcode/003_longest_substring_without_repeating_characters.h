#pragma once

#include <cstdio>
#include <vector>
#include <string>
#include <unordered_set>
#include <cassert>

using std::string;
using std::vector;
using std::unordered_set;

int lengthOfLongestSubstring(string s) {
	int len = s.size();
	int max_len = 0;
	unordered_set<char> str_set{};
	int start = 0, end = 0;
	for (int i = start; i < len; i++) {
		for (int j = i; j < len; j++) {
			if (str_set.count(s.at(j)) == 0) {
				str_set.insert(s.at(j));
				end = j+1;
				continue;
			} 
			max_len = std::max(max_len, j - i);
			start++;
			break;
		}
		max_len = std::max(max_len, end - i);
		str_set.clear();
	}
	return max_len;
}

void test_lengthOfLongestSubstring() {
	string s = "w";
	int ret = lengthOfLongestSubstring(s);
	assert(ret == 1);
}