#include "bits/stdc++.h"

using namespace std;
random_device rd;
vector<int> visitado; 
vector<vector<pair<int,int>>> lista_adj;
vector<pair<int,int>> resultados;

void dfs(int v, int custo_ida, int custo_volta, bool ida){
    visitado[v] = 1;
    vector<pair<int, int>> melhores;
    
    for(auto it: lista_adj[v]){

        if(visitado[it.second])
            continue;
        
        melhores.push_back(it);
    }
    
    if(!melhores.size())
        return;

    int escolhido = rd() % (melhores.size() < 3 ? melhores.size():3);
    sort(melhores.begin(), melhores.end(), greater<pair<int,int>>());
    custo_ida += melhores[escolhido].first*ida;
    custo_volta += melhores[escolhido].first*(!ida);
    v = melhores[escolhido].second;

    if(ida)
        resultados.push_back({custo_ida, custo_volta});

    dfs(v, custo_ida, custo_volta, !ida);  
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