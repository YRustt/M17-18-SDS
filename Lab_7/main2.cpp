//
// Created by rustam on 16.10.17.
//

#include <iostream>
#include <vector>

class SegmentTreeNode {
public:
    SegmentTreeNode(): val(0), left(NULL), right(NULL) {}
    SegmentTreeNode(int val, SegmentTreeNode* left, SegmentTreeNode* right) {
        this->val = val;
        this->left = left;
        this->right = right;
    }

    SegmentTreeNode operator+(const SegmentTreeNode& s) {
        return SegmentTreeNode(
                this->val + s.val,
                this,
                &s
        );
    }

    SegmentTreeNode& operator=(const SegmentTreeNode& s) {
        this->val = s.val;
        this->left = s.left;
        this->right = s.right;
        return *this;
    }
private:
    int val;
    SegmentTreeNode *left, *right;
};


class PersistentSegmentTree {
public:
    PersistentSegmentTree(const std::vector<uint32_t>& ar) {
        this->n = ar.size();
        this->tree = std::vector<std::vector<SegmentTreeNode>>(1, std::vector<SegmentTreeNode>(4 * this->n));
        this->_build_tree(ar, 1, 0, this->n - 1);
    }

    void set(uint32_t pos, uint32_t val) {
        this->tree.push_back(std::vector<uint32_t>());
        this->_set(1, 0, this->n - 1, pos, val);
    }

    uint32_t get_sum(uint32_t left, uint32_t right) {
        return this->_get_sum(1, 0, this->n - 1, left, right);
    }
private:
    SegmentTreeNode _build_tree_node(const std::vector<uint32_t>& ar, uint32_t idx) {
        uint32_t val = ar[idx];
        return SegmentTreeNode(
                val,
                NULL,
                NULL
        );
    }

    void _build_tree(const std::vector<uint32_t>& ar, uint32_t v, uint32_t tl, uint32_t tr) {
        if (tl == tr) {
            this->tree[v] = this->_build_tree_node(ar, tl);
        } else {
            int tm = (tl + tr) / 2;
            this->_build_tree(ar, 2 * v, tl, tm);
            this->_build_tree(ar, 2 * v + 1, tm + 1, tr);
            this->tree[0][v] = this->tree[0][2 * v] + this->tree[0][2 * v + 1];
        }
    }

    void _set(int v, int tl, int tr, int pos, long long val) {
        if (tl == tr) {
            this->tree.back().push_back(SegmentTreeNode(val, NULL, NULL));
        } else {
            int tm = (tr + tl) / 2;

            if (pos <= tm) {
                this->_set(2 * v, tl, tm, pos, val);


            } else {
                this->_set(2 * v + 1, tm + 1, tr, pos, val);
            }

            this->tree[v] = this->tree[2 * v] + this->tree[2 * v + 1];
        }
    }

    long long _get_sum(int v, int tl, int tr, int l, int r) {
        this->pushing(v);

        if (l > r) {
            return 0;
        }
        if ((l == tl) && (r == tr)) {
            return this->tree[v].sum_even;
        }

        int tm = (tr + tl) / 2;
        return this->_get_sum_even(2 * v, tl, tm, l, std::min(r, tm)) +
               this->_get_sum_even(2 * v + 1, tm + 1, tr, std::max(l, tm + 1), r);
    }

    int n;
    std::vector<std::vector<SegmentTreeNode>> tree;
};


int main() {
    int n, q;
    std::cin >> n >> q;


    std::vector<long long> A(n, 0);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> A[i];
    }

    SegmentTree tree(A);

    int c, a, b;
    for (size_t i = 0; i < q; ++i) {
        std::cin >> c >> a >> b;
        map_operation(tree, c, a, b);

//        std::cout << c << " " << a << " " << b << "\n";
//        tree.print();
//        std::cout << "\n";
    }

    return 0;
}