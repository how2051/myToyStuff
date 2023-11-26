#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

// Definition for a binary tree node
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

//preStart 和 preEnd 描述的是子树的范围，相对于前序遍历来说
//inStart 和 inEnd 描述的是子树的范围，相对于中序遍历来说
//返回的是当前节点，即以根节点为起始，构建当前节点并试图插入左右子树，向下递归
TreeNode* buildTreeHelper(const vector<int>& preorder, const vector<int>& inorder,
                          int preStart, int preEnd, int inStart, int inEnd,
                          unordered_map<int, int>& indexMap) {
    if (preStart > preEnd || inStart > inEnd) {
        return nullptr;
    }

    int rootVal = preorder[preStart];
    TreeNode* root = new TreeNode(rootVal);

    int rootIndex = indexMap[rootVal];	//找到根节点在中序遍历中的位置
    int leftSize = rootIndex - inStart;

	//rootIndex 是对于中序遍历的数组来说的，所以仅在中序遍历时有用
	//对于前序遍历来说，只能通过 leftSize 来得出左子树的长度，从而得出前序遍历的末端
    root->left = buildTreeHelper(preorder, inorder, preStart + 1, preStart + leftSize,
                                 inStart, rootIndex - 1, indexMap);	//描述左子树的左右范围
    root->right = buildTreeHelper(preorder, inorder, preStart + leftSize + 1, preEnd,
                                  rootIndex + 1, inEnd, indexMap);	//描述右子树的左右范围

    return root;
}

TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    int n = preorder.size();

    unordered_map<int, int> indexMap;
    for (int i = 0; i < n; i++) {
        indexMap[inorder[i]] = i;
    }

    return buildTreeHelper(preorder, inorder, 0, n - 1, 0, n - 1, indexMap);
}

// Helper function to print the tree (in-order traversal)
void printTree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    printTree(root->left);
    cout << root->val << " ";
    printTree(root->right);
}

int main() {
    vector<int> preorder = {1, 2, 4, 5, 3, 6};
    vector<int> inorder = {4, 2, 5, 1, 6, 3};

    TreeNode* root = buildTree(preorder, inorder);

    cout << "In-order traversal of the constructed tree: ";
    printTree(root);
    cout << endl;

    return 0;
}

