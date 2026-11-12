#include <iostream>
#include <vector>

#include "my_graph.hpp"

int main() {
  int n = 0;
  int m = 0;
  if (!(std::cin >> n >> m)) {
    return 0;
  }

  std::vector<std::vector<int>> points_list(n);
  for (int i = 0; i < m; ++i) {
    int a = 0;
    int b = 0;
    std::cin >> a >> b;
    points_list[a - 1].push_back(b - 1);
  }

  Graph my_graph(points_list);
  std::vector<int> comp(n, -1);
  int id = my_graph.GetSCC(comp);

  std::cout << my_graph.RoadsToConnect(comp, id) << '\n';

  return 0;
}
