class Solution {
public:
    int countCompleteComponents(int n, vector<vector<int>>& edges) {
        vector<vector<int>> g(n);
        for (auto &e : edges) {
            g[e[0]].push_back(e[1]);
            g[e[1]].push_back(e[0]);
        }

        vector<int> vis(n, 0);
        int ans = 0;

        function<void(int, int&, int&)> dfs = [&](int u, int &nodes, int &degSum) {
            vis[u] = 1;
            nodes++;
            degSum += g[u].size();

            for (int v : g[u])
                if (!vis[v])
                    dfs(v, nodes, degSum);
        };

        for (int i = 0; i < n; i++) {
            if (!vis[i]) {
                int nodes = 0, degSum = 0;
                dfs(i, nodes, degSum);

                if (degSum == nodes * (nodes - 1))
                    ans++;
            }
        }

        return ans;
    }
};