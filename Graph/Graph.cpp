#include "Graph.hpp"
#include <queue>



Graph::Graph(size_t size)
    : m_g(size) 
    , visited(size, false)
    , m_size(size)
    
{}


void Graph::addEdge(int u, int v) {
    helperAddEdge(m_g, u, v);
}

void Graph::bfs(void (*unaryFunction)(int)) {
    for (uint i = 0; i < m_g.size(); ++i)
    {
        if (!visited[i]) {
            helperBFS(i, unaryFunction);
        }
    }

    std::fill(visited.begin(), visited.end(), false);
}

std::vector<int> Graph::shortestPathInUnweigthedGraph(int u, int v) {
    std::vector<int> vec;
    for (uint i = 0; i < m_g.size(); ++i)
    {
        if (!visited[i]) {
            vec = helperShortestPath(i, v);
            if (vec.size() > 0) {
                return vec;
            }
        }
    }

    std::fill(visited.begin(), visited.end(), false);
    return vec;
}

std::vector<std::vector<int>> Graph::sccByKosarajou() {
    
    std::stack<int> stack;
    std::vector<bool> vis(size(), false);

    for (uint i = 0; i < m_size; ++i) {
        if (!vis[i]) {
            processingTimeForKasarajou(i, stack, vis);
        }
    }

    std::fill(vis.begin(), vis.end(), false);

    std::vector<ListNode*> transposedGraph(std::move(transposeGraph()));
    
    return sccDfs(transposedGraph, stack, vis);
}



void Graph::findAllPossiblePaths(std::vector<std::vector<int>>& ans, std::vector<bool>& visited ,uint u, uint v, std::vector<int>& path) {
    visited[u] = true;

    path.push_back(u);

    if (u == v) {
        ans.push_back(path);
    }

    else {
        ListNode *curr = m_g[u];
        while (curr) {
            findAllPossiblePaths(ans, visited, curr->val, v, path);
            curr = curr->next;
        }
        
    }

    visited[u] = false;
    path.pop_back();
    
}

std::vector<std::vector<int>> Graph::tarjan(int source) {
    std::vector<std::vector<int>> sccArr;
    std::vector<int> scc;
    std::vector<bool> onStack(m_size, false);
    std::vector<int> lowLinkValues(m_size, -1);
    std::vector<int> ids(m_size, -1);
    std::stack<int> stack;
    helperTarjan(m_g, source, source, lowLinkValues, ids, onStack, stack, sccArr, scc);
    return sccArr;
}

uint Graph::size() {
    return m_size;
}

uint Graph::countOfNodesAtGivenLevel(uint start, uint lvl) {
    if (m_g[start] == nullptr) {
        return -1;
    }
    uint lvlCounter {};
    uint nodeCounter {};
    std::queue<int> q;

    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        

        ListNode* tmp = m_g[u];
        while (tmp) {
            int v = tmp->val;
            if (!visited[v]) {
                q.push(v);
                visited[v] = true;
            }
            tmp = tmp->next;
            ++nodeCounter;
        }
        if (lvlCounter == lvl) {
            return nodeCounter;
        }
        nodeCounter = 0;
        ++lvlCounter;
    }
    return lvlCounter;
}

std::vector<int> Graph::helperShortestPath(int start, int d) {

    if (m_g[start] == nullptr) {
        return {};  // Skip if the list is empty
    }
    std::vector<int> parent(m_size, -1);
    std::vector<int> backtrackingPath;

    std::queue<int> q;

    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int u = q.front();
        
        q.pop();
        ListNode* tmp = m_g[u];
        while (tmp) {
            int t = tmp->val;
            if (!visited[t]) {
                q.push(t);
                visited[t] = true;
                parent[t] = u;

                if (t == d) {
                    int current = d;
                    while (current != -1) {
                        backtrackingPath.push_back(current);
                        current = parent[current];
                    }
                    std::reverse(backtrackingPath.begin(), backtrackingPath.end());
                    return backtrackingPath;
                }

            }
            tmp = tmp->next;
        }
    }
    return backtrackingPath;
}

std::vector<ListNode*> Graph::transposeGraph() {
    std::vector<ListNode*> newGraph(m_g.size());
    for (uint i = 0; i < m_size; ++i) {
        ListNode *curr = m_g[i];
        while (curr) {
            helperAddEdge(newGraph, curr->val, i);
            curr = curr->next;
        }
    }
    return newGraph;
    
}


//helper realizations

