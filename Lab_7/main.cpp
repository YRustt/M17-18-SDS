//
// Created by yrustt on 6.11.17.
//

#include <iostream>
#include <stdint.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <math.h>



class PersistentSegmentTree {
public:
    PersistentSegmentTree(const std::vector<uint32_t>& ar) {
        this->n = ar.size();

        uint32_t LOG_N = ceil(log2(n)) + 1;

        this->node_size = 3;
        this->layer_size = this->node_size * 2 * this->n;
        this->tree.resize(LOG_N * this->layer_size, 0);

        this->last_node_idx.reserve(LOG_N);
        for (uint32_t i = 0; i < LOG_N; ++i) {
            this->last_node_idx.push_back(i * this->layer_size);
        }

        this->build_tree(0, ar, 0, this->n - 1);
    }

    void set(uint32_t pos, uint32_t val) {
        this->_set(this->last_node_idx[0] - this->node_size, 0, this->n - 1, pos, val);
    }

    uint32_t get_sum(uint32_t root_idx, uint32_t left, uint32_t right) {
        return this->_get_sum(root_idx * this->node_size, 0, this->n - 1, left, right);
    }

private:
    uint32_t build_tree(uint32_t v, const std::vector<uint32_t>& ar, uint32_t tl, uint32_t tr) {
        if (tl == tr) {
            uint32_t cur_layer = v / this->layer_size;
            this->last_node_idx[cur_layer] += this->node_size;

            this->tree[v] = ar[tl];
            return this->tree[v];
        } else {
            uint32_t tm = (tl + tr) / 2;

            uint32_t cur_layer = v / this->layer_size;
            uint32_t left_idx = this->last_node_idx[cur_layer + 1];
            uint32_t right_idx = this->last_node_idx[cur_layer + 1] + this->node_size;

            uint32_t left = this->build_tree(left_idx, ar, tl, tm);
            uint32_t right = this->build_tree(right_idx, ar, tm + 1, tr);

            this->last_node_idx[cur_layer] += this->node_size;

            this->tree[v] = left + right;
            this->tree[v + 1] = left_idx;
            this->tree[v + 2] = right_idx;
            return this->tree[v];
        }
    }

    std::pair<uint32_t, uint32_t> _set(uint32_t v, uint32_t tl, uint32_t tr, uint32_t pos, uint32_t val) {
        if (tl == tr) {
            uint32_t cur_layer = v / this->layer_size;
            uint32_t new_v = this->last_node_idx[cur_layer];
            this->tree[new_v] = val;
            this->last_node_idx[cur_layer] += this->node_size;
            return std::make_pair(new_v, this->tree[new_v]);
        } else {
            uint32_t tm = (tr + tl) / 2;
            uint32_t cur_layer = v / this->layer_size;

            std::pair<uint32_t, uint32_t> left, right;
            if (pos <= tm) {
                left = this->_set(this->tree[v + 1], tl, tm, pos, val);

                uint32_t right_idx = this->tree[v + 2];
                uint32_t right_val = this->tree[right_idx];

                right = std::make_pair(right_idx, right_val);
            } else {
                uint32_t left_idx = this->tree[v + 1];
                uint32_t left_val = this->tree[left_idx];

                left = std::make_pair(left_idx, left_val);

                right = this->_set(this->tree[v + 2], tm + 1, tr, pos, val);
            }

            uint32_t new_v = this->last_node_idx[cur_layer];
            this->tree[new_v] = left.second + right.second;
            this->tree[new_v + 1] = left.first;
            this->tree[new_v + 2] = right.first;
            this->last_node_idx[cur_layer] += this->node_size;
            return std::make_pair(new_v, this->tree[new_v]);
        }
    };

    uint32_t _get_sum(uint32_t v, uint32_t tl, uint32_t tr, uint32_t l, uint32_t r) {
        if (l > r) {
            return 0;
        }
        if ((l == tl) && (r == tr)) {
            return this->tree[v];
        }

        uint32_t tm = (tr + tl) / 2;
        return this->_get_sum(this->tree[v + 1], tl, tm, l, std::min(r, tm)) +
               this->_get_sum(this->tree[v + 2], tm + 1, tr, std::max(l, tm + 1), r);
    }

    uint32_t n, node_size, layer_size;
    std::vector<uint32_t> last_node_idx;
    std::vector<uint32_t> tree;
};


int binary_search(std::vector<std::pair<uint32_t, uint32_t>>& ar, uint32_t x) {
    uint32_t l = 0, r = ar.size();

    while (r - 1 != l) {
        uint32_t tmp = (r + l) / 2;
        if (ar[tmp].first < x) {
            l = tmp;
        } else {
            r = tmp;
        }
    }

    if (ar[l].first >= x) {
        return -1;
    }
    return l;
}


int main() {
    uint32_t N, Q;
    std::cin >> N >> Q;

    std::vector<std::pair<uint32_t, uint32_t>> ar(N);

    for (uint32_t i = 0; i < N; ++i) {
        scanf("%u", &ar[i].first);
        ar[i].second = i;
    }

    PersistentSegmentTree tree(std::vector<uint32_t>(N, 0));

    std::sort(ar.begin(), ar.end(), [](const std::pair<uint32_t, uint32_t>& obj1, const std::pair<uint32_t, uint32_t>& obj2) {
       return obj1.first < obj2.first;
    });

    for(auto& x: ar) {
        tree.set(x.second, 1);
    }

    for (uint32_t i = 0; i < Q; ++i) {
        uint32_t l, r, x, y;
        scanf("%u %u %u %u", &l, &r, &x, &y);
        int idx_x = binary_search(ar, x);
        int idx_y = binary_search(ar, y + 1);
        printf("%u\n", tree.get_sum((uint32_t) idx_y + 1, l - 1, r - 1) - tree.get_sum((uint32_t) idx_x + 1, l - 1, r - 1));
    }

    return 0;
}