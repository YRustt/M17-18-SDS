
def magic(tree):
    n = len(tree)
    flags, stack = [False] * n, [0]

    while stack:
        v = stack.pop()

        print(v + 1, end=' ')

        if not flags[v]:
            flags[v] = True

            for sv in tree[v]:
                if not flags[sv]:
                    stack.extend([v, sv])


if __name__ == '__main__':
    n = int(input())
    tree = [[] for _ in range(n)]

    for _ in range(n - 1):
        a, b = list(map(int, input().split()))
        tree[a - 1].append(b - 1)
        tree[b - 1].append(a - 1)

    magic(tree)
