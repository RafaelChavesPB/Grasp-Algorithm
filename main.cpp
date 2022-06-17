#include "bits/stdc++.h"
#define ELITE 3
#define A 450

using namespace std;

vector<vector<pair<int, int>>> lista_adj;
vector<pair<int, int>> resultados;
vector<int> visitados, caminho, dist, ident;
int lim_custo_mov;
random_device rd;

int funcao_objetiva(int custo_ida, int custo_volta, int q_vertices) {
	return A*q_vertices - (custo_ida + custo_volta);
}

void detalhes_resultado(){
    int cont = 0;
    cout << "Detalhes dos Resultados" << endl;
	cout << "Vertices - Custo ida - Custo Volta" << endl;
	for (auto it : resultados)
		cout << 2 * cont++ << ' ' << it.first << ' ' << it.second << endl;
	cout << endl;
    cout << "Caminho" << endl;
	for (auto it : caminho)
		cout << it << ' ';
	cout << endl << endl;;
}

void melhor_resultado(){
    int idx = 0;
	int melhor_resultado = 0;
    cout << "Melhor Resultado" << endl;
    for(int i = 0; i < resultados.size(); i++)
	    if(funcao_objetiva(resultados[i].first, resultados[i].second, 2*i) >
	    melhor_resultado)
	        idx = i;

	cout << resultados[idx].first << ' ' << resultados[idx].second << ' ' <<
	2*idx << ' ' << melhor_resultado << endl;

	for(int i = 0; i < 2*idx; i++)
	    cout << caminho[i] << ' ';
	cout << endl << endl;
}

int calc_custo_mov(int atual, vector<int> &caminho_atual){
    if(caminho_atual.size() < 2)
        return min(dist[atual] + ident[atual], lim_custo_mov);
    
    int ant = caminho_atual[caminho_atual.size() - 2];
    return min(abs(dist[atual] - dist[ant]) + abs(ident[atual] - ident[ant]), lim_custo_mov);
}

void dfs(int v, int custo_ida, int custo_volta, bool ida) {
    vector<pair<int, int>> melhores;
    
	visitados[v] = 1;
	caminho.push_back(v);

	for (auto it : lista_adj[v]) {
		if (visitados[it.second])
			continue;
        it.first += calc_custo_mov(it.second, caminho);
        melhores.push_back(it);
	}    

	if (!melhores.size())
		return;

    sort(melhores.begin(), melhores.end());
    
	int escolhido = rd() % (melhores.size() < ELITE ? melhores.size() : ELITE);
    
	custo_ida += melhores[escolhido].first * ida;
	custo_volta += melhores[escolhido].first * (!ida);
	v = melhores[escolhido].second;

	if (ida)
		resultados.push_back({custo_ida, custo_volta});

	dfs(v, custo_ida, custo_volta, !ida);
}

void grasp(int n) {
	visitados.assign(n, 0);
	resultados = vector<pair<int, int>>(1, {0, 0});
	caminho.clear();

	dfs(0, 0, 0, true);
}

int main() {
	int n, m, a, b, temp;
	cin >> a >> b;
    n = a + b;
    m = a*b*2;
    
	lista_adj.assign(n, vector<pair<int, int>>());
    
    cin >> lim_custo_mov;
    for(int i = 0; i < n; i++){
        dist.push_back(0);
        cin >> dist.back();
    }
    
    for(int i = 0; i < n; i++){
        ident.push_back(0);
        cin >> ident.back();
    }

	for (int i = 0; i < m; i++) {
		int a, b, c;
		cin >> a >> b >> c;
		lista_adj[a].push_back({c, b});
	}

	grasp(n);
    melhor_resultado();
}