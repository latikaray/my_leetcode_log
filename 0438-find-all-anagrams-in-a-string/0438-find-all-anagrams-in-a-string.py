from collections import Counter

class Solution:
    def findAnagrams(self, s: str, p: str) -> List[int]:
        if len(p) > len(s):
            return []

        p_count = Counter(p)
        window = Counter(s[:len(p)])

        ans = []

        if window == p_count:
            ans.append(0)

        for i in range(len(p), len(s)):
            window[s[i]] += 1
            window[s[i - len(p)]] -= 1

            if window[s[i - len(p)]] == 0:
                del window[s[i - len(p)]]

            if window == p_count:
                ans.append(i - len(p) + 1)

        return ans