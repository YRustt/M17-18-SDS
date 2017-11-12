import math


class PersistentSegmentTree:
    def __init__(self, ar):
        self._n = len(ar)

        LOG_N = int(math.ceil(math.log(self._n, 2))) + 1

        self._node_size = 3
        self._layer_size = self._node_size * 2 * self._n
        self._tree = [None] * (LOG_N * self._layer_size)
        self._last_node_idx = [i * self._layer_size for i in range(LOG_N)]

        self._build_tree(0, ar, 0, self._n - 1)

    def _build_tree(self, v, ar, tl, tr):
        if tl == tr:
            cur_layer = v // self._layer_size
            self._last_node_idx[cur_layer] += self._node_size

            self._tree[v] = ar[tl]
            return self._tree[v]
        else:
            tm = (tl + tr) // 2

            cur_layer = v // self._layer_size
            left_idx = self._last_node_idx[cur_layer + 1]
            right_idx = self._last_node_idx[cur_layer + 1] + self._node_size

            left = self._build_tree(left_idx, ar, tl, tm)
            right = self._build_tree(right_idx, ar, tm + 1, tr)

            self._last_node_idx[cur_layer] += self._node_size

            self._tree[v] = left + right
            self._tree[v + 1] = left_idx
            self._tree[v + 2] = right_idx
            return self._tree[v]

    def _set(self, v, tl, tr, pos, val):
        if tl == tr:
            cur_layer = v // self._layer_size
            new_v = self._last_node_idx[cur_layer]
            self._tree[new_v] = val
            self._last_node_idx[cur_layer] += self._node_size
            return new_v, self._tree[new_v]
        else:
            tm = (tr + tl) // 2

            cur_layer = v // self._layer_size

            if pos <= tm:
                left_idx, left_val = self._set(self._tree[v + 1], tl, tm, pos, val)

                right_idx = self._tree[v + 2]
                right_val = self._tree[right_idx]
            else:
                left_idx = self._tree[v + 1]
                left_val = self._tree[left_idx]

                right_idx, right_val = self._set(self._tree[v + 2], tm + 1, tr, pos, val)

            new_v = self._last_node_idx[cur_layer]
            self._tree[new_v] = left_val + right_val
            self._tree[new_v + 1] = left_idx
            self._tree[new_v + 2] = right_idx
            self._last_node_idx[cur_layer] += self._node_size
            return new_v, self._tree[new_v]

    def set(self, pos, val):
        self._set(self._last_node_idx[0] - self._node_size, 0, self._n - 1, pos, val)

    def _get_sum(self, v, tl, tr, l, r):
        if l > r:
            return 0
        if (l == tl) and (r == tr):
            return self._tree[v]

        tm = (tr + tl) // 2
        return self._get_sum(self._tree[v + 1], tl, tm, l, min(r, tm)) + \
               self._get_sum(self._tree[v + 2], tm + 1, tr, max(l, tm + 1), r)

    def get_sum(self, root_idx, left, right):
        return self._get_sum(root_idx * self._node_size, 0, self._n - 1, left, right)


def print_tree(tree):
    print("\nTree:")
    for idx, val in enumerate(tree._tree):
        print(val, end=' ')
        if idx % tree._layer_size == tree._layer_size - 1:
            print()
    print("End Tree\n")


def binary_search(ar, x):
    l, r = 0, len(ar)

    while r - 1 != l:
        tmp = (r + l) // 2
        if ar[tmp][0] < x:
            l = tmp
        else:
            r = tmp

    if ar[l][0] >= x:
        return -1

    return l


if __name__ == '__main__':
    N, Q = list(map(int, input().split()))
    A = list(map(int, input().split()))

    tree = PersistentSegmentTree([0] * len(A))

    ar = sorted(zip(A, range(N)), key=lambda x: x[0])

    # print_tree(tree)

    for x in ar:
        tree.set(x[1], 1)
        # print_tree(tree)

    # print(ar)

    # for i in range(6):
    #     print(tree.get_sum(i, 0, len(A) - 1))

    for _ in range(Q):
        l, r, x, y = list(map(int, input().split()))

        idx_x = binary_search(ar, x)
        idx_y = binary_search(ar, y + 1)
        # print(idx_x, idx_y)
        print(tree.get_sum(idx_y + 1, l - 1, r - 1) - tree.get_sum(idx_x + 1, l - 1, r - 1))
