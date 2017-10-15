//
// Created by rustam on 16.10.17.
//

#include <iostream>
#include <vector>

class SegmentTreeNode {
public:
    SegmentTreeNode() {
        this->sum_odd = 0;
        this->sum_even = 0;
        this->num_odd = 0;
        this->num_even = 0;
        this->num_inc = -1;
    }
    SegmentTreeNode(long long sum_odd, long long sum_even, int num_odd, int num_even, int num_inc) {
        this->sum_odd = sum_odd;
        this->sum_even = sum_even;
        this->num_odd = num_odd;
        this->num_even = num_even;
        this->num_inc = num_inc;
    }

    SegmentTreeNode operator+(const SegmentTreeNode& s) {
        return SegmentTreeNode(
                this->sum_odd + s.sum_odd,
                this->sum_even + s.sum_even,
                this->num_odd + s.num_odd,
                this->num_even + s.num_even,
                0
        );
    }

    SegmentTreeNode& operator=(const SegmentTreeNode& s) {
        this->sum_even = s.sum_even;
        this->sum_odd = s.sum_odd;
        this->num_even = s.num_even;
        this->num_odd = s.num_odd;
        this->num_inc = s.num_inc;
        return *this;
    }

    void print() {
        std::cout << "<STN "
                  << this->sum_odd << " "
                  << this->sum_even << " "
                  << this->num_odd << " "
                  << this->num_even << " "
                  << this->num_inc << "\n";
    }

    long long sum_odd, sum_even;
    int num_odd, num_even, num_inc;
};


class SegmentTree {
public:
    SegmentTree(const std::vector<long long>& ar) {
        this->n = ar.size();
        this->tree = std::vector<SegmentTreeNode>(4 * this->n);
        this->_build_tree(ar, 1, 0, this->n - 1);
    }

    void inc(int left, int right) {
        this->_inc(1, 0, this->n - 1, left, right);
    }

    void set(int pos, long long val) {
        this->_set(1, 0, this->n - 1, pos, val);
    }

    long long get_sum_even(int left, int right) {
        return this->_get_sum_even(1, 0, this->n - 1, left, right);
    }

    long long get_sum_odd(int left, int right) {
        return this->_get_sum_odd(1, 0, this->n - 1, left, right);
    }


    void print() {
        for (size_t i = 0; i < this->tree.size(); ++i) {
            this->tree[i].print();
        }
    }
private:
    SegmentTreeNode _build_tree_node(const std::vector<long long>& ar, int idx) {
        long long val = ar[idx];
        long long sum_odd, sum_even;
        int num_odd, num_even, num_inc;
        if (val % 2) {
            sum_odd = val;
            sum_even = 0;
            num_odd = 1;
            num_even = 0;
        } else {
            sum_odd = 0;
            sum_even = val;
            num_odd = 0;
            num_even = 1;
        }
        num_inc = 0;
        return SegmentTreeNode(
                sum_odd,
                sum_even,
                num_odd,
                num_even,
                num_inc
        );
    }

    void _build_tree(const std::vector<long long>& ar, int v, int tl, int tr) {
        if (tl == tr) {
            this->tree[v] = this->_build_tree_node(ar, tl);
        } else {
            int tm = (tl + tr) / 2;
            this->_build_tree(ar, 2 * v, tl, tm);
            this->_build_tree(ar, 2 * v + 1, tm + 1, tr);
            this->tree[v] = this->tree[2 * v] + this->tree[2 * v + 1];
        }
    }

    void _inc(int v, int tl, int tr, int l, int r) {
        this->pushing(v);

        if (l > r) {
            return;
        }

        if ((l == tl) && (r == tr)) {
            const SegmentTreeNode tmp = this->tree[v];
            this->tree[v] = SegmentTreeNode(
                    tmp.sum_even + tmp.num_even,
                    tmp.sum_odd + tmp.num_odd,
                    tmp.num_even,
                    tmp.num_odd,
                    tmp.num_inc + 1
            );
            return;
        }

        int tm = (tl + tr) / 2;

        this->_inc(2 * v, tl, tm, l, std::min(r, tm));
        this->_inc(2 * v + 1, tm + 1, tr, std::max(l, tm + 1), r);
        this->tree[v] = this->tree[2 * v] + this->tree[2 * v + 1];
    }

