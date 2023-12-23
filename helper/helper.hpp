#pragma once
#include <bits/stdc++.h>

using namespace std;

namespace _Basic {
	mt19937 __rnd(chrono::system_clock::now().time_since_epoch().count());
	const double alpha = 0.7;
}

#define _Randi(l, r) uniform_int_distribution<>(l, r)(_Basic::__rnd)
#define _Randd(l, r) uniform_real_distribution<>(l, r)(_Basic::__rnd)

/* -------------------------------------------------- */
/* This part is about array. */
/* Generate an array with integers between L and R. */
template<class T>
vector<T> genArray(int n, T L, T R) {
	vector<T> a(n);
	for (int i = 0; i < n; ++i)
		a[i] = _Randi(L, R);
	return a;
}

/* Generate a [double] array with real numbers between L and R. */
vector<double> genArray(int n, double L, double R) {
	vector<double> a(n);
	for (int i = 0; i < n; ++i)
		a[i] = _Randd(L, R);
	return a;
}

/* Generate an array with distinct integers between L and R. */
template<class T>
vector<T> genArray_d(int n, T L, T R) {
	assert(R - L + 1 >= n);
	vector<T> a(n);
	if (R - L + 1 >= n / _Basic::alpha) {
		set<int> s;
		for (int i = 0; i < n; ++i) {
			int v = _Randi(L, R);
			while (s.count(v)) v = _Randi(L, R);
			s.insert(v);
			a[i] = v;
		}
	} else {
		vector<int> b(R - L + 1);
		iota(b.begin(), b.end(), L);
		shuffle(b.begin(), b.end(), _Basic::__rnd);
		for (int i = 0; i < n; ++i)
			a[i] = b[i];
	}
	return a;
}

/* Generate n pairs with integers between L and R. */
template<class T>
vector<pair<T, T>> genPairs(int n, T L, T R) {
	vector<pair<T, T>> a(n);
	for (int i = 0; i < n; ++i) {
		T fir = _Randi(L, R), sec = _Randi(L, R);
		a[i] = make_pair(fir, sec);
	}
	return a;
}

/* Generate n distinct pairs with integers between L and R. */
template<class T>
vector<pair<T, T>> genPairs_d(int n, T L, T R) {
	vector<pair<T, T>> a(n);
	__int128_t tot = (__int128_t)(R - L + 1) * (R - L + 1);
	assert(tot >= n);
	if (tot >= n / _Basic::alpha) {
		set<pair<T, T>> s;
		for (int i = 0; i < n; ++i) {
			T fir = _Randi(L, R), sec = _Randi(L, R);
			while (s.count(make_pair(fir, sec)))
				fir = _Randi(L, R), sec = _Randi(L, R);
			s.emplace(fir, sec);
			a[i] = make_pair(fir, sec);
		}
	} else {
		vector<pair<T, T>> b;
		for (T i = L; i <= R; ++i)
			for (T j = L; j <= R; ++j)
				b.emplace_back(i, j);
		shuffle(b.begin(), b.end(), _Basic::__rnd);
		for (int i = 0; i < n; ++i)
			a[i] = b[i];
	}
	return a;
}

/* Generate n pairs with real numbers between L and R. */
vector<pair<double, double>> genPairs(int n, double L, double R) {
	vector<pair<double, double>> a(n);
	for (int i = 0; i < n; ++i)
		a[i] = make_pair(_Randd(L, R), _Randd(L, R));
	return a;
}

/* Generate a permutation. */
vector<int> genPerm(int n) {
	vector<int> a(n);
	iota(a.begin(), a.end(), 1);
	shuffle(a.begin(), a.end(), _Basic::__rnd);
	return a;
}

/* -------------------------------------------------- */
/* This part is about tree. */
/* Generate a tree. */
template<class T = int>
class Tree {
private:
	int n, rt;
	vector<T> w;
	vector<int> id, fa;
	vector<tuple<int, int, int>> edge;

	/* Get idth edge's weight. */
	T getWeight(int id) {
		if (id >= (int)w.size()) return 0;
		return w[id];
	}

public:
	/* Build a tree. */
	void buildTree(int n, int maxDeg = 0, bool opt = false, int root = 0) {
		rt = root;
		id.clear(), fa.clear(), edge.clear();
		id.resize(n), fa.resize(n);
		if (maxDeg == 0) maxDeg = n;
		if (root) opt = 1;
		assert(root >= 0 && root < n);
		id[0] = root, fa[0] = -1;
		for (int i = 1; i < n; ++i)
			id[i] = i - (i - 1 < root); 
		if (opt)
			shuffle(next(id.begin()), id.end(), _Basic::__rnd);
		vector<int> deg(n, 0);
		for (int i = 1; i < n; ++i) {
			fa[i] = _Randi(0, i - 1);
			while (deg[fa[i]] >= maxDeg)
				fa[i] = _Randi(0, i - 1);
			edge.emplace_back(fa[i], i, i - 1);
			++deg[fa[i]];
		}
	}

