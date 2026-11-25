
class Solution {
public:
    int smallestUniqueSubarray(vector<int>& nums) {
        int n = nums.size();

        // Coordinate compression
        vector<int> vals = nums;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());

        vector<int> sa(n), rank(n), tmp(n);

        for (int i = 0; i < n; i++) {
            sa[i] = i;
            rank[i] = lower_bound(vals.begin(), vals.end(),
                                  nums[i]) - vals.begin();
        }

        // Build suffix array
        for (int k = 1; k < n; k *= 2) {
            auto cmp = [&](int a, int b) {
                if (rank[a] != rank[b])
                    return rank[a] < rank[b];

                int x = (a + k < n) ? rank[a + k] : -1;
                int y = (b + k < n) ? rank[b + k] : -1;

                return x < y;
            };

            sort(sa.begin(), sa.end(), cmp);

            tmp[sa[0]] = 0;

            for (int i = 1; i < n; i++) {
                tmp[sa[i]] = tmp[sa[i - 1]] +
                    (cmp(sa[i - 1], sa[i]) ? 1 : 0);
            }

            rank = tmp;

            if (rank[sa[n - 1]] == n - 1)
                break;
        }

        // Kasai's algorithm: build LCP array
        vector<int> pos(n), lcp(n, 0);

        for (int i = 0; i < n; i++)
            pos[sa[i]] = i;

        int h = 0;

        for (int i = 0; i < n; i++) {
            int p = pos[i];

            if (p == 0) {
                h = 0;
                continue;
            }

            int j = sa[p - 1];

            while (i + h < n && j + h < n &&
                   nums[i + h] == nums[j + h]) {
                h++;
            }

            lcp[p] = h;

            if (h > 0) h--;
        }

        // Find shortest unique prefix among all suffixes
        int ans = n;

        for (int i = 0; i < n; i++) {
            int longest = lcp[i];

            if (i + 1 < n)
                longest = max(longest, lcp[i + 1]);

            int need = longest + 1;

            if (need <= n - sa[i])
                ans = min(ans, need);
        }

        return ans;
    }
};
