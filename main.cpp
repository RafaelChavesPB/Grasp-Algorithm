#include "bits/stdc++.h"
#define ALPHA 3
#define OMEGA 3
#define LIMITE_T 0.1
#define TOP 1
#define A 200.0
using namespace std;

void debug(string message)
{
	cout << message << endl;
}

// Definição do grafo
vector<vector<int>> matriz_adj;
vector<vector<pair<int, int>>> lista_adj;

// Variáveis necessárias criação da solução
vector<int> visitados;
int linhas_a, linhas_b;
random_device rd;

// Váriaveis que descrevem a solução atual
vector<pair<int, int>> resultados;
vector<int> caminho;

struct Solucao
{
	int melhor_resultado, idx, qtd_vertices, tempo;
	vector<int> caminho;
	vector<pair<int, int>> resultados_parciais;

	Solucao();
	Solucao(vector<int> &caminho_, vector<pair<int, int>> &resultados_);
	int funcao_objetiva(int custo_ida, int custo_volta, int q_vertices);
	void trocar_ida(int a, int b);
	void trocar_volta(int a, int b);
	void reposicionar_pares(int idx_a, int idx_b);
	void recalc_resultado();
	void obter_tempo(clock_t inicio);
	void obter_melhor_resultado();
	void print_resultado();
	void print_melhor_caminho();
	void print_resultado_detalhado();
};

bool bl_linhas_a(Solucao &atual);
bool bl_linhas_b(Solucao &atual);
void bl_nojump(Solucao &atual, int lado);
void obter_ordem(vector<pair<int, int>> &v);
int obter_linha_inicial(int linhas);
void dfs(int v, int custo_ida, int custo_volta, bool ida);
Solucao grasp(int n, int linha_inicial);

