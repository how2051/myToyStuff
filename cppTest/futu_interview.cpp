#include <iostream>
#include <random>
#include <algorithm>
#include <memory>
using namespace std;


ListNode* combineList(ListNode* list1, ListNode* list2) {
	ListNode* p1 = list1;
	ListNode* p2 = list2;
	ListNode* ret = new ListNode;
	ListNode* cur = ret;
	while(p1 != nullptr && p2 != nullptr) {
		if(p1->m_val <= p2->m_val) {
			cur->m_next = p1;
			p1 = p1->m_next;
		} else {
			cur->m_next = p2;
			p2 = p2->m_next;
		}
		cur = cur->m_next;
	}

	if(p1 != nullptr) {
		cur->m_next = p1;
	}

	if(p2 != nullptr) {
		cur->m_next = p2;
	}

	ListNode* del = ret;
	ret = ret->m_next;
	delete del;
	return ret;
}

#include <string>
#include <map>

vector<string> getKword(vector<string> words, int k) {
	vector<string> ret;
	sort(words.begin(), words.end(), [](const string& str1, const string& str2){ return str1 < str2; });
	unordered_map<string, int> table;
	for(string str : words) {
		str ==> freq;
	}

	sort(table.begin(), table.end(), [](pair<string, int> p1, pair<string, int> p2){ p1.second > p2.second;});
	for(auto it = table.begin(); it != table.end() && k > 0; it++) {
		ret.push_back(*it.first);
		k--;
	} return ret;
}



int main() {
	return 0;
}

































