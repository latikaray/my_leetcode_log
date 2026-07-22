#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
    struct SparseTable {
        vector<vector<int>> st;
        vector<int> log2_arr;
        
        SparseTable(const vector<int>& arr) {
            int n = arr.size();
            if (n == 0) return;
            log2_arr.assign(n + 1, 0);
            for (int i = 2; i <= n; i++) log2_arr[i] = log2_arr[i / 2] + 1;
            int K = log2_arr[n] + 1;
            st.assign(n, vector<int>(K, 0));
            for (int i = 0; i < n; i++) st[i][0] = arr[i];
            for (int j = 1; j < K; j++) {
                for (int i = 0; i + (1 << j) <= n; i++) {
                    st[i][j] = max(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
                }
            }
        }
        
        int query(int L, int R) {
            if (L > R) return -1e9;
            int j = log2_arr[R - L + 1];
            return max(st[L][j], st[R - (1 << j) + 1][j]);
        }
    };

    struct ZBlock { int start, end, len; };
    struct OBlock { int u, v, zL_start, zR_end; };

public:
    vector<int> maxActiveSectionsAfterTrade(string s, vector<vector<int>>& queries) {
        int n = s.length();
        int total_ones = 0;
        for (char c : s) if (c == '1') total_ones++;

        // 1. Identify all 0-blocks
        vector<ZBlock> z_blocks;
        for (int k = 0; k < n; ) {
            if (s[k] == '0') {
                int st = k;
                while (k < n && s[k] == '0') k++;
                z_blocks.push_back({st, k - 1, k - st});
            } else {
                k++;
            }
        }
        
        vector<int> z_lens;
        vector<int> z_block_idx(n, -1);
        for (int idx = 0; idx < z_blocks.size(); idx++) {
            z_lens.push_back(z_blocks[idx].len);
            for (int k = z_blocks[idx].start; k <= z_blocks[idx].end; k++) {
                z_block_idx[k] = idx;
            }
        }
        SparseTable st_z(z_lens);

        // 2. Identify all valid 1-blocks (strictly bounded by 0s)
        vector<OBlock> o_blocks;
        vector<int> o_val1, o_val2;
        
        for (int k = 0; k < n; ) {
            if (s[k] == '1') {
                int st = k;
                while (k < n && s[k] == '1') k++;
                int en = k - 1;
                if (st > 0 && en < n - 1 && s[st - 1] == '0' && s[en + 1] == '0') {
                    int zl = z_blocks[z_block_idx[st - 1]].start;
                    int zr = z_blocks[z_block_idx[en + 1]].end;
                    o_blocks.push_back({st, en, zl, zr});
                    o_val1.push_back(-(en - st + 1));
                    o_val2.push_back((st - 1 - zl + 1) + (zr - (en + 1) + 1));
                }
            } else {
                k++;
            }
        }
        
        SparseTable st_o_val1(o_val1);
        SparseTable st_o_val2(o_val2);

        vector<int> ans;
        ans.reserve(queries.size());

        for (const auto& q : queries) {
            int L = q[0], R = q[1];
            
            // Step A: Find the max 0-block entirely within s[L...R]
            int z_max = 0;
            if (!z_blocks.empty()) {
                int low = 0, high = (int)z_blocks.size() - 1;
                int a = z_blocks.size();
                while (low <= high) {
                    int mid = low + (high - low) / 2;
                    if (z_blocks[mid].end >= L) { a = mid; high = mid - 1; } 
                    else { low = mid + 1; }
                }
                
                low = 0; high = (int)z_blocks.size() - 1;
                int b = -1;
                while (low <= high) {
                    int mid = low + (high - low) / 2;
                    if (z_blocks[mid].start <= R) { b = mid; low = mid + 1; } 
                    else { high = mid - 1; }
                }
                
                if (a <= b) {
                    if (a == b) {
                        z_max = min(R, z_blocks[a].end) - max(L, z_blocks[a].start) + 1;
                    } else {
                        int z_a = z_blocks[a].end - max(L, z_blocks[a].start) + 1;
                        int z_b = min(R, z_blocks[b].end) - z_blocks[b].start + 1;
                        int z_mid = (a + 1 <= b - 1) ? st_z.query(a + 1, b - 1) : 0;
                        z_max = max({z_a, z_b, z_mid});
                    }
                }
            }
            
            // Step B: Calculate maximum potential gain from flipping 1-blocks in s[L...R]
            int gain = 0;
            if (!o_blocks.empty()) {
                int low = 0, high = (int)o_blocks.size() - 1;
                int i = o_blocks.size();
                while (low <= high) {
                    int mid = low + (high - low) / 2;
                    if (o_blocks[mid].u >= L + 1) { i = mid; high = mid - 1; } 
                    else { low = mid + 1; }
                }
                
                low = 0; high = (int)o_blocks.size() - 1;
                int j = -1;
                while (low <= high) {
                    int mid = low + (high - low) / 2;
                    if (o_blocks[mid].v <= R - 1) { j = mid; low = mid + 1; } 
                    else { high = mid - 1; }
                }
                
                if (i <= j) {
                    int max_val1 = st_o_val1.query(i, j);
                    int max_val2 = 0;
                    
                    if (i == j) {
                        int L0 = (o_blocks[i].u - 1) - max(L, o_blocks[i].zL_start) + 1;
                        int R0 = min(R, o_blocks[i].zR_end) - (o_blocks[i].v + 1) + 1;
                        max_val2 = L0 + R0;
                    } else {
                        int L0_i = (o_blocks[i].u - 1) - max(L, o_blocks[i].zL_start) + 1;
                        int R0_i = o_blocks[i].zR_end - (o_blocks[i].v + 1) + 1;
                        
                        int L0_j = (o_blocks[j].u - 1) - o_blocks[j].zL_start + 1;
                        int R0_j = min(R, o_blocks[j].zR_end) - (o_blocks[j].v + 1) + 1;
                        
                        int val2_mid = (i + 1 <= j - 1) ? st_o_val2.query(i + 1, j - 1) : 0;
                        max_val2 = max({L0_i + R0_i, L0_j + R0_j, val2_mid});
                    }
                    gain = max(z_max + max_val1, max_val2);
                }
            }
            
            // Apply gain directly to the total number of '1's in the complete string
            ans.push_back(total_ones + gain);
        }
        
        return ans;
    }
};