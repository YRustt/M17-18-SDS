//
// Created by rustam on 8.10.17.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

typedef long long type;
typedef std::pair<type, type> pair;


int main() {
    long long n, W;

    std::cin >> n >> W;

    std::vector<type> w(n), p(n);

    for (size_t i = 0; i < n; ++i) {
        std::cin >> w[i] >> p[i];
    }

    if (n == 1) {
        if (w[0] <= W) {
            std::cout << 1 << "\n" << 1 << std::endl;
        } else {
            std::cout << 0;
        }
        return 0;
    }

    int l = n / 2, r = n - l, a_size = std::pow(2, l), b_size = std::pow(2, r);

    std::vector<int> pows(b_size + 1, 0);
    for (int i = 0; i < r + 1; ++i) {
        pows[1 << i] = i;
    }


    std::vector<std::vector<long long>> b(b_size, std::vector<long long>(3, 0));
    std::vector<pair> a(a_size, std::make_pair(0, 0));

    b[1][0] = w[l];
    b[1][1] = p[l];
    b[1][2] = 1;
    for (size_t i = 2; i < b_size; ++i) {
        b[i][0] = b[i & (i - 1)][0] + w[l + pows[i - (i & (i - 1))]];
        b[i][1] = b[i & (i - 1)][1] + p[l + pows[i - (i & (i - 1))]];
        b[i][2] = i;
    }

    std::sort(b.begin(), b.end(), [](const std::vector<long long>& p1, const std::vector<long long>& p2) -> bool {
        return p1[0] < p2[0];
    });


    std::vector<long long> bp(b_size, 0);

    int tmp = 0;
    for (int i = 0; i < b_size; ++i) {
        if (b[i][1] > b[tmp][1]) {
            bp[i] = tmp = i;
        } else {
            bp[i] = tmp;
        }
    }

    a[1] = std::make_pair(w[0], p[0]);
    for (size_t i = 2; i < a_size; ++i) {
        a[i] = std::make_pair(a[i & (i - 1)].first + w[pows[i - (i & (i - 1))]],
                              a[i & (i - 1)].second + p[pows[i - (i & (i - 1))]]);
    }

    long long opt_res = 0;
    long long opt_ai = 0, opt_bi = 0;
    for (int i = 0; i < a_size; ++i) {
        long long tmp_b = W - a[i].first;

        if (tmp_b > 0) {
//            std::cout << "------" << std::endl;
//            std::cout << tmp_b << std::endl;

            int lb = 0, rb = b_size;
            while (rb - lb > 1) {
                int tmp2 = (lb + rb) / 2;
//                std::cout << tmp2 << std::endl;
                if (b[tmp2][0] > tmp_b) {
                    rb = tmp2;
                } else {
                    lb = tmp2;
                }
            }
            if (b[lb][0] > tmp_b) {
                continue;
            }

            if ((a[i].first + b[lb][0] <= W) && ((a[i].second + b[bp[lb]][1]) > opt_res)) {
                opt_res = a[i].second + b[bp[lb]][1];
                opt_ai = i;
                opt_bi = b[bp[lb]][2];

//                std::cout << opt_ai << " " <<  a[i].first << " " << a[i].second << std::endl;
//                std::cout << opt_bi << " " << b[bp[rb]][0] << " " << b[bp[rb]][1] << std::endl;
//                std::cout << opt_res << std::endl;
//                std::cout << "------" << std::endl;
            }
        }
    }

    std::vector<long long> res;
    long long k = 0;
    while (opt_ai != 0) {
        if (opt_ai % 2 != 0) {
            res.push_back(k);
        }
        opt_ai /= 2;
        k++;
    }
    k = 0;
    while (opt_bi != 0) {
        if (opt_bi % 2 != 0) {
            res.push_back(l + k);
        }
        opt_bi /= 2;
        k++;
    }

    std::cout << res.size() << "\n";
    for (int i = 0; i < res.size(); ++i) {
        std::cout << res[i] + 1 << " ";
    }

    return 0;
}