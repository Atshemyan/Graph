#include "ListNode.hpp"
#include <vector>
#include <iostream>
#include <stack>

class Graph
{
    std::vector<ListNode*> m_g;
    std::vector<bool> visited;
    uint m_size;

    //helpers
    void helperAddEdge(std::vector<ListNode*>&, int, int);
    void helperBFS(int index ,void (*)(int));
    void helperDFS(std::vector<ListNode*>, int index ,void (*)(int));
    void deleteEdge(int, int);
    std::vector<int> helperShortestPath(int, int);
    std::vector<std::vector<int>> sccDfs(std::vector<ListNode*>, std::stack<int> &, std::vector<bool> &);
    void processingTimeForKasarajou(int u, std::stack<int> &s, std::vector<bool> &vis);
    void helperTarjan(std::vector<ListNode*> &, int, int &, std::vector<int> &, std::vector<int> &, std::vector<bool> &, std::stack<int> &, std::vector<std::vector<int>> &sccArr, std::vector<int> &scc); //maybe tmp


public:
    Graph(size_t);
    std::vector<std::vector<int>> tarjan(int source);
    std::vector<std::vector<int>> sccByKosarajou();
    uint size();
    std::vector<ListNode*> transposeGraph();
    void bfs(void (*fun_ptr)(int));
    void dfs(void (*fun_ptr)(int));
    void addEdge(int, int);
    std::vector<int> shortestPathInUnweigthedGraph(int, int);
    uint countOfNodesAtGivenLevel(uint, uint);
    void findAllPossiblePaths(std::vector<std::vector<int>>& ,std::vector<bool>&, uint, uint, std::vector<int>& path);
    
};
