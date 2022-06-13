template <class T>
struct SegmentTree
{
    T U = 0;
    size_t N, n;
    vector<T> tree;
    SegmentTree() {}
    SegmentTree(int an)
    {
        n = (size_t)an;
        for (N = 1UL; N < n; N <<= 1UL);
        tree = vector<T>(N << 1UL,0);
    }
    T fnc(T a, T b)
    {
        return (a + b);
    }
    void build(VI a)
    {
        for (size_t i = 0; i < n; ++i) tree[N + i] = a[i];
        for (size_t i = N - 1; i >= 1; --i) tree[i] = fnc(tree[i << 1] , tree[i << 1 | 1]);
    }
    void update(int k, T val)
    {
        tree[k += N] = val; k >>= 1;
        for (; k >= 1; k >>= 1) 
            tree[k] = fnc(tree[k << 1], tree[k << 1 | 1]);
    }
    T query(int L, int R, int l = 0, int r = -1, int v = 1)
    {
        if (r == -1) r = N - 1;
        if (R < l or L > r) return U;
        if (L <= l and R >= r) return tree[v];
        int mid = l + ((r - l) >> 1);
        return fnc(query(L, R, l, mid, v << 1) ,
                query(L, R, mid + 1, r, v << 1 | 1));
    }
};
