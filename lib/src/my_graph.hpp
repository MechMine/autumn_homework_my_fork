#pragma once

#include <vector>

class Graph {
 public:
  std::vector<std::vector<int>> points_list;

  explicit Graph(const std::vector<std::vector<int>>& points_list)
      : points_list(points_list) {}

  void SortPart(int node, std::vector<int>& colors,
                std::vector<int>& sorted_list) {
    colors[node] = 1;  // 0 - белый, 1 - серый, 2 - чёрный

    for (int next_node : points_list[node]) {
      if (colors[next_node] == 0) {
        SortPart(next_node, colors, sorted_list);
      }
    }
    sorted_list.push_back(node);
    colors[node] = 2;
  }

  std::vector<int> SortFull() {
    std::vector<int> colors(points_list.size(), 0);
    std::vector<int> sorted_list;
    for (int i = 0; i < static_cast<int>(colors.size()); i++) {
      if (colors[i] == 0) {
        SortPart(i, colors, sorted_list);
      }
    }

    return sorted_list;
  }

  void CheckPart(int node, std::vector<int>& colors, int& indicator) {
    colors[node] = 1;  // 0 - белый, 1 - серый, 2 - чёрный
    if (points_list[node].empty()) {
      colors[node] = 2;
      return;
    }
    for (int next_node : points_list[node]) {
      if (indicator == 1) {
        return;
      }
      if (colors[next_node] == 1) {
        indicator = 1;
        return;
      }
      if (colors[next_node] == 0) {
        CheckPart(next_node, colors, indicator);
      }
    }
    colors[node] = 2;
  }

  int CheckCycles() {
    std::vector<int> colors(points_list.size(), 0);
    int indicator = 0;
    for (int i = 0; i < static_cast<int>(colors.size()); i++) {
      if (indicator == 1) {
        break;
      }
      if (colors[i] == 0) {
        CheckPart(i, colors, indicator);
      }
    }
    return indicator;
  }
  void DFS2part(int node, std::vector<int>& comp, int id) {
    comp[node] = id;
    for (int next_node : points_list[node]) {
      if (comp[next_node] == -1) {
        DFS2part(next_node, comp, id);
      }
    }
  }
  Graph Transponse() {
    std::vector<std::vector<int>> new_points_list(points_list.size(),
                                                  std::vector<int>(0));
    for (int i = 0; i < points_list.size(); i++) {
      for (auto it = points_list[i].begin(); it != points_list[i].end(); it++) {
        new_points_list[*it].push_back(i);
      }
    }
    return Graph(new_points_list);
  }
  int GetSCC(std::vector<int>& comp) {
    std::vector<int> order = SortFull();

    Graph GT = Transponse();
    int id = 0;
    for (auto it = order.rbegin(); it != order.rend(); it++) {
      if (comp[*it] == -1) {
        GT.DFS2part(*it, comp, id);
        id++;
      }
    }
    return id;
  }
  int RoadsToConnect(std::vector<int>& comp, int id) {
    if (id <= 1) {
      return 0;
    }
    std::vector<bool> out_zero(id, true);
    std::vector<bool> in_zero(id, true);
    int out_zero_count = 0;
    int in_zero_count = 0;
    int roads_amount;
    for (int u = 0; u < comp.size(); u++) {
      for (int v = 0; v < points_list[u].size(); v++) {
        if (comp[points_list[u][v]] != comp[u]) {
          out_zero[comp[u]] = false;
          in_zero[comp[points_list[u][v]]] = false;
        }
      }
    }
    for (int i = 0; i < id; i++) {
      if (out_zero[i]) {
        out_zero_count++;
      }
      if (in_zero[i]) {
        in_zero_count++;
      }
    }
    roads_amount =
        out_zero_count > in_zero_count ? out_zero_count : in_zero_count;
    return roads_amount;
  }
};