	/* Generate a tree with n nodes. */
	Tree(int n = 0, int maxDeg = 0, bool opt = false, int root = 0) 
		: n(n), rt(root) {
		buildTree(n, maxDeg, opt, root);
	}
	
	/* Init edge weights. */
	void initEdge(vector<T> W = vector<T>()) {
		w = W;
		shuffle(w.begin(), w.end(), _Basic::__rnd);
	}

	/* Get tree's edge set. */
	vector<tuple<int, int, T>> getEdge() {
		vector<tuple<int, int, T>> a;
		for (int i = 0; i < (int)edge.size(); ++i) {
			int x, y, j;
			tie(x, y, j) = edge[i];
			a.emplace_back(id[x], id[y], getWeight(j));
		}
		shuffle(a.begin(), a.end(), _Basic::__rnd);
		return a;
	}

	/* Get tree's father nodes set */
	vector<int> getFather() {
		vector<int> a(n);
		if (n) a[rt] = -1;
		for (int i = 1; i < n; ++i)
			a[id[i]] = id[fa[i]];
		return a;
	}
};

/* -------------------------------------------------- */
/* This part is about graph. */
template<class T = int>
class Graph {
private:
	int n, m;
	vector<T> w;
	vector<tuple<int, int, int>> edge;
	bool rptEdge, selfCirc, isConnected, isDirected;

	/* Get idth edge's weight. */
	T getWeight(int id) {
		if (id >= (int)w.size()) return 0;
		return w[id];
	}

	/* Generate a new pair of (x, y) */
	void genNew(int &x, int &y) {
		x = _Randi(0, n - 1), y = _Randi(0, n - 1);
	}
	
public:
	/* Generate a graph. */
	Graph() {
		n = m = 0;
		w.clear(), edge.clear();
		rptEdge = false, selfCirc = false;
	}

	/* Build a graph with n nodes and m edges. */
	void buildNormalGraph(int N, int M, bool RptEdge, bool SelfCircle, bool IsConnected, bool IsDirected) {
		n = N, m = M;
		rptEdge = RptEdge, selfCirc = SelfCircle;
		isConnected = IsConnected, isDirected = IsDirected;
		edge.clear();
		if (isConnected)
			assert(m >= n - 1);
		if (!rptEdge)
			assert(n * (n - 1) / (2 - isDirected) + n * selfCirc >= m);
		if (isConnected) {
			Tree<> base(n, true, 0);
			edge = base.getEdge();
		}
		if (!rptEdge) {
			set<pair<int, int>> edgeSet;
			for (int i = 0; i < (int)edge.size(); ++i) {
				int x, y, w;
				tie(x, y, w) = edge[i];
				if (!isDirected && x > y) swap(x, y);
				edgeSet.emplace(x, y);
			}
			if (n * (n - 1) / (2 - isDirected) + n * selfCirc >= m / _Basic::alpha) {
				for (int i = edge.size() + 1; i <= m; ++i) {
					int x, y;
					genNew(x, y);
					if (!isDirected) {
						while ((!selfCirc && x == y)
								|| (!rptEdge && edgeSet.count({min(x, y), max(x, y)})))
							genNew(x, y);
					} else {
						while ((!selfCirc && x == y)
								|| (!rptEdge && edgeSet.count({x, y})))
							genNew(x, y);
					}
					edge.emplace_back(x, y, 0);
					if (!isDirected)
						edgeSet.emplace(min(x, y), max(x, y));
					else
						edgeSet.emplace(x, y);
				}
			} else {
				vector<pair<int, int>> tmpEdge;
				if (!isDirected) {
					for (int i = 0; i < n; ++i)
						for (int j = i + !selfCirc; j < n; ++j) {
							if (!edgeSet.count({i, j}))
								tmpEdge.emplace_back(i, j);
						}
				} else {
					for (int i = 0; i < n; ++i)
						for (int j = 0; j < n; ++j) {
							if (!selfCirc && i == j) continue;
							if (!edgeSet.count({i, j}))
								tmpEdge.emplace_back(i, j);
						}
				}
				shuffle(tmpEdge.begin(), tmpEdge.end(), _Basic::__rnd);
				for (int i = edge.size() + 1, j = 0; i <= m; ++i, ++j) {
					edge.emplace_back(tmpEdge[i].first, tmpEdge[i].second, 0);
				}
			}
		} else {
			for (int i = edge.size() + 1; i <= m; ++i) {
				int x, y;
				genNew(x, y);
				while ((!selfCirc && x == y)) genNew(x, y);
				edge.emplace_back(x, y, 0);
			}
		}
		for (int i = 0; i < m; ++i) {
			int x, y, z;
			tie(x, y, z) = edge[i];
			z = i;
			edge[i] = make_tuple(x, y, z);
		}
	}
	
	/* Build a DAG with n nodes and m edges. */
	void buildDAG(int N, int M, bool RptEdge) {
		n = N, m = M, rptEdge = RptEdge;
		edge.clear();
		vector<int> deg(n, 0);
		for (int i = 0; i < M; ++i) {
			int x = _Randi(1, n - 1);
			if (!rptEdge) {
				while (deg[x] >= x)
					x = _Randi(1, n - 1);
			}
			++deg[x];
		}
		int cntEdge = 0;
		for (int i = 1; i < N; ++i) {
			vector<int> pre;
			if (!rptEdge)
				pre = genArray_d(deg[i], 0, i - 1);
			else 
				pre = genArray(deg[i], 0, i - 1);
			for (int j = 0; j < deg[i]; ++j)
				edge.push_back({pre[j], i, cntEdge++});
		}
	}

