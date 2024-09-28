#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

struct node{
    int index;
    // interval [l, r]
    int l = 2147483647;
    int r = 0;
    bool visit = 0;
    set<int> parent;
    node(int index) {
        this->index = index;
    }
};
int V; // num of vertices
vector<int> upper_layer;
vector<int> lower_layer;
vector<node> G; // graph
set<int> num; // all index
int track = 0;

bool comp(node n1, node n2) {
    return n1.l < n2.l;
}
vector<vector<int>> solve() {
    vector<vector<int>> ans(1, vector<int> (V, 0)); 
    int watermark;
    int count = G.size();
    while(count) {
        watermark = -1;
        for(int i = 0; i < G.size(); i++) {
            if(G[i].parent.empty() && G[i].l > watermark && !G[i].visit) {
                for(int j = G[i].l; j <= G[i].r; j++) {
                    ans[track][j] = G[i].index;
                } // fill number
                for(size_t j = 0; j < G.size(); j++) {
                    auto pos = G[j].parent.find(G[i].index);
                    if(pos != G[j].parent.end()) {
                        G[j].parent.erase(pos);
                    }
                } // erase parent
                G[i].visit = 1;
                watermark = G[i].r;
                count--;
            }
        }
        ans.push_back(vector<int>(V, 0));
        track++;
    }
    return ans;
}
int main() {
    string filename;
    ifstream fin;
    cin >> filename;
    fin.open(filename + ".g");
    while(!fin.is_open()) {
        cout << filename << " can't be open\nfilename : ";
        cin >> filename;
        fin.open(filename + ".g");
    }
    fin >> V;
	upper_layer.resize(V);
	lower_layer.resize(V);
    for(int i = 0; i < V; i++) {
        fin >> upper_layer[i];
    }
    for(int i = 0; i < V; i++) {
        fin >> lower_layer[i];
    }
    fin.close();
    // initialize graph 
    for(int i = 0; i < V; i++) {
        int u = upper_layer[i];
        int l = lower_layer[i];
        if(u!=0 && num.find(u) == num.end()) {
            num.insert(u);
            G.push_back(node(u));
        }
        if(l!= 0 && num.find(l) == num.end()) {
            num.insert(l);
            G.push_back(node(l));
        }
        if(u == 0 || l == 0)continue;
        for (size_t j = 0; j < G.size(); j++) {
            if(G[j].index == l) {
                G[j].parent.insert(u);
            }
        }
    }
    // adding interval
    for(int i = 0; i < V; i++) {
        int u = upper_layer[i];
        int l = lower_layer[i];
        for(size_t j = 0; j < G.size(); j++) {
            if(G[j].index == u) {
                G[j].l = G[j].l < i ? G[j].l : i;
                G[j].r = G[j].r > i ? G[j].r : i;
            }
            if(G[j].index == l) {
                G[j].l = G[j].l < i ? G[j].l : i;
                G[j].r = G[j].r > i ? G[j].r : i;
            }
        }
    }
    sort(G.begin(), G.end(), comp);
    vector<vector<int>> ans = solve();
    ofstream fout;
    fout.open(filename + ".txt");
    fout << track << "\n";
    for(int i = 0; i < track; i++) {
        for(int j = 0; j < V; j++) {
            fout << ans[i][j];
            if(j < V - 1)fout << " ";
        }
        if(i < track - 1)fout << "\n";
    }
    return 0;
}