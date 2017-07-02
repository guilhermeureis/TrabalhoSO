#ifndef HEADER_H
#define HEADER_H

/* Bibliotecas */

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <utility>

using namespace std;

/* Macros */

#define Thread map < string, map < int, vector < int > > >

/* Funções */
vector<string> EntradaPadrao();
vector<int> EntradaSemaforo();
vector<int> ContadorTabulacao(vector<string> v);
Thread HistoricoThreads(vector<string> v, vector<int> ContadorTabulacao);
vector<int> ContarNo(vector<string> v, vector<int> tabulacao);

void caminhosGrafos(Thread grafo, vector<int> n);
void salvaTodosCaminhosEncontrados(int inicio, int fim, bool visitado[], int caminho[], 
								int &indiceCaminho, map < int,vector < int > > g);
void salvaTodosCaminhos(int inicio, int fim, int n, map < int,vector < int > > g);
#endif
 