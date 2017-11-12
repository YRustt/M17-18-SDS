
class PersistentSegmentTree:
    def __init__(self, ar):
        self._n = len(ar)
        self._roots = [self._build_tree(ar, 0, self._n - 1),]

    def _build_tree(self, ar, tl, tr):
        if tl == tr:
            return ar[tl], None, None
        else:
            tm = (tl + tr) // 2
            left = self._build_tree(ar, tl, tm)
            right = self._build_tree(ar, tm + 1, tr)
            return left[0] + right[0], left, right

    def _set(self, node, tl, tr, pos, val):
        if tl == tr:
            return val, None, None
        else:
            tm = (tr + tl) // 2

            if pos <= tm:
                left = self._set(node[1], tl, tm, pos, val)
                return left[0] + node[2][0], left, node[2]
            else:
                right = self._set(node[2], tm + 1, tr, pos, val)
                return node[1][0] + right[0], node[1], right

    def set(self, pos, val):
        self._roots.append(self._set(self._roots[-1], 0, self._n - 1, pos, val))

    def _get_sum(self, node, tl, tr, l, r):
        if l > r:
            return 0
        if (l == tl) and (r == tr):
            return node[0]

        tm = (tr + tl) // 2
        return self._get_sum(node[1], tl, tm, l, min(r, tm)) + \
               self._get_sum(node[2], tm + 1, tr, max(l, tm + 1), r)

    def get_sum(self, root_idx, left, right):
        return self._get_sum(self._roots[root_idx], 0, self._n - 1, left, right)


def print_tree(tree, root_idx):
    def rec_print_node(node):
        print(node._sum)

        if node._left is not None:
            rec_print_node(node._left)

        if node._right is not None:
            rec_print_node(node._right)

    rec_print_node(tree._roots[root_idx])


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

    for x in ar:
        tree.set(x[1], 1)

    # print(ar)

    # for i in range(6):
    #     print(tree.get_sum(i, 0, len(A) - 1))

    for _ in range(Q):
        l, r, x, y = list(map(int, input().split()))

        idx_x = binary_search(ar, x)
        idx_y = binary_search(ar, y + 1)
        # print(idx_x, idx_y)
        print(tree.get_sum(idx_y + 1, l - 1, r - 1) - tree.get_sum(idx_x + 1, l - 1, r - 1))
