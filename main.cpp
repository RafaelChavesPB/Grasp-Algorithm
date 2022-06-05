#include "bits/stdc++.h"
#define A_ -1
#define B_ -1
#define C_ 1

using namespace std;

vector<vector<pair<int, int>>> lista_adj;
vector<pair<int, int>> resultados;
vector<int> visitados, caminho;
random_device rd;

int funcao_objetiva(int custo_ida, int custo_volta, int q_vertices) {
	return A_ * custo_ida + B_ * custo_volta + C_ * q_vertices;
}

void dfs(int v, int custo_ida, int custo_volta, bool ida) {
	vector<pair<int, int>> melhores;
	visitados[v] = 1;
	caminho.push_back(v);

	for (auto it : lista_adj[v]) {
		if (visitados[it.second])
			continue;

		melhores.push_back(it);
	}

	if (!melhores.size())
		return;

	int escolhido = rd() % (melhores.size() < 3 ? melhores.size() : 3);
	sort(melhores.begin(), melhores.end());
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
	int idx = 0;
	int melhor_resultado = 0;
	int cont = 0;
    
	cout << "Vertices - Custo ida - Custo Volta" << endl;
	for (auto it : resultados)
		cout << 2 * cont++ << ' ' << it.first << ' ' << it.second << endl;
	cout << endl;
	
    cout << "Caminho" << endl;
	for (auto it : caminho)
		cout << it << ' ';
	cout << endl;
    
	// for(int i = 0; i < resultados.size(); i++)
	//     if(funcao_objetiva(resultados[i].first, resultados[i].second, 2*i) >
	//     melhor_resultado)
	//         idx = i;

	// cout << resultados[idx].first << ' ' << resultados[idx].second << ' ' <<
	// 2*idx << endl;

	// for(int i = 0; i < 2*idx; i++)
	//     cout << caminho[i] << ' ';
	// cout << endl;
}

int main() {
	int n, m;
	cin >> n >> m;

	lista_adj.assign(n, vector<pair<int, int>>());

	for (int i = 0; i < m; i++) {
		int a, b, c;
		cin >> a >> b >> c;
		lista_adj[a].push_back({c, b});
	}

	// for (int i = 0; i < n; i++){
	//     cout << i << ':';
	//     for(auto v: lista_adj[i]){
	//         cout << ' ' << v.first <<':'<< v.second;
	//     }
	//     cout << endl;
	// }

	grasp(n);
}