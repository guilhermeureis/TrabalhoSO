/**
*	@file header.h 
*	@brief biblioteca que contem, bibliotecas padrão e as assinaturas das funçoes utilizadas no trabalho
*
*
*	@author Filipe Nascimento Caniçali
*	@author Guilherme uliana dos Reis 
*
*/
#ifndef HEADER_H
#define HEADER_H

/* Bibliotecas */
#include <bits/stdc++.h>
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
Thread HistoricoThreads(vector<string> v, vector<int> ContadorTabulacao, vector<int> n);
vector<int> ContarNo(vector<string> v, vector<int> tabulacao);
void caminhosGrafos(Thread grafo, vector<int> n);
void salvaTodosCaminhosEncontrados(int inicio, int fim, bool visitado[], int caminho[], int &indiceCaminho, map < int,vector < int > > g);
void salvaTodosCaminhos(int inicio, int fim, int n, map < int,vector < int > > g);
bool AlgoritmoBanqueiro (Thread t, vector<string> v, vector<int> disponivel, vector<int> combinacoes);
string buscaThreads(Thread t, vector<int>combinacoes, int l);
void InicializarTuplas();
void OrdenaCaminhos(Thread t, vector<string> v, vector<int> disponivel);
bool EncontraDeadlock (map<string, vector<int> > necessidade, map<string, vector<int> > alocacao, vector <int> disponivel);
#endif
 