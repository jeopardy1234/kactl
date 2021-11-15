/**
 * Author: chilli
 * Date: 2019-04-26
 * License: CC0
 * Source: https://cp-algorithms.com/graph/dinic.html
 * Description: Flow algorithm with complexity $O(VE\log U)$ where $U = \max |\text{cap}|$.
 * $O(\min(E^{1/2}, V^{2/3})E)$ if $U = 1$; $O(\sqrt{V}E)$ for bipartite matching.
 * Status: Tested on SPOJ FASTFLOW and SPOJ MATCHING, stress-tested
 */
#pragma once

// struct Dinic {
// 	struct Edge {
// 		int to, rev;
// 		ll c, oc;
// 		ll flow() { return max(oc - c, 0LL); } // if you need flows
// 	};
// 	vi lvl, ptr, q;
// 	vector<vector<Edge>> adj;
// 	Dinic(int n) : lvl(n), ptr(n), q(n), adj(n) {}
// 	void addEdge(int a, int b, ll c, ll rcap = 0) {
// 		adj[a].push_back({b, sz(adj[b]), c, c});
// 		adj[b].push_back({a, sz(adj[a]) - 1, rcap, rcap});
// 	}
// 	ll dfs(int v, int t, ll f) {
// 		if (v == t || !f) return f;
// 		for (int& i = ptr[v]; i < sz(adj[v]); i++) {
// 			Edge& e = adj[v][i];
// 			if (lvl[e.to] == lvl[v] + 1)
// 				if (ll p = dfs(e.to, t, min(f, e.c))) {
// 					e.c -= p, adj[e.to][e.rev].c += p;
// 					return p;
// 				}
// 		}
// 		return 0;
// 	}
// 	ll calc(int s, int t) {
// 		ll flow = 0; q[0] = s;
// 		rep(L,0,31) do { // 'int L=30' maybe faster for random data
// 			lvl = ptr = vi(sz(q));
// 			int qi = 0, qe = lvl[s] = 1;
// 			while (qi < qe && !lvl[t]) {
// 				int v = q[qi++];
// 				for (Edge e : adj[v])
// 					if (!lvl[e.to] && e.c >> (30 - L))
// 						q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
// 			}
// 			while (ll p = dfs(s, t, LLONG_MAX)) flow += p;
// 		} while (lvl[t]);
// 		return flow;
// 	}
// 	bool leftOfMinCut(int a) { return lvl[a] != 0; }
// };

template <typename T, bool directed> struct Dinic {
    struct edge {
        int to, rev;
        T cap;
        edge(int to, T cap, int rev) : to(to), cap(cap), rev(rev) {}
    };
    vector<vector<edge>> G;
    vector<pair<int, int>> pos;
    vector<int> level, iter;
    Dinic(int n) : G(n), level(n), iter(n) {}
    int add_edge(int from, int to, T cap) {
        pos.emplace_back(from, G[from].size());
        G[from].emplace_back(to, cap, G[to].size());
        G[to].emplace_back(from, directed ? 0 : cap, G[from].size() - 1);
        return pos.size() - 1;
    }
    int add_vertex() {
        G.emplace_back();
        level.emplace_back();
        iter.emplace_back();
        return G.size() - 1;
    }
    tuple<int, int, int, int> get_edge(int i) {
        auto e = G[pos[i].first][pos[i].second];
        auto re = G[e.to][e.rev];
        return {pos[i].first, e.to, e.cap + re.cap, re.cap};
    }
    vector<tuple<int, int, int, int>> edges() {
        vector<tuple<int, int, int, int>> res;
        for (int i = 0; i < (int)pos.size(); i++) {
            res.emplace_back(get_edge(i));
        }
        return res;
    }
    void change_edge(int i, T new_cap, T new_flow) {
        auto& e = G[pos[i].first][pos[i].second];
        auto& re = G[e.to][e.rev];
        e.cap = new_cap - new_flow;
        re.cap = (directed ? new_flow : new_cap + new_flow);
    }
    void bfs(int s) {
        fill(level.begin(), level.end(), -1);
        queue<int> que;
        level[s] = 0;
        que.emplace(s);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (auto& e : G[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.emplace(e.to);
                }
            }
        }
    }
    T dfs(int v, int t, T f) {
        if (v == t) return f;
        for (int& i = iter[v]; i < (int)G[v].size(); i++) {
            auto& e = G[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                T d = dfs(e.to, t, min(f, e.cap));
                if (d <= 0) continue;
                e.cap -= d;
                G[e.to][e.rev].cap += d;
                return d;
            }
        }
        return 0;
    }
    T max_flow(int s, int t, T lim) {
        T flow = 0;
        while (lim > 0) {
            bfs(s);
            if (level[t] < 0) break;
            fill(iter.begin(), iter.end(), 0);
            while (lim > 0) {
                T f = dfs(s, t, lim);
                if (f == 0) break;
                flow += f;
                lim -= f;
            }
        }
        return flow;
    }
    T max_flow(int s, int t) { return max_flow(s, t, numeric_limits<T>::max()); }
    vector<bool> min_cut(int s) {
        vector<bool> res(G.size());
        queue<int> que;
        res[s] = true;
        que.emplace(s);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (auto e : G[v]) {
                if (e.cap > 0 && !res[e.to]) {
                    res[e.to] = true;
                    que.emplace(e.to);
                }
            }
        }
        return res;
    }
};
