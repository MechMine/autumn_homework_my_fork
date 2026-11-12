#include <gtest/gtest.h>

#include <vector>

#include "my_graph.hpp"

// Вспомогательная функция проверки корректности топологической сортировки
void CheckValidOrder(const std::vector<std::vector<int>>& adj,
                     const std::vector<int>& result) {
  int n = static_cast<int>(adj.size());

  ASSERT_EQ(static_cast<int>(result.size()),
            n);  // проверка на наличие всех вершин

  // Запоминаем позицию каждой вершины в итоговом списке

  std::vector<int> pos(n, -1);
  for (int i = 0; i < n; ++i) {
    int node = result[i];
    ASSERT_GE(node, 0);
    ASSERT_LT(node, n);
    ASSERT_EQ(pos[node], -1);
    pos[node] = i;
  }

  // Проверяем главное условие: если 'a' зависит от 'b' (есть ребро a -> b),
  // то 'b' должен быть установлен раньше, чем 'a' (pos[b] < pos[a])
  for (int a = 0; a < n; ++a) {
    for (int b : adj[a]) {
      EXPECT_LT(pos[b], pos[a])
          << "Ошибка: компонент " << b << " должен стоять левее, чем " << a;
    }
  }
}

// 0 зависит от 1, 1 зависит от 2
TEST(TopologicalSortTest, SimpleChain) {
  std::vector<std::vector<int>> adj = {{1}, {2}, {}};
  Graph g(adj);

  ASSERT_EQ(g.CheckCycles(), 0);
  CheckValidOrder(adj, g.SortFull());
}

// Ромбовидный граф (несколько допустимых ответов):
// 0 зависит от 1 и 2; оба (1 и 2) зависят от 3.
// Допустимы и {3, 1, 2, 0}, и {3, 2, 1, 0}.
TEST(TopologicalSortTest, DiamondMultipleValidAnswers) {
  std::vector<std::vector<int>> adj = {{1, 2}, {3}, {3}, {}};
  Graph g(adj);

  ASSERT_EQ(g.CheckCycles(), 0);
  CheckValidOrder(adj, g.SortFull());
}

// Граф без зависимостей (любая перестановка валидна)
TEST(TopologicalSortTest, NoDependencies) {
  std::vector<std::vector<int>> adj = {{}, {}, {}, {}};
  Graph g(adj);

  ASSERT_EQ(g.CheckCycles(), 0);
  CheckValidOrder(adj, g.SortFull());
}

// Несвязные цепочки: 0 -> 1 и 2 -> 3
TEST(TopologicalSortTest, DisconnectedComponents) {
  std::vector<std::vector<int>> adj = {{1}, {}, {3}, {}};
  Graph g(adj);

  ASSERT_EQ(g.CheckCycles(), 0);
  CheckValidOrder(adj, g.SortFull());
}

// Прямой цикл: 0 -> 1 -> 0
TEST(TopologicalSortTest, DirectCycle) {
  std::vector<std::vector<int>> adj = {{1}, {0}};
  Graph g(adj);

  ASSERT_EQ(g.CheckCycles(), 1);
}

// Петля на себя: 0 -> 0
TEST(TopologicalSortTest, SelfLoop) {
  std::vector<std::vector<int>> adj = {{0}, {}};
  Graph g(adj);

  ASSERT_EQ(g.CheckCycles(), 1);
}

// 3 2
// 0 1
// 1 2
// Единственный допустимый порядок: 2 1 0
TEST(TopologicalSortTest, Sample1ExactOrder) {
  std::vector<std::vector<int>> adj(3);
  adj[0].push_back(1);
  adj[1].push_back(2);

  Graph g(adj);

  ASSERT_EQ(g.CheckCycles(), 0);
  std::vector<int> result = g.SortFull();
  CheckValidOrder(adj, result);

  std::vector<int> expected = {2, 1, 0};
  ASSERT_EQ(result, expected);
}

// Цикл 0 -> 1 -> 2 -> 0, ответ должен быть -1
TEST(TopologicalSortTest, Sample2CycleWithTail) {
  std::vector<std::vector<int>> adj(4);
  adj[0].push_back(1);
  adj[1].push_back(2);
  adj[2].push_back(0);
  adj[3].push_back(1);

  Graph g(adj);

  ASSERT_EQ(g.CheckCycles(), 1);
}
