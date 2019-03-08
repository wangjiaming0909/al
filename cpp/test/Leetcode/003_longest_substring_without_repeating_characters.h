#pragma once

#include <cstdio>
#include <vector>
#include <string>
#include <unordered_set>
#include <cassert>
#include <iostream>
using namespace std;

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

int lengthOfLongestSubstring2(string s) {
	vector<int> dict(256, -1);
	int maxLen = 0, start = -1;
	for (int i = 0; i != s.length(); i++) {
		if (dict[s[i]] > start)
			start = dict[s[i]];
		dict[s[i]] = i;
		maxLen = std::max(maxLen, i - start);
	}
	return maxLen;
}

int lengthOfLongestSubstring3(string s) {
	size_t i = 0, j = 0;
	unordered_map<char, size_t> char_set{};
	size_t longestLength = 0;
	for (; j < s.size(); j++) {
		if (char_set.count(s[j]) != 0) {
			i = std::max(char_set[s[j]], i);
		}
		longestLength = std::max(longestLength, j - i + 1);
		char_set.insert(std::pair<char, size_t>(s[j], j));
	}
	return longestLength;
}

void test_lengthOfLongestSubstring() {
	string s = "wsdafaczqwasda";
	//int ret = lengthOfLongestSubstring(s);
	int ret = lengthOfLongestSubstring3(s);

	cout << ret << endl;

}