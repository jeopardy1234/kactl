/*
   query -> The usual O(log n) queries
   mquery -> Special type of queries (min/max) in O(1)
*/

template<class T>
struct SparseTable{
    vector<vector<T>>st;
    int n,k; T U = LONG_LONG_MIN;
    SparseTable(vector<T>&v){
        n = v.size(); k = __lg(n)+1;
        st.resize(n+1,vector<T>(k+1,U));
        for(int i=0; i<n; i++)st[i][0] = v[i];
        for(int j=1; j<=k; j++)for(int i=0; i+(1<<j) <= n; i++)st[i][j] = f(st[i][j-1] , st[i+(1<<(j-1))][j-1]);
    }

    T f(T a, T b){
        return max(a,b);
    }

    T query(int L, int R){
        int size = (R-L+1); int ans = U;
        for(int i=k; i>=0; i--){
            if(size & (1 << i)){
                ans = f(ans, st[L][i]);
                L += (1<<i);
            }
        }
        return ans;
    }

    T mquery(int L , int R){
        int p = __lg(R-L+1);
        return f(st[L][p], st[R-(1<<p)+1][p]);
    }
};
