/*
    Updates the whole rangle [L,R] to a particular value
    Change the push function to alter the kind of update
*/

template <class T>
struct LazySegmentTree
{
    #define lc (n << 1)
    #define rc ((n << 1) | 1)
    T U = 0;
    int N;
    vector<T>t,lazy;
    LazySegmentTree(int _n) : N(_n), t(_n << 2, 0), lazy(_n << 2, 0) {}
    inline void push(int n, int b, int e)
    {
        if (lazy[n] == 0)
            return;
        t[n] = lazy[n] * (e - b + 1);
        if (b != e)
        {
            lazy[lc] = lazy[n];
            lazy[rc] = lazy[n];
        }
        lazy[n] = 0;
    }
    inline T combine(long long a, long long b)
    {
        return a + b;
    }
    inline void pull(int n)
    {
        t[n] = t[lc] + t[rc];
    }
    void build(vi &a, int n, int b, int e)
    {
        lazy[n] = U;
        if (b == e)
        {
            t[n] = a[b];
            return;
        }
        int mid = (b + e) >> 1;
        build(a, lc, b, mid);
        build(a, rc, mid + 1, e);
        pull(n);
    }
    void upd(int n, int b, int e, int i, int j, long long v)
    {
        push(n, b, e);
        if (j < b || e < i)
            return;
        if (i <= b && e <= j)
        {
            lazy[n] = v; // set lazy
            push(n, b, e);
            return;
        }
        int mid = (b + e) >> 1;
        upd(lc, b, mid, i, j, v);
        upd(rc, mid + 1, e, i, j, v);
        pull(n);
    }
    T query(int n, int b, int e, int i, int j)
    {
        push(n, b, e);
        if (i > e || b > j)
            return 0; // return null
        if (i <= b && e <= j)
            return t[n];
        int mid = (b + e) >> 1;
        return combine(query(lc, b, mid, i, j), query(rc, mid + 1, e, i, j));
    }
};
