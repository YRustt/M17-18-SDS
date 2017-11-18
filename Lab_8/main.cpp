//
// Created by yrustt on 18.11.17.
//

#include <iostream>
#include <stdint.h>
#include <utility>
#include <vector>
#include <stack>


void dfs(const std::vector<std::vector<uint32_t>>& graph,
         std::vector<std::pair<uint32_t, uint32_t>>& answer) {
    uint32_t n = graph.size();
    std::vector<bool> flags(n, false);
    std::vector<bool> result(n, false);
    std::vector<uint32_t> degrees(n, 0);

    std::stack<uint32_t> s;
    for (uint32_t i = 0; i < n; ++i) {
        if (!flags[i]) {
            s.push(i);
            uint32_t cur_n = 0;

            while (!s.empty()) {
                uint32_t v = s.top();
//                std::cout << "top: " << v + 1 << std::endl;

                if (!flags[v]) {
                    flags[v] = true;
                    cur_n++;

                    bool f = false;
                    for (auto &sv : graph[v]) {
                        if (!flags[sv]) {
//                            std::cout << "push: " << sv + 1 << std::endl;
                            s.push(sv);
                            f = true;
                        }
                    }
                    if (!f) {
//                        std::cout << "pop: " << v + 1 << std::endl;
                        s.pop();
                        result[v] = true;
                    }
                } else {
//                    std::cout << "pop: " << v + 1 << std::endl;

                    for (auto &sv : graph[v]) {
                        if ((!result[v]) && (degrees[sv] % 2 == 0) && (result[sv])) {
                            degrees[v]++;
                            degrees[sv]++;
                            answer.push_back(std::make_pair(v, sv));
                        }
                    }

                    result[v] = true;
                    s.pop();
                }
            }

            if (cur_n % 2 == 1) {
                std::cout << -1;
                exit(0);
            }
        }
    }
}


int main() {
    uint32_t n, m;
    std::cin >> n >> m;

    if (n % 2 == 1) {
        std::cout << -1;
        return 0;
    }

    std::vector<std::vector<uint32_t>> graph(n, std::vector<uint32_t>());
    std::vector<std::pair<uint32_t, uint32_t>> answer;

    uint32_t a, b;
    for (uint32_t i = 0; i < m; ++i) {
        std::cin >> a >> b;
        graph[a - 1].push_back(b - 1);
        graph[b - 1].push_back(a - 1);
    }

    dfs(graph, answer);

    std::cout << answer.size() << "\n";
    for (auto& p: answer) {
        std::cout << p.first + 1 << " " << p.second + 1 << "\n";
    }

    return 0;
}