	/* Init edge's weights set. */
	void initEdge(vector<T> W = vector<T>()) {
		w = W;
		shuffle(w.begin(), w.end(), _Basic::__rnd);
	}

	/* Get Graph's edge sets. */
	vector<tuple<int, int, T>> getEdge() {
		vector<tuple<int, int, T>> ed(m);
		for (int i = 0; i < m; ++i) {
			int x, y, j;
			tie(x, y, j) = edge[i];
			ed[i] = make_tuple(x, y, getWeight(j));
		}
		shuffle(ed.begin(), ed.end(), _Basic::__rnd);
		return ed;
	}
};

/* -------------------------------------------------- */
/* This part is about checking. */
/* Check if two files are the same. */
int diffFile(const string file1, const string file2) {
	vector<string> s1, s2;
	string buf;
	ifstream File1(file1), File2(file2);
	if (!File1.is_open() || !File2.is_open()) {
		cerr << "Can't find such file!\n";
		return -1;
	}
	while (File1 >> buf)
		s1.emplace_back(buf);
	while (File2 >> buf)
		s2.emplace_back(buf);
	File1.close(), File2.close();
	if (s1.size() != s2.size())
		return true;
	for (int i = 0; i < (int)s1.size(); ++i) {
		if (s1[i] != s2[i]) return true;
	}
	return false;
}

/* Check your program by data and bf. */
template<class Data, class Bf>
int checkAnswer_ll(string file, int T, Data data, Bf bf, string spj = "") {
	for (int i = 1; i <= T; ++i) {
		cerr << "Running on testcase " << i << ".\n";
		fclose(stdout);
		freopen("input.txt", "w", stdout);
		data();
		fclose(stdout);
		freopen("input.txt", "r", stdin);
		freopen("answer.txt", "w", stdout);
		bf();
		fclose(stdin), fclose(stdout);
		system((file + " < input.txt > output.txt").c_str());
		if (spj.empty()) {
			if (diffFile("answer.txt", "output.txt")) {
				cerr << "Wrong answer on testcase " << i << "!\n";
				return 1;
			}
		} else {
			if (system((spj + " input.txt output.txt answer.txt").c_str()))
				return 1;
		}
	}
	cerr << "Accepted!\n";
	return 0;
}

/* Check your program by data and bf. */
template<class Data>
int checkAnswer_ls(string file, int T, Data data, string bf, string spj = "") {
	for (int i = 1; i <= T; ++i) {
		cerr << "Running on testcase " << i << ".\n";
		fclose(stdout);
		freopen("input.txt", "w", stdout);
		data();
		fclose(stdout);
		system((bf + " < input.txt > answer.txt").c_str());
		system((file + " < input.txt > output.txt").c_str());
		if (spj.empty()) {
			if (diffFile("answer.txt", "output.txt")) {
				cerr << "Wrong answer on testcase " << i << "!\n";
				return 1;
			}
		} else {
			if (system((spj + " input.txt output.txt answer.txt").c_str()))
				return 1;
		}
	}
	cerr << "Accepted!\n";
	return 0;
}

/* Check your program by data and bf. */
template<class Bf>
int checkAnswer_sl(string file, int T, string data, Bf bf, string spj = "") {
	for (int i = 1; i <= T; ++i) {
		cerr << "Running on testcase " << i << ".\n";
		system((data + " > input.txt").c_str());
		fclose(stdin), fclose(stdout);
		freopen("input.txt", "r", stdin);
		freopen("answer.txt", "w", stdout);
		bf();
		fclose(stdin), fclose(stdout);
		system((file + " < input.txt > output.txt").c_str());
		if (spj.empty()) {
			if (diffFile("answer.txt", "output.txt")) {
				cerr << "Wrong answer on testcase " << i << "!\n";
				return 1;
			}
		} else {
			if (system((spj + " input.txt output.txt answer.txt").c_str()))
				return 1;
		}
	}
	cerr << "Accepted!\n";
	return 0;
}

/* Check your program by data and bf. */
int checkAnswer_ss(string file, int T, string data, string bf, string spj = "") {
	for (int i = 1; i <= T; ++i) {
		cerr << "Running on testcase " << i << ".\n";
		system((data + " > input.txt").c_str());
		system((bf + " < input.txt > answer.txt").c_str());
		system((file + " < input.txt > output.txt").c_str());
		if (spj.empty()) {
			if (diffFile("answer.txt", "output.txt")) {
				cerr << "Wrong answer on testcase " << i << "!\n";
				return 1;
			}
		} else {
			if (system((spj + " input.txt output.txt answer.txt").c_str()))
				return 1;
		}
	}
	cerr << "Accepted!\n";
	return 0;
}