void Graph::helperTarjan(std::vector<ListNode*> &g, int source, int &id, std::vector<int> &ll, std::vector<int> &ids, std::vector<bool> &onStack, std::stack<int> &stack, std::vector<std::vector<int>> &sccArr, std::vector<int> &scc) {
    ids[source] = ll[source] = id++;
    onStack[source] = true;
    stack.push(source);

    ListNode *curr = g[source];
    while (curr) {
        //if (!visited)
        if (ids[curr->val] == -1) { 
            helperTarjan(g, curr->val, id, ll, ids, onStack, stack, sccArr, scc);
            ll[source] = std::min(ll[source], ll[curr->val]);
        }
        else if (onStack[curr->val]) {
            ll[source] = std::min(ll[source], ll[curr->val]);
        }
        curr = curr->next;
    }

    if (ll[source] == ids[source]) {
        for (int t = stack.top(); ; t = stack.top())
        {
            onStack[t] = false;
            stack.pop();
            scc.push_back(t);
            if (t == source) {
                break;
            }
        }
        sccArr.push_back(scc);
        
        scc = std::vector<int>();
    }

}

void Graph::processingTimeForKasarajou(int u, std::stack<int> &s, std::vector<bool> &vis) {
    vis[u] = true;
    ListNode *curr = m_g[u];
    while (curr) {
        if (!vis[curr->val]) {
            processingTimeForKasarajou(curr->val, s, vis);
        }
        curr = curr->next;
    }
    
    s.push(u);

}

std::vector<std::vector<int>> Graph::sccDfs(std::vector<ListNode*> g, std::stack<int> &s, std::vector<bool> &vis) {

    std::vector<std::vector<int>> sccArr;
    std::vector<int> scc;

    while (!s.empty()) {
        int start = s.top();
        s.pop();
        if (g[start] == nullptr) {
            continue;
        }
        std::stack<int> stack;
        stack.push(start);
        if (!vis[start]) {
            scc.push_back(start);
        }

        vis[start] = true;

        while (!stack.empty()) {
            int u = stack.top();
            stack.pop();

            ListNode* tmp = g[u];
            while (tmp) {
                int v = tmp->val;
                if (!vis[v]) {
                    scc.push_back(v);
                    stack.push(v);
                    vis[v] = true;
                }
                tmp = tmp->next;
            }
        }
        if (!scc.empty()){
            sccArr.push_back(scc);
        }
        scc = std::vector<int>();
    }
    

    return sccArr;
}




void Graph::deleteEdge(int index, int number) {
    ListNode *curr = m_g[index];
    ListNode *prev = nullptr;

    while (curr) {
        if (curr->val == number) {
            if (!prev) {
                m_g[index] = curr->next;
                delete curr;
            }
            else {
                prev->next = curr->next;
                delete curr;
            }
        }
        prev = curr;
        curr = curr->next;
    }
    
    
} 



void Graph::helperBFS(int start ,void (*unaryFunction)(int)) {
    if (m_g[start] == nullptr) {
        return;  // Skip if the list is empty
    }
    std::queue<int> q;

    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        unaryFunction(u);

        ListNode* tmp = m_g[u];
        while (tmp) {
            int v = tmp->val;
            if (!visited[v]) {
                q.push(v);
                visited[v] = true;
            }
            tmp = tmp->next;
        }
    }
}

void Graph::dfs(void (*unaryFunction)(int)) {
    for (uint i = 0; i < m_g.size(); ++i)
    {
        if (!visited[i]) {
            helperDFS(m_g, i, unaryFunction);
        }
    }

    std::fill(visited.begin(), visited.end(), false);
}

void Graph::helperDFS(std::vector<ListNode*> g, int start ,void (*unaryFunction)(int)) {
    if (g[start] == nullptr) {
        return;  // Skip if the list is empty
    }
    std::stack<int> s;
    s.push(start);
    visited[start] = true;

    while (!s.empty()) {
        int u = s.top();
        s.pop();

        unaryFunction(u);

        ListNode* tmp = g[u];
        while (tmp) {
            int v = tmp->val;
            if (!visited[v]) {
                s.push(v);
                visited[v] = true;
            }
            tmp = tmp->next;
        }
    }
}


void Graph::helperAddEdge(std::vector<ListNode*> &graph, int u, int v) {
    ListNode* curr = graph[u];
    while (curr) {
        if (curr->val == v) {
            return;
        }
        curr = curr->next;
    }

    if (!graph[u]) graph[u] = new ListNode(v);
    else
    {
        ListNode *curr = graph[u];
        while (curr->next) curr = curr->next;
        curr->next = new ListNode(v);
    }
}




