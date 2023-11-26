#include <iostream>
#include <ctime>
#include <vector>
#include <queue>
using namespace std;

vector<int> topK(int n, int k) {
	if(n < k) return {};
	vector<int> ret;
	priority_queue<int, vector<int>, less<int>> bigHeap;
	cout << "generating random val x" << n << " : " << endl;
	for(int i = 0; i < n; i++) {
		int val = rand() % 100;
		cout << val << " ";
		bigHeap.push(val);
		if(i >= k) {
			bigHeap.pop();
		}
	} cout << endl;

	for(int i = 0; i < k; i++) {
		ret.push_back(bigHeap.top());
		bigHeap.pop();
	} return ret;
}

class MeyersSingleton {
private:
	MeyersSingleton() {};
	MeyersSingleton(const MeyersSingleton&) = delete;
	MeyersSingleton& operator=(const MeyersSingleton&) = delete;

public:
	~MeyersSingleton() {};
	static MeyersSingleton& getInstance();
};

MeyersSingleton& MeyersSingleton::getInstance() {
	static MeyersSingleton s_instance;
	return s_instance;
}

typedef struct _TreeNode {
	_TreeNode* m_left;
	_TreeNode* m_right;
	int m_val;
} TreeNode;

TreeNode* tree;
int Delete(TreeNode* root, int TargetID) {
	int ret = 0;
	queue<TreeNode*> que;
	if(root) que.push(root);
	TreeNode* targetNode = nullptr;
	while(!que.empty()) {
		int size = que.size();
		for(int i = 0; i < size; i++) {
			TreeNode* cur = que.front();
			if(cur->m_val == TargetID) {
				targetNode = cur;
				break;
			} else {
				que.pop();
				if(cur->m_left) que.push(cur->m_left);
				if(cur->m_right) que.push(cur->m_right);
			}
		}
		if(targetNode) break;
	}

	if(!targetNode) return ret;
	que = queue<TreeNode*>();
	que.push(targetNode);
	while(!que.empty()) {
		TreeNode* cur = que.front();
		if(cur->m_left) que.push(cur->m_left);
		if(cur->m_right) que.push(cur->m_right);
		delete cur;
		que.pop();
		ret++;
	} return ret;
}

int main() {
	srand(time(nullptr));
	int n = 0, k = 0;
	cin >> n >> k;
	vector<int> ret = topK(8, 4);
	cout << "topK " << k << " : " << endl;
	for(int val : ret) {
		cout << val << " ";
	} cout << endl;
	return 0;
}



