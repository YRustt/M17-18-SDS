
class SegmentTree:
    class SegmentTreeNode:
        def __init__(self, sum_odd: int, sum_even: int, num_odd: int, num_even: int, num_inc: int):
            self._sum_odd = sum_odd
            self._sum_even = sum_even
            self._num_odd = num_odd
            self._num_even = num_even
            self._num_inc = num_inc

        def __add__(self, other):
            return SegmentTree.SegmentTreeNode(
                sum_odd=self._sum_odd + other._sum_odd,
                sum_even=self._sum_even + other._sum_even,
                num_odd=self._num_odd + other._num_odd,
                num_even=self._num_even + other._num_even,
                num_inc=0
            )

        def __str__(self):
            return "<STN: {sum_odd} {sum_even} {num_odd} {num_even} {num_inc}>".format(
                sum_odd=self._sum_odd,
                sum_even=self._sum_even,
                num_odd=self._num_odd,
                num_even=self._num_even,
                num_inc=self._num_inc
            )

        def __repr__(self):
            return str(self)

    def __init__(self, ar):
        self._n = len(ar)
        self._tree = [None] * (4 * self._n)
        self._build_tree(ar, 1, 0, self._n - 1)

    def _build_tree_node(self, ar, idx):
        val = ar[idx]
        sum_odd, sum_even = (val, 0) if val % 2 else (0, val)
        num_odd, num_even = (1, 0) if val % 2 else (0, 1)
        num_inc = 0
        return SegmentTree.SegmentTreeNode(
            sum_odd=sum_odd,
            sum_even=sum_even,
            num_odd=num_odd,
            num_even=num_even,
            num_inc=num_inc
        )

    def _build_tree(self, ar, v, tl, tr):
        if tl == tr:
            self._tree[v] = self._build_tree_node(ar, tl)
        else:
            tm = (tl + tr) // 2
            self._build_tree(ar, 2 * v, tl, tm)
            self._build_tree(ar, 2 * v + 1, tm + 1, tr)
            self._tree[v] = self._tree[2 * v] + self._tree[2 * v + 1]

    def _inc(self, v, tl, tr, l, r):
        self.pushing(v)

        if l > r:
            return

        if (l == tl) and (r == tr):
            tmp = self._tree[v]

            self._tree[v] = SegmentTree.SegmentTreeNode(
                sum_odd=tmp._sum_even + tmp._num_even,
                sum_even=tmp._sum_odd + tmp._num_odd,
                num_odd=tmp._num_even,
                num_even=tmp._num_odd,
                num_inc=tmp._num_inc + 1
            )
            return

        tm = (tl + tr) // 2

        self._inc(2 * v, tl, tm, l, min(r, tm))
        self._inc(2 * v + 1, tm + 1, tr, max(l, tm + 1), r)
        self._tree[v] = self._tree[2 * v] + self._tree[2 * v + 1]

    def inc(self, left, right):
        self._inc(1, 0, self._n - 1, left, right)

    def pushing(self, v):
        def pushing_left_son(v):
            tmp, tmp_l = self._tree[v], self._tree[2 * v]

            if tmp is None or tmp_l is None:
                return

            if tmp._num_inc % 2:
                self._tree[2 * v] = SegmentTree.SegmentTreeNode(
                    sum_odd=tmp_l._sum_even + tmp_l._num_even * tmp._num_inc,
                    sum_even=tmp_l._sum_odd + tmp_l._num_odd * tmp._num_inc,
                    num_odd=tmp_l._num_even,
                    num_even=tmp_l._num_odd,
                    num_inc=tmp_l._num_inc + tmp._num_inc
                )
            else:
                self._tree[2 * v] = SegmentTree.SegmentTreeNode(
                    sum_odd=tmp_l._sum_odd + tmp_l._num_odd * tmp._num_inc,
                    sum_even=tmp_l._sum_even + tmp_l._num_even * tmp._num_inc,
                    num_odd=tmp_l._num_odd,
                    num_even=tmp_l._num_even,
                    num_inc=tmp_l._num_inc + tmp._num_inc
                )

        def pushing_right_son(v):
            tmp, tmp_r = self._tree[v], self._tree[2 * v + 1]

            if tmp is None or tmp_r is None:
                return

            if tmp._num_inc % 2:
                self._tree[2 * v + 1] = SegmentTree.SegmentTreeNode(
                    sum_odd=tmp_r._sum_even + tmp_r._num_even * tmp._num_inc,
                    sum_even=tmp_r._sum_odd + tmp_r._num_odd * tmp._num_inc,
                    num_odd=tmp_r._num_even,
                    num_even=tmp_r._num_odd,
                    num_inc=tmp_r._num_inc + tmp._num_inc
                )
            else:
                self._tree[2 * v + 1] = SegmentTree.SegmentTreeNode(
                    sum_odd=tmp_r._sum_odd + tmp_r._num_odd * tmp._num_inc,
                    sum_even=tmp_r._sum_even + tmp_r._num_even * tmp._num_inc,
                    num_odd=tmp_r._num_odd,
                    num_even=tmp_r._num_even,
                    num_inc=tmp_r._num_inc + tmp._num_inc
                )

        if self._tree[v]._num_inc > 0:
            if 2 * v >= len(self._tree):
                pass
            elif 2 * v + 1 >= len(self._tree):
                pushing_left_son(v)
            else:
                pushing_left_son(v)
                pushing_right_son(v)

            self._tree[v]._num_inc = 0

    def _set(self, v, tl, tr, pos, val):
        self.pushing(v)

        if tl == tr:
            if val % 2:
                self._tree[v] = SegmentTree.SegmentTreeNode(
                    sum_odd=val,
                    sum_even=0,
                    num_odd=1,
                    num_even=0,
                    num_inc=0
                )
            else:
                self._tree[v] = SegmentTree.SegmentTreeNode(
                    sum_odd=0,
                    sum_even=val,
                    num_odd=0,
                    num_even=1,
                    num_inc=0
                )
        else:
            tm = (tr + tl) // 2

            if pos <= tm:
                self._set(2 * v, tl, tm, pos, val)
            else:
                self._set(2 * v + 1, tm + 1, tr, pos, val)

            self._tree[v] = self._tree[2 * v] + self._tree[2 * v + 1]

    def set(self, pos, val):
        self._set(1, 0, self._n - 1, pos, val)

    def _get_sum_even(self, v, tl, tr, l, r):
        self.pushing(v)

        if l > r:
            return 0
        if (l == tl) and (r == tr):
            return self._tree[v]._sum_even

        tm = (tr + tl) // 2
        return self._get_sum_even(2 * v, tl, tm, l, min(r, tm)) + \
               self._get_sum_even(2 * v + 1, tm + 1, tr, max(l, tm + 1), r)

    def get_sum_even(self, left, right):
        return self._get_sum_even(1, 0, self._n - 1, left, right)

    def _get_sum_odd(self, v, tl, tr, l, r):
        self.pushing(v)

        if l > r:
            return 0
        if (l == tl) and (r == tr):
            return self._tree[v]._sum_odd

        tm = (tr + tl) // 2
        return self._get_sum_odd(2 * v, tl, tm, l, min(r, tm)) + \
               self._get_sum_odd(2 * v + 1, tm + 1, tr, max(l, tm + 1), r)

    def get_sum_odd(self, left, right):
        return self._get_sum_odd(1, 0, self._n - 1, left, right)
    
    def __str__(self):
        res = []
        for node in self._tree:
            res.append(str(node))

        return '\n'.join(res)

    def __repr__(self):
        return str(self)


def map_operation(tree, **kwargs):
    c = kwargs.get('c')
    if c == 1:
        p, v = kwargs.get('a'), kwargs.get('b')
        tree.set(p - 1, v)
    elif c == 2:
        l, r = kwargs.get('a'), kwargs.get('b')
        tree.inc(l - 1, r - 1)
    elif c == 3:
        l, r = kwargs.get('a'), kwargs.get('b')
        print(tree.get_sum_even(l - 1, r - 1))
    elif c == 4:
        l, r = kwargs.get('a'), kwargs.get('b')
        print(tree.get_sum_odd(l - 1, r - 1))


if __name__ == '__main__':
    n, q = list(map(int, input().split()))
    A = list(map(int, input().split()))

    tree = SegmentTree(A)

    for _ in range(q):
        c, a, b = list(map(int, input().split()))
        map_operation(
            tree,
            c=c,
            a=a,
            b=b
        )
        # print("\nc=%s a=%s b=%s" %(c, a, b))
        # print(tree)
