#include "bits/stdc++.h"
#define ALPHA 3
#define OMEGA 3
#define AMOSTRAS 100
#define TOP 3
#define A 450

using namespace std;

void debug(string message)
{
	cout << message << endl;
}

// Definição do grafo
vector<vector<int>> matriz_adj;
vector<vector<pair<int, int>>> lista_adj;

// Variáveis necessárias criação da solução
vector<int> visitados, dist, ident;
int lim_custo_mov;
random_device rd;

// Váriaveis que descrevem a solução atual
vector<pair<int, int>> resultados;
vector<int> caminho;

struct Solucao
{
	vector<int> caminho;
	vector<pair<int, int>> resultados_parciais;
	int melhor_resultado, idx, qtd_vertices;

	Solucao();
	Solucao(vector<int> &caminho_, vector<pair<int, int>> &resultados_);
	int funcao_objetiva(int custo_ida, int custo_volta, int q_vertices);
	void trocar_ida(int a, int b);
	void trocar_volta(int a, int b);
	void reposicionar_pares(int idx_a, int idx_b);
	void recalc_resultado();
	void obter_melhor_resultado();
	void print_melhor_resultado();
	void print_melhor_caminho();
	void print_resultado_completo();
};

void busca_local(Solucao &atual);
void obter_ordem(vector<pair<int,int>> &v);
int calc_custo_mov(int atual, vector<int> &caminho_atual);
int obter_linha_inicial(int linhas);
void dfs(int v, int custo_ida, int custo_volta, bool ida);
Solucao grasp(int n, int linha_inicial);

// O(log(N)*N²)
int main()
{
	int n, m, linhas_a, linhas_b, temp;
	cin >> linhas_a >> linhas_b;
	n = linhas_a + linhas_b;
	m = linhas_a * linhas_b * 2;

	lista_adj.assign(n, vector<pair<int, int>>());
	matriz_adj.assign(n, vector<int>(n, 0));

	cin >> lim_custo_mov;
	for (int i = 0; i < n; i++)
	{
		dist.push_back(0);
		cin >> dist.back();
	}

	for (int i = 0; i < n; i++)
	{
		ident.push_back(0);
		cin >> ident.back();
	}

	for (int i = 0; i < m; i++)
	{
		int a, b, c;
		cin >> a >> b >> c;
		lista_adj[a].push_back({c, b});
		matriz_adj[a][b] = c;
	}

	vector<Solucao> solucoes;
	for (int i = 0; i < AMOSTRAS; i++)
	{
		Solucao atual = grasp(n, obter_linha_inicial(linhas_a));
		solucoes.push_back(atual);
	}


	sort(solucoes.begin(), solucoes.end(), [](Solucao &x, Solucao &y)
		 { return x.melhor_resultado > y.melhor_resultado; });

	for (int i = 0; i < TOP; i++)
	{	
		solucoes[i].print_melhor_resultado();
		solucoes[i].print_melhor_caminho();
		cout << endl;
	}

	for(auto &it: solucoes)
		busca_local(it);

	sort(solucoes.begin(), solucoes.end(), [](Solucao &x, Solucao &y)
		 { return x.melhor_resultado > y.melhor_resultado; });

	for (int i = 0; i < TOP; i++)
	{	
		solucoes[i].print_melhor_resultado();
		solucoes[i].print_melhor_caminho();
		cout << endl;
	}
}

// O(1)
Solucao::Solucao() {}

// O(N)
Solucao::Solucao(vector<int> &caminho_, vector<pair<int, int>> &resultados_)
{
	this->caminho = caminho_;
	this->resultados_parciais = resultados_;
	obter_melhor_resultado();
}

// O(1)
void Solucao::trocar_ida(int idx_a, int idx_b)
{
	swap(this->caminho[idx_a * 2], this->caminho[idx_b * 2]);
}

// O(1)
void Solucao::trocar_volta(int idx_a, int idx_b)
{
	swap(this->caminho[idx_a * 2 + 1], this->caminho[idx_b * 2 + 1]);
}

