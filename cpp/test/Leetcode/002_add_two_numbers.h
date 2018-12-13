#pragma once
#include <cassert>
#include <string>
using std::string;
using std::to_string;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

long double list_node2long(const ListNode* node) {
	long double index = 0;
	long double ret = 0;
	while (node) {
		ret += node->val * pow(10, index);
		node = node->next;
		index++;
	}
	return ret;
}

void char_2_list_node(const char* buffer, ListNode* node) {
	char* buffer2 = new char[64];
	memset(buffer2, 0, 64);
	memcpy(buffer2, buffer, strlen(buffer));
	int i = 0;
	while (buffer2[i]) {
		if (buffer2[i] == '.') {
			//buffer2[i] = '\0';
			break;
		}
		i++;
	}
	for (int j = i; i < strlen(buffer2); j++) {
		buffer2[j] = '\0';
	}

	int size = strlen(buffer2);
	int index = size - 1;
	char* tmp = new char[2];
	memset(tmp, 0, 2);
	ListNode* currentNode = node;
	for(; index >= 0; index--){
		tmp[0] = buffer2[index];
		currentNode->val = atoi(tmp);
		if (index-1 >= 0) {
			currentNode->next = new ListNode(0);
			currentNode = currentNode->next;
		}
	}
	delete[] buffer2;
	delete[] tmp;
}

ListNode* add_two_numbers(ListNode* l1, ListNode* l2) {
	ListNode *c1 = l1, *c2 = l2, *head = nullptr, *prev = nullptr;
	short c = 0;
	while (c1 || c2 || c) {
		c += (c1 ? c1->val : 0);
		c += (c2 ? c2->val : 0);
		ListNode *cur = new ListNode(c % 10);
		c /= 10;
		if (!head) head = cur;
		if (prev) prev->next = cur;
		prev = cur;
		if (c1) c1 = c1->next;
		if (c2) c2 = c2->next;
	}
	return head;
}

void char_2_list_node_test() {
	ListNode node{ 0 };
	char *buffer = new char[3];
	memset(buffer, 0, 3);
	buffer[0] = '1';
	buffer[1] = '2';
	char_2_list_node(buffer, &node);
	assert(node.val == 2);
	assert(node.next->val == 1);
}

void add_two_numbers_test() {
	ListNode* first = new ListNode(2);
	first->next = new ListNode(4);
	first->next->next = new ListNode(3);

	ListNode* second = new ListNode(5);
	second->next = new ListNode(6);
	second->next->next = new ListNode(4);
	auto result = add_two_numbers(first, second);
	assert(result->val == 7);
	assert(result->next->val == 0);
	assert(result->next->next->val == 8);
}

void list_node2long_test() {
	ListNode node1{ 0 };
	auto ret1 = list_node2long(&node1);
	assert(ret1 == 0);
	ListNode node2{ 1 };
	node2.next = new ListNode(2);
	auto ret2 = list_node2long(&node2);
	assert(ret2 == 21);

}
void add_two_numbers_test2() {
	ListNode* first = new ListNode(1);
	first->next = new ListNode(8);
	ListNode *second = new ListNode(0);
	auto result = add_two_numbers(first, second);
	assert(result->val == 1);
	assert(result->next->val == 8);
}
void add_two_numbers_test3() {
	ListNode* first = new ListNode(9);
	first->next = new ListNode(9);

	ListNode *second = new ListNode(9);
	second->next = new ListNode(9);
	second->next->next = new ListNode(9);
	second->next->next->next = new ListNode(9);
	second->next->next->next->next = new ListNode(9);
	second->next->next->next->next->next = new ListNode(9);
	second->next->next->next->next->next->next = new ListNode(9);
	second->next->next->next->next->next->next->next = new ListNode(9);
	second->next->next->next->next->next->next->next->next = new ListNode(9);
	second->next->next->next->next->next->next->next->next->next = new ListNode(9);
	second->next->next->next->next->next->next->next->next->next->next = new ListNode(9);

	auto result = add_two_numbers(first, second);
	assert(result->val == 8);
	assert(result->next->val == 9);
	assert(result->next->next->val == 0);
	assert(result->next->next->next->val == 0);
	assert(result->next->next->next->next->val == 0);
	assert(result->next->next->next->next->next->val == 0);
	assert(result->next->next->next->next->next->next->val == 0);
	assert(result->next->next->next->next->next->next->next->val == 0);
	assert(result->next->next->next->next->next->next->next->next->val == 0);
	assert(result->next->next->next->next->next->next->next->next->next->val == 0);
	assert(result->next->next->next->next->next->next->next->next->next->next->val == 0);
	assert(result->next->next->next->next->next->next->next->next->next->next->next->val == 1);
}
