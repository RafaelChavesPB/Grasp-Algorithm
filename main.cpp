#include "bits/stdc++.h"

using namespace std;

vector<int> visitado; 
vector<vector<pair<int,int>>> lista_adj;
vector<int> resultados;

void dfs(int v, int custo, int q_vis){
    visitado[v] = 1;
    vector<pair<int, int>> melhores;
    for(auto it: lista_adj[v]){
        if(visitado[it.second])
            continue;
    }
}

int main(){
    int n, m;
    cin >> n >> m;
    
    lista_adj.assign(n, vector<pair<int,int>>());
    visitado.assign(n, 1);

    for(int i = 0; i < m; i++){
        int a, b, c;
        cin >> a >> b >> c;
        lista_adj[a].push_back({c,b});
    }




}