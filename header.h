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

#endif