// O(1)
void Solucao::reposicionar_pares(int idx_a, int idx_b)
{
	swap(this->caminho[idx_a * 2], this->caminho[idx_b * 2]);
	swap(this->caminho[idx_a * 2 + 1], this->caminho[idx_b * 2 + 1]);
}

// O(1)
int Solucao::funcao_objetiva(int custo_ida, int custo_volta, int q_vertices)
{
	return A * q_vertices - (custo_ida + custo_volta);
}

// O(n)
void Solucao::recalc_resultado()
{
	pair<int, int> custo = {0, 0};
	for (int i = 0; i < this->caminho.size() - 1; i++)
	{
		if (i % 2 == 0)
		{
			custo.first += calc_custo_mov(this->caminho[i], this->caminho) +
						   matriz_adj[this->caminho[i]][this->caminho[i + 1]];
			this->resultados_parciais[i / 2 + 1] = custo;
		}
		else
		{
			custo.second += calc_custo_mov(this->caminho[i], this->caminho);
		}
	}
	
	// for(auto it: resultados_parciais){
	// 	cout << "(" << it.first << ',' << it.second << ")" << ' ';
	// }
	// cout << endl;
	
	obter_melhor_resultado();
}

// O(N)
void Solucao::obter_melhor_resultado()
{
	this->qtd_vertices = 0;
	this->melhor_resultado = 0;
	for (int i = 0; i < this->resultados_parciais.size(); i++)
	{
		int resultado_atual = funcao_objetiva(
			this->resultados_parciais[i].first,
			this->resultados_parciais[i].second,
			2 * i);
		if (resultado_atual > this->melhor_resultado)
		{
			this->idx = i;
			this->qtd_vertices = 2 * i;
			this->melhor_resultado = resultado_atual;
		}
	}
}

// O(1)
void Solucao::print_melhor_resultado()
{
	cout << "Vertices - Custo ida - Custo Volta - Função Objetiva" << endl;
	cout << this->resultados_parciais[this->qtd_vertices / 2].first << ' '
		 << this->resultados_parciais[this->qtd_vertices / 2].second << ' '
		 << this->qtd_vertices << ' ' << this->melhor_resultado << endl;
}

// O(N)
void Solucao::print_melhor_caminho()
{
	cout << "Caminho" << endl;
	for (int i = 0; i < this->qtd_vertices; i++)
		cout << this->caminho[i] << ' ';
	cout << endl;
}

// O(N)
void Solucao::print_resultado_completo()
{
	int cont = 0;
	cout << "Detalhes dos Resultados" << endl
		 << "Vertices - Custo ida - Custo Volta" << endl;
	for (auto it : this->resultados_parciais)
		cout << 2 * cont++ << ' ' << it.first << ' ' << it.second << endl;
	cout << endl
		 << "Caminho" << endl;
	for (auto it : caminho)
		cout << it << ' ';
	cout << endl
		 << endl;
}

