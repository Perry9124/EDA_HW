#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct node {
	int index; // index
	int type; // 0 NOP, 1 MULT, 2 ALU
	int L; // ALAP
	int d; // ASAP
	vector<int> S; // successor
};
int V, L, T = 0;
int Max = 2147483647;
vector<node> G;
vector<bool> done;
vector<int> C;
bool comp1(node a, node b) {
	if (a.L > b.L)return true;
	else if (a.L == b.L) {
		return a.index > b.index;
	}
	else return false;
}
bool comp2(int a, int b) {
	return a < b;
}
void init(){
	for(int i = 0; i < V; i++){
		G[i].L = Max;
		G[i].d = 0;
	}
}
void dfs(int index, int d) {
	G[index].d = max(G[index].d, d);
	if(index == V - 1){
		G[index].L = L - T + 1;
		return;
	}
	vector<int> S = G[index].S;
	for (size_t i = 0; i < S.size(); i++) {
		if (G[S[i]].L == Max) {
			if(!done[S[i]])dfs(S[i], d + 1);
			else dfs(S[i], d);
		}
		G[index].L = min(G[index].L, G[S[i]].L - 1);
	}
}
vector<vector<int>> solve(vector<int> &a) {
	vector<bool> vis(V, 0);
	vector<vector<node>> u(2);
	vector<vector<int>> ans(L);
	vector<int> S = G[0].S;
	for (size_t i = 0; i < G[0].S.size(); i++) {
		u[G[S[i]].type - 1].push_back(G[S[i]]);
		vis[S[i]] = true;
		C[S[i]]--;
	} // initialize
	for (int t = 0; t < L; t++) {
		T = t;
		init();
		dfs(0, 0);
		// decreasing order
		for(int i = 0; i < 2; i++){
			for(size_t j = 0; j < u[i].size(); j++){
				u[i][j] = G[u[i][j].index];
			}
		}
		sort(u[0].begin(), u[0].end(), comp1);
		sort(u[1].begin(), u[1].end(), comp1);
		int count[2] = { 0, 0 };
		// initial : u, next : v
		// initial : C, next tmp
		vector<vector<node>> v = u;
		vector<int> tmp = C;
		for (int i = 0; i < 2; i++) {
			while (!u[i].empty()) {
				size_t j = u[i].size() - 1;
				count[i]++;
				S = u[i][j].S;
				// add vertex
				for (size_t k = 0; k < S.size(); k++) {
					tmp[S[k]]--;
				}
				// operation done
				ans[t].push_back(u[i][j].index);
				done[u[i][j].index] = true;
				u[i].pop_back();
				v[i].erase(v[i].begin() + j);
				// increasing resource
				if (count[i] == a[i]) {
					if (!u[i].empty() && u[i][j - 1].L - u[i][j - 1].d == 0 &&
						C[u[i][j - 1].index] == 0) {
						a[i]++;
					}
					else break;
				}
			}
		}
		for(int j = 1; j < V - 1; j++) {
			if(tmp[j] == 0 && C[j] != 0)v[G[j].type - 1].push_back(G[j]);
		}
		u = v;
		C = tmp;
		sort(ans[t].begin(), ans[t].end(), comp2);
	}
	return ans;
}
int main() {
	ifstream fin;
	string filename;
	cin >> filename;
	fin.open(filename + ".dfg");
	while (!fin.is_open()) {
		cout << filename << ".dfg not found\nfilename : ";
		cin >> filename;
		fin.open(filename + ".dfg");
	}
	fin >> V >> L;
	G.resize(V);
	done.resize(V, 0);
	C.resize(V, 0);
	for (int i = 0; i < V; i++) {
		fin >> G[i].index;
		size_t size;
		fin >> G[i].type >> size;
		G[i].S.resize(size);
		for (size_t j = 0; j < G[i].S.size(); j++) {
			fin >> G[i].S[j];
			C[G[i].S[j]]++;
		} //  successor
	}
	fin.close();
	init();
	dfs(0, 0);
	vector<int> a(2,1);
	vector<vector<int>> ans = solve(a);
	ofstream fout;
	fout.open(filename + ".txt");
	fout << a[0] << "\n" << a[1] << "\n";
	for(int i = 0; i < L; i++){
		for(size_t j = 0; j < ans[i].size(); j++) {
			fout << ans[i][j];
			if(j != ans[i].size() -1)fout << " ";
		}
		if(i != L - 1)fout << "\n";
	}
	return 0;
}