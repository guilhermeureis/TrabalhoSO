/**
*	@file trabalho.cpp 
*	@brief Detecção de deadlock.
*
*	@section Introdução
*
*	Trabalho consiste e analisar uma sequência afim de verificar se um programa está suscetivel a deadlock
	monstrando a sequência que deu deadlock.	
*
*	@author Filipe Nascimento Caniçali
*	@author Guilherme uliana dos Reis 
*
*/

#include "header.h"

///Variaveis Globais
map<string,vector<int> > mapTodosOsCaminhos;
vector<vector<int> > auxiliarCaminho;
string auxThread = "";
vector<int> vetorAuxCaminho;
vector<pair<string,int> > maioresCaminhos;

/** 
* Função Principal do Programa
*
*/
int main(){
	Thread threads;
	vector<string> vetorPrincipal;
	vector<int> vetorSemaforo;
	vector<int> vetorContadorTabulacao;
	vector<int> vetorQtdeDeNo;
	vetorSemaforo = EntradaSemaforo();
	vetorPrincipal = EntradaPadrao();
	vetorContadorTabulacao = ContadorTabulacao(vetorPrincipal);
	vetorQtdeDeNo = ContarNo(vetorPrincipal,vetorContadorTabulacao);
	threads = HistoricoThreads(vetorPrincipal,vetorContadorTabulacao,vetorQtdeDeNo);
	caminhosGrafos(threads,vetorQtdeDeNo);
	InicializarTuplas();
	OrdenaCaminhos(threads,vetorPrincipal,vetorSemaforo);
	
	return 0;
}

/** 
* Função que captura a todas as demais linhas da entrada e salva em um vetor de string que representa
* todos os comandos da entrada.
*/
vector<string> EntradaPadrao(){
	
	stringstream buffer;
	vector <string> v;
	string aux,aux2;
	int posicao;

	while(getline(cin,aux)){
		if(aux != "\0"){	
			posicao = aux.find("thread");
			if(posicao == 0){
				aux2 = aux.substr(posicao+7);
				v.push_back(aux2);
			}
			else{
				v.push_back(aux);
			}
		}
	}
	v.push_back("Final");
	return v;
}
/** 
* Função que captura a primeira linha da entrada e salva em um vetor de inteiros que representa
* a inicialização dos semáforos.
*/
vector<int> EntradaSemaforo(){
	
	stringstream buffer;
	vector <int> v;
	int aux,qtde;
	string nome;
	getline(cin,nome);
	buffer<<nome;
	if(nome.size()!=0)
		qtde = nome.size()+1;
	else
		qtde = nome.size();
	qtde = qtde/2;
	for (int i = 0; i < qtde; ++i){
		buffer>>aux;
		v.push_back(aux);
	}
	return v;
}
/** 
* Função que faz a contagem das tabulações de todas as linhas da entrada e salva em um vetor de inteiros.
* @param v é um vector de string.
*/
vector<int> ContadorTabulacao(vector<string> v){
	int cont;
	vector<int> contador;
	for (int i = 0; i < v.size(); ++i){
		cont=0;
		for (int j = 0; j < v[i].size(); ++j)
		{
			if(v[i][j]=='\t'){
				cont++;
			}
		}
		contador.push_back(cont);
	}
	return contador;
}
/** 
* Função que salva todos as informações em um grafo.
* @param v é um argumento do tipo vector de string.
* @param ContadorTabulacao é um argumento do tipo vector de inteiro.
* @param n é um argumento do tipo vector de inteiro.
*/
Thread HistoricoThreads(vector<string> v, vector<int> ContadorTabulacao, vector<int> n){
	
	string aux;
	int i=0, proxPosicao=0,aux2,posicaoELSE,contadorNo=0;
	bool flagUltimoElementoThread = 0;
	vector<string> vetorAux;
	map<string,vector<string> > mapAux;
	Thread grafo;
	int ultimaPosicao = v.size();
	

	for (int i = 0; i < v.size(); i++){ 	///percorre até encontrar o último elemento so arquivo
		if(ContadorTabulacao[i] == 0){ 		/// Verifica se esse f é da threads através de tabulação 
			aux = v[i];
			if(aux.find("Final")>0)
				contadorNo++;
		}							///string aux recebe a linha do V[i] (f1, f2, f3, f4)
		else{
			if (i==(v.size()-1) || ContadorTabulacao[i+1] == 0){  /// Se é ultimo elemento do arquivo ou Se o proximo elemento e uma threads
				flagUltimoElementoThread = 1;//
				grafo[aux][i].push_back(n[contadorNo]);	
			}
			else{
				int posicaoIF = v[i].find("if"); ///identifica se possui IF 
				if(posicaoIF > -1){              ///caso encontre o if, valor da posição será maior que -1
					int k=i;	
					do{			
						k++;
						///determina se é um else e se é o else daquele if pela tabulação										
						if((ContadorTabulacao[i]==ContadorTabulacao[k])||(ContadorTabulacao[k]==0)) {  
 							grafo[aux][i].push_back(k);
							break;
						}
						///Se for último elemento da thread ou último elemento do arquivo
						if(k==(v.size()-1) || flagUltimoElementoThread == 1){  
							grafo[aux][i].push_back(n[contadorNo]);
							flagUltimoElementoThread = 0;
							break;				
						}
						

					}while(1);
				}
				posicaoELSE = v[i+1].find("else"); ///proximo elemento é o else 
				if (posicaoELSE > -1){				/// se for maior que -1 é o else 
					int j = i+1;
					do{
						j++;
						if (ContadorTabulacao[j]==ContadorTabulacao[i+1]){
							grafo[aux][i].push_back(j);
							break;								
						}
						if(j==(v.size()-1) || ContadorTabulacao[j] == 0){
							grafo[aux][i].push_back(n[contadorNo]);
							break;
						}
						
					}while(1);
					
				}
				else{
					grafo[aux][i].push_back(i+1);
					flagUltimoElementoThread = 0;
				}
			}
						
		}
	}	
	return grafo;
}
/** 
* Função que faz a contagem dos Nós existentes em cada grafo.
* @param v é um argumento do tipo vector de string.
* @param tabulacao é um argumento do tipo vector de inteiro.
*/
vector<int> ContarNo(vector<string> v, vector<int> tabulacao){
	vector<int> vetorQuantidadeNO;
	for (int i = 0; i < v.size(); ++i)
	{
		if(tabulacao[i] == 0){
			vetorQuantidadeNO.push_back(i);
		}
	}

	return vetorQuantidadeNO;
}

