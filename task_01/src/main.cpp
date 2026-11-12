#include <iostream>
#include <vector>

#include "my_graph.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<std::vector<int>> points_list(n);
  int a, b;
  for (int i = 0; i < m; i++) {
    std::cin >> a >> b;
    points_list[a].push_back(b);  // формирование списка смежностей
  }

  Graph my_graph(points_list);
  if (my_graph.CheckCycles() == 0) {
    std::vector<int> sorted_list = my_graph.SortFull();
    for (int i : sorted_list) {
      std::cout << i << ' ';
    }
  } else {
    std::cout << -1;
  }

  return 0;
}
