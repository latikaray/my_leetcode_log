class Solution {
    public int[] gcdValues(int[] nums, long[] queries) {
        int mx = 0;
        for (int x : nums) mx = Math.max(mx, x);

        int[] freq = new int[mx + 1];
        for (int x : nums) freq[x]++;

        long[] gcdCnt = new long[mx + 1];

        // Inclusion-Exclusion to count pairs having GCD exactly i
        for (int i = mx; i >= 1; i--) {
            long cnt = 0;
            for (int j = i; j <= mx; j += i) {
                cnt += freq[j];
                gcdCnt[i] -= gcdCnt[j];
            }
            gcdCnt[i] += cnt * (cnt - 1) / 2;
        }

        // Prefix sum: pairs with GCD <= i
        for (int i = 2; i <= mx; i++) {
            gcdCnt[i] += gcdCnt[i - 1];
        }

        int[] ans = new int[queries.length];
        for (int i = 0; i < queries.length; i++) {
            ans[i] = upperBound(gcdCnt, queries[i]);
        }

        return ans;
    }

    private int upperBound(long[] arr, long target) {
        int l = 0, r = arr.length - 1;
        while (l < r) {
            int mid = (l + r) >>> 1;
            if (arr[mid] > target) {
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        return l;
    }
}