void busca_local(Solucao &atual)
{
	Solucao nova = atual;
	vector<pair<int,int>> ordem(nova.resultados_parciais.size() - 1);
	int melhor_resultado = nova.melhor_resultado;

	obter_ordem(ordem);

	for (int i = 0; i <  ordem.size(); i++)
	{
		int a = ordem[i].second; 
		for (int j = i + 1; j <  ordem.size(); j++)
		{
			int b = ordem[j].second;
			nova.trocar_ida(a, b);
			nova.recalc_resultado();

			// if(nova.melhor_resultado > atual.melhor_resultado){
			// 	cout << nova.melhor_resultado << " > " <<  atual.melhor_resultado << endl;
			// } else if(nova.melhor_resultado == atual.melhor_resultado){
			// 	cout << nova.melhor_resultado << " == " <<  atual.melhor_resultado << endl;
			// } else{
			// 	cout << nova.melhor_resultado << " < " <<  atual.melhor_resultado << endl;
			// }

			if(nova.melhor_resultado > atual.melhor_resultado){
				atual = nova;
			} else {
				nova.trocar_ida(a, b);
			}
		}
	}

	obter_ordem(ordem);

	for (int i = 0; i < ordem.size(); i++)
	{
		int a = ordem[i].second; 
		for (int j = i + 1; j <  ordem.size(); j++)
		{
			int b = ordem[j].second;
			nova.trocar_volta(a, b);
			nova.recalc_resultado();

			// if(nova.melhor_resultado > atual.melhor_resultado){
			// 	cout << nova.melhor_resultado << " > " <<  atual.melhor_resultado << endl;
			// } else if(nova.melhor_resultado == atual.melhor_resultado){
			// 	cout << nova.melhor_resultado << " == " <<  atual.melhor_resultado << endl;
			// } else{
			// 	cout << nova.melhor_resultado << " < " <<  atual.melhor_resultado << endl;
			// }

			if(nova.melhor_resultado > atual.melhor_resultado){
				atual = nova;
			} else {
				nova.trocar_volta(b, a);
			}
		
		}
	}

	obter_ordem(ordem);

	for (int i = 0; i <  ordem.size(); i++)
	{
		int a = ordem[i].second; 
		for (int j = i + 1; j <  ordem.size(); j++)
		{
			int b = ordem[j].second;
			nova.reposicionar_pares(a,b);
			nova.recalc_resultado();
			
			// if(nova.melhor_resultado > atual.melhor_resultado){
			// 	cout << nova.melhor_resultado << " > " <<  atual.melhor_resultado << endl;
			// } else if(nova.melhor_resultado == atual.melhor_resultado){
			// 	cout << nova.melhor_resultado << " == " <<  atual.melhor_resultado << endl;
			// } else{
			// 	cout << nova.melhor_resultado << " < " <<  atual.melhor_resultado << endl;
			// }

			
			if(nova.melhor_resultado > atual.melhor_resultado){
				atual = nova;
			} else {
				nova.reposicionar_pares(b,a);
			}		
		}
	}
}

// O(1)
int calc_custo_mov(int atual, vector<int> &caminho_atual)
{
	if (caminho_atual.size() < 2)
		return min(dist[atual] + ident[atual], lim_custo_mov);

	int ant = caminho_atual[caminho_atual.size() - 2];

	return min(
		abs(dist[atual] - dist[ant]) + 50 * ((dist[atual] - dist[ant]) == 0) + abs(ident[atual] - ident[ant]),
		lim_custo_mov);
}

// O(log(N)*N)
int obter_linha_inicial(int linhas)
{
	vector<pair<int, int>> opcoes;
	for (int i = 0; i < linhas; i++)
	{
		int custo = min(dist[i] + ident[i], 300);
		opcoes.push_back({custo + rd() % 300, i});
	}
	sort(opcoes.begin(), opcoes.end());
	return opcoes.front().second;
}

// O(log(N)*N)
void obter_ordem(vector<pair<int,int>> &v){
	for(int i = 0; i < v.size(); i++)
		v[i] = {i + rd() % int (OMEGA*v.size()), i};
	sort(v.begin(),v.end());
}

// O(log(N)*N²)
void dfs(int v, int custo_ida, int custo_volta, bool ida)
{
	vector<pair<int, int>> melhores;
	visitados[v] = 1;
	caminho.push_back(v);

	for (auto it : lista_adj[v])
	{
		if (visitados[it.second])
			continue;
		it.first += calc_custo_mov(it.second, caminho);
		melhores.push_back(it);
	}

	if (!melhores.size())
		return;

	sort(melhores.begin(), melhores.end());

	int escolhido = rd() % (melhores.size() < ALPHA ? melhores.size() : ALPHA);

	custo_ida += melhores[escolhido].first * ida;
	custo_volta += melhores[escolhido].first * (!ida);
	v = melhores[escolhido].second;

	if (ida)
		resultados.push_back({custo_ida, custo_volta});

	dfs(v, custo_ida, custo_volta, !ida);
}

// O(log(N)*N²)
Solucao grasp(int n, int linha_inicial)
{
	visitados.assign(n, 0);
	resultados = vector<pair<int, int>>(1, {0, 0});
	caminho.clear();
	dfs(linha_inicial, 0, 0, true);
	return Solucao(caminho, resultados);
}
