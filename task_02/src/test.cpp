#include <gtest/gtest.h>

#include <vector>

#include "my_graph.hpp"

int SolveRoads(int n, const std::vector<std::pair<int, int>>& edges) {
  std::vector<std::vector<int>> adj(n);
  for (const auto& [u, v] : edges) {
    adj[u].push_back(v);
  }
  Graph g(adj);
  std::vector<int> comp(n, -1);
  int id = g.GetSCC(comp);
  return g.RoadsToConnect(comp, id);
}

// 1. Одна вершина с петлёй -> уже 1 квартал, дорог не нужно (0)
TEST(RoadsToConnectTest, SingleNodeSelfLoop) {
  EXPECT_EQ(SolveRoads(1, {{0, 0}}), 0);
}

// 2. Весь граф — один большой цикл -> 0 дорог
TEST(RoadsToConnectTest, SingleCycle) {
  EXPECT_EQ(SolveRoads(3, {{0, 1}, {1, 2}, {2, 0}}), 0);
}

// 3. Цепочка из 3 компонент: 0 -> 1 -> 2 -> нужна 1 дорога (2 -> 0)
TEST(RoadsToConnectTest, ThreeSCCsChain) {
  EXPECT_EQ(SolveRoads(3, {{0, 1}, {1, 2}}), 1);
}

// 4. Две изолированные компоненты (без дорог) -> нужно 2 дороги
TEST(RoadsToConnectTest, TwoSCCsDisconnected) {
  EXPECT_EQ(SolveRoads(2, {}), 2);
}

// 5. Несколько истоков и стоков (например, 0 -> 2 и 1 -> 2)
// Истоки: {0}, {1} (in_zero = 2); Сток: {2} (out_zero = 1) -> ответ max(2, 1) =
// 2
TEST(RoadsToConnectTest, MultipleSourcesOneSink) {
  EXPECT_EQ(SolveRoads(3, {{0, 2}, {1, 2}}), 2);
}

// 6. Две компоненты сильной связности, соединённые в одну сторону
// Цикл (0 <-> 1) -> Цикл (2 <-> 3) -> нужна 1 дорога
TEST(RoadsToConnectTest, TwoCyclesConnectedOneWay) {
  EXPECT_EQ(SolveRoads(4, {{0, 1}, {1, 0}, {1, 2}, {2, 3}, {3, 2}}), 1);
}
