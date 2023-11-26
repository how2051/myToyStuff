#include <iostream>
#include <vector>

using namespace std;

class SegmentTree {
private:
    vector<int> tree; // 线段树的数组表示
    int n; // 输入数组的大小

    // 递归构建线段树的函数
    void build(vector<int>& nums, int node, int left, int right) {
        if (left == right) {
            // 叶子节点表示单个元素，直接将该元素的值存储在树中
            tree[node] = nums[left];
            return;
        }

        // 不是叶子节点，则划分为更小的子区间
        int mid = left + (right - left) / 2;
        // 递归构建左子树
        build(nums, 2 * node + 1, left, mid);
        // 递归构建右子树
        build(nums, 2 * node + 2, mid + 1, right);

        // 计算当前节点的统计信息，这里以区间和为例
        tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
    }

    // 更新操作的递归函数
    void update(int node, int left, int right, int index, int val) {
        if (left == right) {
            // 叶子节点表示单个元素，直接更新该元素的值
            tree[node] = val;
            return;
        }

        int mid = left + (right - left) / 2;
        if (index <= mid) {
            // 待更新元素在左子区间内
            update(2 * node + 1, left, mid, index, val);
        } else {
            // 待更新元素在右子区间内
            update(2 * node + 2, mid + 1, right, index, val);
        }

        // 更新当前节点的统计信息，这里以区间和为例
        tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
    }

    // 查询操作的递归函数
    int query(int node, int left, int right, int l, int r) {
        if (left > r || right < l) {
            // 当前节点表示的区间完全在待查询区间之外
            // 返回统计信息的默认值，这里以0表示区间和的默认值
            return 0;
        }

        if (left >= l && right <= r) {
            // 当前节点表示的区间完全包含在待查询区间之内
            // 直接返回当前节点的统计信息
            return tree[node];
        }

        int mid = left + (right - left) / 2;
        // 递归查询左子区间和右子区间，并将结果合并，这里以区间和为例
        return query(2 * node + 1, left, mid, l, r) + query(2 * node + 2, mid + 1, right, l, r);
    }

public:
    // 构造函数
    SegmentTree(vector<int>& nums) {
        n = nums.size();
        tree.resize(4 * n); // 数组大小为 4 * n，确保足够的空间来表示线段树
        build(nums, 0, 0, n - 1); // 从根节点 0 开始构建线段树
    }

    // 更新第 index 个元素的值为 val
    void update(int index, int val) {
        update(0, 0, n - 1, index, val);
    }

    // 查询区间 [l, r] 的和
    int query(int l, int r) {
        return query(0, 0, n - 1, l, r);
    }
};

int main() {
    vector<int> nums = {1, 3, 5, 7, 9, 11};
    SegmentTree segTree(nums);

    cout << segTree.query(0, 2) << endl; // 输出9，查询区间[0, 2]的和
    segTree.update(1, 10); // 更新第1个元素为10
    cout << segTree.query(0, 2) << endl; // 输出16，更新后查询区间[0, 2]的和

	getchar();
    return 0;
}