// O(P*N³)
int main(int argc, char *argv[])
{
	double inicio = clock();

	int n, m, temp;
	cin >> linhas_a >> linhas_b;
	n = linhas_a + linhas_b;
	m = linhas_a * linhas_b * 2;

	lista_adj.assign(n, vector<pair<int, int>>());
	matriz_adj.assign(n, vector<int>(n, 0));

	for (int i = 0; i < m; i++)
	{
		int a, b, c;
		cin >> a >> b >> c;
		lista_adj[a].push_back({c, b});
		matriz_adj[a][b] = c;
	}

	vector<Solucao> solucoes;
	while ((clock() - inicio) / (CLOCKS_PER_SEC * 1.0) < LIMITE_T)
	{
		int linha_inicial = obter_linha_inicial(linhas_a);
		Solucao a = grasp(n, 0);
		a.obter_tempo(inicio);
		auto b = a;

		bl_nojump(a, 0);
		while (bl_linhas_b(a))
		{
			a.obter_tempo(inicio);
		}
		bl_nojump(b, 1);
		while (bl_linhas_a(b))
		{
			b.obter_tempo(inicio);
		}

		if (a.melhor_resultado > b.melhor_resultado)
			solucoes.push_back(a);
		else
			solucoes.push_back(b);
	}

	sort(solucoes.begin(), solucoes.end(),
		 [](Solucao &x, Solucao &y)
		 {
			 if (x.melhor_resultado == y.melhor_resultado)
				 return x.tempo < y.tempo;
			 return x.melhor_resultado > y.melhor_resultado;
		 });

	cout << argv[1] << ": ";
	solucoes.front().print_resultado();

	// for (int i = 0; i < TOP; i++)
	// {
	// 	solucoes[i].print_resultado_detalhado();
	// 	solucoes[i].print_melhor_caminho();
	// }
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
			custo.first += matriz_adj[this->caminho[i]][this->caminho[i + 1]];
			this->resultados_parciais[i / 2 + 1] = custo;
		}
		else
		{
			custo.second += matriz_adj[this->caminho[i]][this->caminho[i + 1]];
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
void Solucao::print_resultado()
{
	cout << setprecision(5) << fixed << this->melhor_resultado / (A * this->qtd_vertices)
		 << "  |  " << setw(8) <<this->tempo << " ms" << endl;
}

// O(1)
void Solucao::print_resultado_detalhado()
{
	cout << "Vertices: " << this->qtd_vertices << endl
		 << "Função Objetiva: " << this->melhor_resultado << endl
		 << "Resultado: " << setprecision(5) << fixed << this->melhor_resultado / (A * this->qtd_vertices) << endl
		 << "Tempo: " << this->tempo << " ms" << endl;
}

// O(N)
void Solucao::print_melhor_caminho()
{
	cout << "Caminho:" << endl;
	cout << "Linhas A -";
	for (int i = 0; i < this->resultados_parciais.size() - 1; i++)
		cout << setw(3) << this->caminho[i * 2] << ' ';
	cout << endl;
	cout << "Linhas B -";
	for (int i = 0; i < this->resultados_parciais.size() - 1; i++)
		cout << setw(3) << this->caminho[i * 2 + 1] << ' ';
	cout << endl;
}

// O(1)
void Solucao::obter_tempo(clock_t inicio)
{
	this->tempo = 1000 * (clock() - inicio) / (CLOCKS_PER_SEC * 1.0);
}

// O(N³)
bool bl_linhas_a(Solucao &atual)
{
	Solucao nova = atual;
	vector<pair<int, int>> ordem(nova.resultados_parciais.size() - 1);
	int melhor_resultado = nova.melhor_resultado;
	bool flag = false;
	obter_ordem(ordem);

	for (int i = 0; i < ordem.size(); i++)
	{
		int a = ordem[i].second;
		for (int j = i + 1; j < ordem.size(); j++)
		{
			int b = ordem[j].second;
			nova.trocar_ida(a, b);
			nova.recalc_resultado();

			if (nova.melhor_resultado > atual.melhor_resultado)
			{
				flag = true;
				atual = nova;
			}
			else
			{
				nova.trocar_ida(a, b);
			}
		}
	}
	return false;
}

// O(N³)
bool bl_linhas_b(Solucao &atual)
{
	Solucao nova = atual;
	vector<pair<int, int>> ordem(nova.resultados_parciais.size() - 1);
	int melhor_resultado = nova.melhor_resultado;
	bool flag = false;

	obter_ordem(ordem);

	for (int i = 0; i < ordem.size(); i++)
	{
		int a = ordem[i].second;
		for (int j = i + 1; j < ordem.size(); j++)
		{
			int b = ordem[j].second;
			nova.trocar_volta(a, b);
			nova.recalc_resultado();

			if (nova.melhor_resultado > atual.melhor_resultado)
			{
				flag = true;
				atual = nova;
			}
			else
			{
				nova.trocar_volta(b, a);
			}
		}
	}
	return flag;
}

// O(N²)
void bl_nojump(Solucao &atual, int lado)
{
	for (int i = 0; i < atual.resultados_parciais.size() - 1; i++)
	{
		for (int j = 1; j < atual.resultados_parciais.size() - 1 - i; j++)
		{
			if (atual.caminho[j * 2 + lado] > atual.caminho[(j - 1) * 2 + lado])
				continue;
			atual.reposicionar_pares(j - 1, j);
		}
	}
	atual.recalc_resultado();
}

// O(log(N)*N)
int obter_linha_inicial(int linhas)
{
	vector<pair<int, int>> opcoes;
	for (int i = 0; i < linhas; i++)
	{
		opcoes.push_back({i * 5 + rd() % 300, i});
	}
	sort(opcoes.begin(), opcoes.end());
	return opcoes.front().second;
}

// O(log(N)*N)
void obter_ordem(vector<pair<int, int>> &v)
{
	for (int i = 0; i < v.size(); i++)
		v[i] = {i + rd() % int(OMEGA * v.size()), i};
	sort(v.begin(), v.end());
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
