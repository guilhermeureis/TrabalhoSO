#include "header.h"

// Variaveis Globais para salvar todos os caminhos possiveis
map<string,vector<int> > mapTodosOsCaminhos;
string auxThread;
vector<int> vetorAuxCaminho;

void ImprimiVetor(vector<string> v){
	for (int i = 0; i < v.size(); ++i){
		cout<<v[i]<<endl;
	}
}

void ImprimiVetor(vector<int> v){
	for (int i = 0; i < v.size(); ++i){
		cout<<v[i]<<endl;
	}
}
void ImprimiMapeamento(){
	map<string,vector<int> > :: iterator i;
	for (i = mapTodosOsCaminhos.begin(); i != mapTodosOsCaminhos.end(); ++i){
		cout<<i->first<<": ";
		for (int j = 0; j!= i->second.size(); ++j){
			cout<<i->second[j]<<" ";
		}
		cout<<endl;
	}
}
void ImprimiGrafo(Thread t){
	
	Thread :: iterator i;
	map<int, vector<int> > :: iterator j;

	for (i = t.begin(); i != t.end(); ++i){
		for (j = i->second.begin(); j != i->second.end(); ++j){
			for (int k = 0; k < j->second.size(); ++k){
				cout<<i->first<<" "<<j->first<<" "<<j->second[k]<<endl;
			}
			
		}
	}
}

int main(){
	Thread threads;
	vector<string> vetorPrincipal;
	vector<int> vetorSemaforo;
	vector<int> vetorContadorTabulacao;
	vector<int> vetorQtdeDeNo;
	vetorSemaforo = EntradaSemaforo();
	vetorPrincipal = EntradaPadrao();
	vetorContadorTabulacao = ContadorTabulacao(vetorPrincipal);
	threads = HistoricoThreads(vetorPrincipal,vetorContadorTabulacao);
	vetorQtdeDeNo = ContarNo(vetorPrincipal,vetorContadorTabulacao);
	//ImprimiVetor(vetorContadorTabulacao);
	ImprimiGrafo(threads);
	//ImprimiVetor(vetorQtdeDeNo);
	//ImprimiVetor(vetorPrincipal);	
	//ImprimiVetor(vetorSemaforo);
	caminhosGrafos(threads,vetorQtdeDeNo);
	ImprimiMapeamento();
	return 0;
}

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

Thread HistoricoThreads(vector<string> v, vector<int> ContadorTabulacao ){
	
	string aux;
	int i=0, proxPosicao=0,aux2,posicaoELSE;
	bool flagUltimoElementoThread = 0;
	vector<string> vetorAux;
	map<string,vector<string> > mapAux;
	Thread grafo;
	int ultimaPosicao = v.size();
	

	for (int i = 0; i < v.size(); i++){ 	//percorre ate encontar o ultimo elemento so arquivo
		if(ContadorTabulacao[i] == 0){ 		// Verifica se esse f e da threads atravez de tabulação 
			aux = v[i];
		}							//string aux recebe a linha do V[i] (f1, f2, f3, f4)
		else{
			if (i==(v.size()-1) || ContadorTabulacao[i+1] == 0){  			// Se é ultimo elemento do arquivo ou Se o proximo elemento e uma threads
				flagUltimoElementoThread = 1;//
				grafo[aux][i].push_back(-1);//grafo aponta para -1;	
			}
			else{
				int posicaoIF = v[i].find("if"); //identifica que se possui IF 
				if(posicaoIF > -1){              //caso encontre o if, valor da posição sera maior que -1
					int k=i;					// cria uma variavel contador para percorrer ate o final do IF	
					do{			
						k++;				
						if((ContadorTabulacao[i]==ContadorTabulacao[k])) { //determina se e um else e se e o else daquele if pela tabulação 
 							grafo[aux][i].push_back(k);
							break;
						}
						if(k==(v.size()-1) || flagUltimoElementoThread == 1){  //Se for ultimo elemento ou ultimo elemento do arquivo
							grafo[aux][i].push_back(-1);
							flagUltimoElementoThread = 0;
							break;				
						}
					}while(1);
				}
				posicaoELSE = v[i+1].find("else"); //proximo elemento e o else 
				if (posicaoELSE > -1){				// se for mais -1 e o else 
					int j = i+1;
					do{
						j++;
						if (ContadorTabulacao[j]==ContadorTabulacao[i+1]){
							grafo[aux][i].push_back(j);
							break;								
						}
						if(j==(v.size()-1) || ContadorTabulacao[j] == 0){
							grafo[aux][i].push_back(-1);
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
//tipo Thread == map<string, map<int, vector<int>>>
void caminhosGrafos(Thread grafo, vector<int> n){
	int auxiliar,inicio, fim,tamanho;
	auxiliar=0;
	Thread :: iterator i;
	for (i=grafo.begin(); i!= grafo.end(); i++)
	{
		inicio = n[auxiliar]+1;
		fim = -1;
		tamanho = n[auxiliar+1]-1;
		//Zerando o vetor "vetorAuxCaminho"
		for (int k = 0; k < vetorAuxCaminho.size(); ++k)
		{
			vetorAuxCaminho.erase(vetorAuxCaminho.begin());
		}
		auxThread = i->first;
		cout<<"auxThread: "<<auxThread<<endl;
		salvaTodosCaminhos(inicio,fim,tamanho,i->second);		
		auxiliar++;
	}
}

void salvaTodosCaminhos(int inicio, int fim, int n, map < int,vector < int > > g){
	// Vetor de vertices não visitados
	bool visitado[n];
	//Vetor que armazena os caminhos
	int caminho[n];
	int indiceCaminho= 0;

	for (int i = 0; i < n; ++i){
		visitado[i] = false;
	}

	salvaTodosCaminhosEncontrados(inicio,fim,visitado,caminho,indiceCaminho,g);
}

void salvaTodosCaminhosEncontrados(int inicio, int fim, bool visitado[], int caminho[], 
								int &indiceCaminho, map < int,vector < int > > g){
	visitado[inicio] = true;
	caminho[indiceCaminho] = inicio;
	indiceCaminho++;
	if(inicio == fim){

		//Não imprimir  o ultimo elemento(alterei o for)
		cout<<endl;
		for (int i = 0; i < indiceCaminho; ++i){
			cout<<caminho[i]<<" ";
			vetorAuxCaminho.push_back(caminho[i]);
			mapTodosOsCaminhos[auxThread] = vetorAuxCaminho;
		}
		cout<<"aqui"<<endl;
		
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