void caminhosGrafos(Thread grafo, vector<int> n){
	int auxiliar,inicio, fim,tamanho;
	auxiliar=0;
	Thread :: iterator i;
	for (i=grafo.begin(); i!= grafo.end(); i++)
	{

		inicio = n[auxiliar]+1;
		fim = n[auxiliar+1];
		tamanho = n[auxiliar+1]+1;
		
		///Liberando o espaço do vetor "vetorAuxCaminho"
		while(!vetorAuxCaminho.empty()){
			vetorAuxCaminho.pop_back();
		}
		///Liberando o espaço do vetor "vetorAuxCaminho"
		while(!auxiliarCaminho.empty()){
			auxiliarCaminho.pop_back();
		}
		auxThread = i->first;
		salvaTodosCaminhos(inicio,fim,tamanho,i->second);		
		auxiliar++;
	}
}
/** 
* Função que faz a chamada da função de recursão e inicializa as variáveis.
* @param inicio é um argumento inteiro.
* @param fim é um argumento inteiro.
* @param n é um argumento inteiro.
* @param g é um argumento de mapeamento.
*/

void salvaTodosCaminhos(int inicio, int fim, int n, map < int,vector < int > > g){
	/// Vetor de vertices não visitados
	bool visitado[n];
	///Vetor que armazena os caminhos
	int caminho[n];
	int indiceCaminho= 0;

	for (int i = 0; i < n; ++i){
		visitado[i] = false;
	}

	salvaTodosCaminhosEncontrados(inicio,fim,visitado,caminho,indiceCaminho,g);
}
/** 
* Função recursiva que salva todos os maiores caminhos.
* @param inicio é um argumento inteiro.
* @param fim é um argumento inteiro.
* @param visitado[] é um argumento do tipo vetor boolean.
* @param indiceCaminho é um argumento de endereço inteiro.
* @param g é um argumento de mapeamento.
*/
void salvaTodosCaminhosEncontrados(int inicio, int fim, bool visitado[], int caminho[], int &indiceCaminho, map < int,vector < int > > g){
	visitado[inicio] = true;
	caminho[indiceCaminho] = inicio;
	indiceCaminho++;
	if(inicio == fim){		
		
		for (int i = 0; i < indiceCaminho; ++i){
			vetorAuxCaminho.push_back(caminho[i]);
		}
		mapTodosOsCaminhos[auxThread] = vetorAuxCaminho;
		auxiliarCaminho.push_back(vetorAuxCaminho); 
		while(!vetorAuxCaminho.empty()){
			vetorAuxCaminho.pop_back();
		}

	}
	else{
		vector< int> :: iterator i;
		for (i = g[inicio].begin(); i != g[inicio].end(); ++i){
			if (!visitado[*i]){			
				salvaTodosCaminhosEncontrados(*i, fim, visitado, caminho, indiceCaminho, g);
			}
		}
	}
	indiceCaminho--;
	visitado[inicio] = false;
}
/** 
* Função que inicializa um vector de pair onde é salvado o caminho de cada thread.
*/
void InicializarTuplas(){
	map<string, vector<int> > :: iterator i;
	pair<string, int> tuplaAux;
	for (i = mapTodosOsCaminhos.begin(); i != mapTodosOsCaminhos.end(); ++i){
		for (int j = 0; j!= i->second.size(); ++j){
			tuplaAux.first = i->first;
			tuplaAux.second = i->second[j];
			maioresCaminhos.push_back(tuplaAux);
		}
	}

}
/** 
* Função que simula a região crítica ordenando todos os caminhos possíveis.
* @param t é um argumento do tipo Thread.
* @param v é um argumento do tipo vector de string.
* @param disponivel é um argumento do tipo vector de inteiro.
*/
void OrdenaCaminhos(Thread t, vector<string> v, vector<int> disponivel){

	vector<pair<string,int> > :: iterator i;
	map<int, vector<int> > :: iterator aux;
	Thread :: iterator j;
	vector<string> vetorAux;
	bool resultado,flag;
	//
	vector<pair<string,int> > maioresCaminhosEstadoInicial = maioresCaminhos;
	for (j = t.begin(); j != t.end(); ++j){
		for (aux = j->second.begin(); aux != j->second.end(); ++aux)
		{
			vetorAux.push_back(j->first);	
		}
		
	}
	///Faz a permutação de todas as sequências das possíveis combinações de caminho.
	///Simulação da região crítica.
	do{
		flag = true;
		vector<int> caminhoAleatorio;
		maioresCaminhos = maioresCaminhosEstadoInicial;
		for (int k = 0; k<vetorAux.size(); ++k){
			for (i = maioresCaminhos.begin(); i != maioresCaminhos.end(); ++i){
				if((*i).first == vetorAux[k]){
					caminhoAleatorio.push_back((*i).second);
					maioresCaminhos.erase(i);
					break;
				}
			}
		}

		if(AlgoritmoBanqueiro (t,v,disponivel,caminhoAleatorio) == true){
			flag=false;
			break;
		}	
	}while(resultado = next_permutation(vetorAux.begin(),vetorAux.end()));
	if(flag==true){
		cout << "programa livre de impasse. " << endl;
	}
}
/** 
* Função que analisa os tipos de nós de cada Grafo e analisa se contém DEADLOCK.
* @param t é um argumento do tipo Thread.
* @param v é um argumento do tipo vector de string.
* @param disponivel é um argumento do tipo vector de inteiro.
* @param combinacoes é um argumento do tipo vector de inteiro.
*/
bool AlgoritmoBanqueiro (Thread t, vector<string> v, vector<int> disponivel, vector<int> combinacoes){
	

	
	map<string,vector<int> > necessidade;   /// Matriz de necessidade.
	map<string,vector<int> > alocacao;     ///Matriz de alocação.
	vector<int> iniciaVetor;
	iniciaVetor.resize(disponivel.size());
	Thread :: iterator inicial;		

	///Inicialização das duas matrizes com zeros.
	for (inicial = t.begin(); inicial != t.end(); ++inicial){
		necessidade[inicial->first] = iniciaVetor;
		alocacao[inicial->first] = iniciaVetor;		
	}
	
	int numeros[combinacoes.size()];
	char PeV[combinacoes.size()];
	stringstream buffer;
	int pIF;
	int pELSE;
	///Identificação dos tipos existentes em cada nó do grafo.
	for(int i=0; i<combinacoes.size();i++){
			pIF = v[combinacoes[i]].find("if"); 
			pELSE = v[combinacoes[i]].find("else");
			if((pIF<0) &&(pELSE<0)){
				buffer<<v[combinacoes[i]];
				buffer >> PeV[i]; 
				buffer >> numeros[i]; 
				buffer.clear();
			}
			else{
				PeV[i] = '*';
				numeros[i] = -1;
			}
			
	}
	
	map<string, vector<int> > :: iterator aloc;
	map<string, vector<int> > :: iterator nesc;		
	string Threads;
	
	for(int i=0; i<combinacoes.size();i++){
		/// Encontrando de qual thread é o "p" ou "v"		
		Threads = buscaThreads(t,combinacoes,i); 
		/// Condição se for um "p"
		if (PeV[i] == 'p'){  
			for(aloc = alocacao.begin(),nesc=necessidade.begin(); aloc != alocacao.end()  && nesc != alocacao.end() ; aloc++,nesc++){	
				if(aloc->first== Threads && nesc->first== Threads){				
					nesc->second[numeros[i]]++;
					if(disponivel[numeros[i]]!= 0){
						aloc->second[numeros[i]]++;
						disponivel[numeros[i]]--;
						nesc->second[numeros[i]]--;
						
					}
				}
			}
		}
		if (PeV[i] == 'v'){
			/// Condição se for um "v"
			for(aloc = alocacao.begin(),nesc=necessidade.begin(); aloc!= alocacao.end() && nesc!= alocacao.end() ; aloc++,nesc++){	
				if(aloc->first == Threads ){				
					if(nesc->second[numeros[i]]>0){
							nesc->second[numeros[i]]--;
							aloc->second[numeros[i]]++;
						
						}
					if(aloc->second[numeros[i]]==0){
						map<string, vector<int> > :: iterator auxiliar;
						auxiliar = alocacao.begin();
						while(1){
							if (auxiliar->second[numeros[i]] > 0){
								auxiliar->second[numeros[i]]--;
								disponivel[numeros[i]]++;
								if(nesc->second[numeros[i]] > 0){
									nesc->second[numeros[i]]--;
									aloc->second[numeros[i]]++;
									break;		
								}
							}
							if(auxiliar == alocacao.end()){
								break;
							}
							auxiliar++;
						}
					}
					else{	
						aloc->second[numeros[i]]--;
						disponivel[numeros[i]]++;
					}
				}
	 		}
		}
		
		string auxiliarThreads;
		if (EncontraDeadlock(necessidade,alocacao, disponivel) == true){
			for(int o=0; o<=i;o++){
				auxiliarThreads = buscaThreads(t,combinacoes,o);
				cout << auxiliarThreads<< ": "<< " " <<v[combinacoes[o]] << endl;
			}
			return true;	
		}
	}
	return false;
}
/** 
* Função que busca a thread(grafo) que o nó pertence.
* @param t é um argumento do tipo Thread.
* @param combinacoes é um argumento do tipo vector de inteiro.
* @param l é um argumento do tipo inteira
*/
string buscaThreads(Thread t, vector<int>combinacoes, int l){
	
	Thread :: iterator i;
	map<int, vector<int> > :: iterator j;
	
	for (i = t.begin(); i != t.end(); ++i){
		for (j = i->second.begin(); j != i->second.end(); ++j){
			if(j->first == combinacoes[l]){
				string threads = i->first;
				return threads;
			}
		}
	}
	string aux = "nao encontrado";
	return aux;
}
/** 
* Função que encontra deadlock.
* @param necessidade é um argumento de mapeamento.
* @param alocacao é um argumento de mapeamento.
* @param disponivel é um argumento do tipo vector de inteiro.
*/
bool EncontraDeadlock (map<string, vector<int> > necessidade, map<string, vector<int> > alocacao, vector <int> disponivel){
	int deadlock = 0;
	vector<bool> termino;
	termino.resize(2);
	int cont=0, k=0;
	for(int i=0; i<termino.size();i++){
		termino[i] = true;	
	}
	map<string, vector<int> > :: iterator j;
	map<string, vector<int> > :: iterator aloc;

	for(aloc = alocacao.begin(); aloc!= alocacao.end() ;aloc++){
		cont = 0;
		for(int i = 0;i<aloc->second.size();i++){
			if(aloc->second[i] == 0){
				cont++;
			}
		}
		if(cont != aloc->second.size()){
			termino[k] = false;
			
		}
		k++;
	}
	k=0;
	for(aloc = alocacao.begin(),j = necessidade.begin(); aloc != alocacao.end() && j != necessidade.end() ; aloc++,j++){
		cont = 0;
		if(termino[k] == false ){
			for(int i = 0; i<j->second.size();i++){
				if(j->second[i] <= disponivel[i]){
					cont++;
				}
			}
			if(cont == j->second.size()){
				for(int l=0; l<termino.size(); l++){
					disponivel[l] = disponivel[l] + aloc->second[l]; 	
				}
				termino[k] = true;
			}
		}	
	k++;
	}
	for(k =0; k<termino.size(); k++){
		if(termino[k] == false){
			deadlock++;
		}
	}
	if(deadlock > 1){
		return true;
	}
	else{
		return false;
	}	
}

