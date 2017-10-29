

with open('test.txt', 'w') as f:
    f.write('100000\n')
    for i in range(100000):
        f.write('%d %d\n' % (i + 1, i + 2))
