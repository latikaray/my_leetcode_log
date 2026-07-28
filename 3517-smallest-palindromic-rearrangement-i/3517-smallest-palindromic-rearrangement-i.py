from collections import Counter

class Solution:
    def smallestPalindrome(self, s: str) -> str:
        cnt = Counter(s)

        left = []
        middle = ""

        for i in range(26):
            ch = chr(ord('a') + i)

            left.append(ch * (cnt[ch] // 2))

            if cnt[ch] % 2 == 1:
                middle = ch

        left = "".join(left)
        return left + middle + left[::-1]
        