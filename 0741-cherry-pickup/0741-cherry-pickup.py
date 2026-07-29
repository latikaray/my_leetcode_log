from functools import lru_cache

class Solution:
    def cherryPickup(self, grid):
        n = len(grid)

        @lru_cache(None)
        def dfs(r1, c1, r2):
            c2 = r1 + c1 - r2

            if (
                r1 >= n or c1 >= n or
                r2 >= n or c2 >= n or
                grid[r1][c1] == -1 or
                grid[r2][c2] == -1
            ):
                return float("-inf")

            if r1 == n - 1 and c1 == n - 1:
                return grid[r1][c1]

            cherries = grid[r1][c1]

            if (r1, c1) != (r2, c2):
                cherries += grid[r2][c2]

            cherries += max(
                dfs(r1 + 1, c1, r2 + 1),  # down, down
                dfs(r1 + 1, c1, r2),      # down, right
                dfs(r1, c1 + 1, r2 + 1),  # right, down
                dfs(r1, c1 + 1, r2)       # right, right
            )

            return cherries

        ans = dfs(0, 0, 0)
        return max(0, ans)