    void pushing_left_son(int v) {
        const SegmentTreeNode tmp = this->tree[v], tmp_l = this->tree[2 * v];

        if (tmp.num_inc == -1 || tmp_l.num_inc == -1) {
            return;
        }

        if (tmp.num_inc % 2) {
            this->tree[2 * v] = SegmentTreeNode(
                    tmp_l.sum_even + tmp_l.num_even * tmp.num_inc,
                    tmp_l.sum_odd + tmp_l.num_odd * tmp.num_inc,
                    tmp_l.num_even,
                    tmp_l.num_odd,
                    tmp_l.num_inc + tmp.num_inc
            );
        } else {
            this->tree[2 * v] = SegmentTreeNode(
                    tmp_l.sum_odd + tmp_l.num_odd * tmp.num_inc,
                    tmp_l.sum_even + tmp_l.num_even * tmp.num_inc,
                    tmp_l.num_odd,
                    tmp_l.num_even,
                    tmp_l.num_inc + tmp.num_inc
            );
        }
    }

    void pushing_right_son(int v) {
        const SegmentTreeNode tmp = this->tree[v], tmp_r = this->tree[2 * v + 1];

        if (tmp.num_inc == -1 || tmp_r.num_inc == -1) {
            return;
        }

        if (tmp.num_inc % 2) {
            this->tree[2 * v + 1] = SegmentTreeNode(
                    tmp_r.sum_even + tmp_r.num_even * tmp.num_inc,
                    tmp_r.sum_odd + tmp_r.num_odd * tmp.num_inc,
                    tmp_r.num_even,
                    tmp_r.num_odd,
                    tmp_r.num_inc + tmp.num_inc
            );
        } else {
            this->tree[2 * v + 1] = SegmentTreeNode(
                    tmp_r.sum_odd + tmp_r.num_odd * tmp.num_inc,
                    tmp_r.sum_even + tmp_r.num_even * tmp.num_inc,
                    tmp_r.num_odd,
                    tmp_r.num_even,
                    tmp_r.num_inc + tmp.num_inc
            );
        }
    }

    void pushing(int v) {
        if (2 * v >= this->tree.size()) {
            return;
        }
        if (2 * v + 1 >= this->tree.size()) {
            this->pushing_left_son(v);
            return;
        }
        pushing_left_son(v);
        pushing_right_son(v);

        this->tree[v].num_inc = 0;
    }

    void _set(int v, int tl, int tr, int pos, long long val) {
        this->pushing(v);

        if (tl == tr) {
            if (val % 2) {
                this->tree[v] = SegmentTreeNode(
                        val, 0, 1, 0, 0
                );
            } else {
                this->tree[v] = SegmentTreeNode(
                        0, val, 0, 1, 0
                );
            }
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

    long long _get_sum_even(int v, int tl, int tr, int l, int r) {
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

    long long _get_sum_odd(int v, int tl, int tr, int l, int r) {
        this->pushing(v);

        if (l > r) {
            return 0;
        }
        if ((l == tl) && (r == tr)) {
            return this->tree[v].sum_odd;
        }

        int tm = (tr + tl) / 2;
        return this->_get_sum_odd(2 * v, tl, tm, l, std::min(r, tm)) +
               this->_get_sum_odd(2 * v + 1, tm + 1, tr, std::max(l, tm + 1), r);
    }

    int n;
    std::vector<SegmentTreeNode> tree;
};


void map_operation(SegmentTree& tree, int c, int a, int b) {
    switch(c){
        case 1:
            tree.set(a - 1, b);
            break;
        case 2:
            tree.inc(a - 1, b - 1);
            break;
        case 3:
            std::cout << tree.get_sum_even(a - 1, b - 1) << "\n";
            break;
        case 4:
            std::cout << tree.get_sum_odd(a - 1, b - 1) << "\n";
            break;
        default:
            break;
    }

}


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