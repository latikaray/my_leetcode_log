class Solution:
    def longestRepeating(self, s: str, queryCharacters: str, queryIndices: List[int]) -> List[int]:
        n = len(s)
        tree = [None] * (4 * n)

        # (left_char, right_char, prefix, suffix, best, length)
        def merge(a, b):
            lc, lrc, lp, ls, lb, llen = a
            rc, rrc, rp, rs, rb, rlen = b

            prefix = lp
            suffix = rs
            best = max(lb, rb)

            if lrc == rc:
                best = max(best, ls + rp)

                if lp == llen:
                    prefix = llen + rp

                if rs == rlen:
                    suffix = rlen + ls

            return (
                lc,
                rrc,
                prefix,
                suffix,
                best,
                llen + rlen
            )

        def build(node, left, right):
            if left == right:
                ch = s[left]
                tree[node] = (ch, ch, 1, 1, 1, 1)
                return

            mid = (left + right) // 2

            build(node * 2, left, mid)
            build(node * 2 + 1, mid + 1, right)

            tree[node] = merge(tree[node * 2], tree[node * 2 + 1])

        def update(node, left, right, idx, ch):
            if left == right:
                tree[node] = (ch, ch, 1, 1, 1, 1)
                return

            mid = (left + right) // 2

            if idx <= mid:
                update(node * 2, left, mid, idx, ch)
            else:
                update(node * 2 + 1, mid + 1, right, idx, ch)

            tree[node] = merge(tree[node * 2], tree[node * 2 + 1])

        build(1, 0, n - 1)

        ans = []

        for ch, idx in zip(queryCharacters, queryIndices):
            update(1, 0, n - 1, idx, ch)
            ans.append(tree[1][4])

        return ans