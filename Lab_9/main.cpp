//
// Created by yrustt on 24.11.17.
//

#include <iostream>
#include <stdint.h>
#include <utility>
#include <vector>
#include <algorithm>


int main() {
    uint32_t T, n, k;

    std::cin >> T;

    for (uint32_t i = 0; i < T; ++i) {

        std::cin >> n >> k;

        std::vector<uint32_t> a(n), b(n);

        uint32_t max_a = 0, min_b = UINT32_MAX, max_b = 0;
        for (uint32_t j = 0; j < n; ++j) {
            std::cin >> a[j];

            if (a[j] > max_a) {
                max_a = a[j];
            }
        }

        for (uint32_t j = 0; j < n; ++j) {
            std::cin >> b[j];

            if (b[j] < min_b) {
                min_b = b[j];
            }
            if (b[j] > max_b) {
                max_b = b[j];
            }
        }

        double left = 0., right = max_a / min_b, min_diff = 1. / k / max_b / max_b;
        double tmp, sum, sum_ai, sum_bi;

//        std::cout << left << " " << right << std::endl;

        std::vector<std::pair<double, uint32_t>> lines(n);

        while (true) {

            sum = 0;
            tmp = (right + left) / 2;

            for (uint32_t j = 0; j < n; ++j) {
                lines[j] = std::make_pair(a[j] - tmp * b[j], j);
            }
            std::sort(lines.rbegin(), lines.rend());
            for (uint32_t j = 0; j < k; ++j) {
                sum += lines[j].first;
            }


            if (sum < 0) {
                right = tmp;
            } else {
                sum_ai = 0;
                sum_bi = 0;

                for (uint32_t j = 0; j < k; ++j) {
                    sum_ai += a[lines[j].second];
                    sum_bi += b[lines[j].second];
                }
                left = sum_ai / sum_bi;
            }


            if (right - left < min_diff) {
                for (uint32_t j = 0; j < n; ++j) {
                    lines[j] = std::make_pair(a[j] - left * b[j], j);
                }
                std::sort(lines.rbegin(), lines.rend());

                for (uint32_t j = 0; j < k; ++j) {
                    std::cout << lines[j].second + 1 << " ";
                }
                std::cout << std::endl;
                break;
            }
        }
    }

    return 0;
}