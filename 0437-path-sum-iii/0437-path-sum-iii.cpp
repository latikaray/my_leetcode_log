/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

class Solution {
public:

    long dfs(TreeNode* node, long currentSum, int targetSum,
             unordered_map<long, int>& prefixSum) {

        if (node == nullptr) {
            return 0;
        }

        currentSum += node->val;

        long count = prefixSum[currentSum - targetSum];

        prefixSum[currentSum]++;

        count += dfs(node->left, currentSum, targetSum, prefixSum);
        count += dfs(node->right, currentSum, targetSum, prefixSum);

        prefixSum[currentSum]--;

        return count;
    }

    int pathSum(TreeNode* root, int targetSum) {

        unordered_map<long, int> prefixSum;

        prefixSum[0] = 1;

        return dfs(root, 0, targetSum, prefixSum);
    }
};