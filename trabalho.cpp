#include "header.h"

// Variaveis Globais para salvar todos os caminhos possiveis
map<string,vector<int> > mapTodosOsCaminhos;
vector<vector<int> > auxiliarCaminho;
string auxThread = "";
vector<int> vetorAuxCaminho;
vector<pair<string,int> > maioresCaminhos;

void ImprimeVetor(vector<string> v){
	for (int i = 0; i < v.size(); ++i){
		cout<<v[i]<<endl;
	}
}

void ImprimeVetor(vector<int> v){
	for (int i = 0; i < v.size(); ++i){
		cout<<v[i]<<" ";
	}
	cout<<endl;
}
void ImprimeMapeamento(){
	map<string,vector<int> > :: iterator i;
	for (i = mapTodosOsCaminhos.begin(); i != mapTodosOsCaminhos.end(); ++i){
		cout<<i->first<<": ";
		for (int j = 0; j!= i->second.size(); ++j){
			cout<<i->second[j]<<" ";
		}
		cout<<endl;
	}
}
void ImprimeGrafo(Thread t){
	
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
void ImprimeTuplas(){
	vector<pair<string,int> > :: iterator i;

	for (i = maioresCaminhos.begin(); i != maioresCaminhos.end(); ++i){
		cout<<(*i).first<<" "<<(*i).second<<" ";
	}
	cout<<endl;	
}

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

void OrdenaCaminhos(){

	vector<pair<string,int> > :: iterator i;
	pair<string, int> tuplaAux;
	bool troca = true;
	int total = maioresCaminhos.size();
	int cont = 0;
	vector<pair<string,int> > maioresCaminhosEstadoInicial = maioresCaminhos;
	while((total/2)+1 > cont){
		//cout<<"entrei"<<cont<<endl;
		maioresCaminhos = maioresCaminhosEstadoInicial;
		cont++;
		troca = true;
		while(troca){
			troca = false;
			for (i = maioresCaminhos.begin(); i != maioresCaminhos.end()-cont; ++i){
				if((*i).first<(*(i+1)).first){
					troca = true;		
					int aux = cont;
					while(aux>=0){
						//cout<<"Entrei 2 "<<aux<<endl;		
						tuplaAux = *i;
						*i = *(i+aux);
						*(i+aux) = tuplaAux;
						aux--;
					}
					break;
				}
			}
			ImprimeTuplas();
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
	vetorQtdeDeNo = ContarNo(vetorPrincipal,vetorContadorTabulacao);
	threads = HistoricoThreads(vetorPrincipal,vetorContadorTabulacao,vetorQtdeDeNo);
	//ImprimeVetor(vetorContadorTabulacao);
	ImprimeVetor(vetorQtdeDeNo);
	ImprimeGrafo(threads);
	//ImprimeVetor(vetorQtdeDeNo);
	//ImprimeVetor(vetorPrincipal);	
	//ImprimeVetor(vetorSemaforo);
	caminhosGrafos(threads,vetorQtdeDeNo);
	cout<<endl;
	//ImprimeMapeamento();
	InicializarTuplas();
	ImprimeTuplas();
	OrdenaCaminhos();
	//ImprimeMapVectorInt();
	//cout<<mapTodosOsCaminhosPossiveis["f1"].size()<<endl;
	//cout<<mapTodosOsCaminhosPossiveis["f2"].size()<<endl;
	//InicializarMapVectorTodosOsCaminhos();
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

Thread HistoricoThreads(vector<string> v, vector<int> ContadorTabulacao, vector<int> n){
	
	string aux;
	int i=0, proxPosicao=0,aux2,posicaoELSE,contadorNo=0;
	bool flagUltimoElementoThread = 0;
	vector<string> vetorAux;
	map<string,vector<string> > mapAux;
	Thread grafo;
	int ultimaPosicao = v.size();
	

	for (int i = 0; i < v.size(); i++){ 	//percorre ate encontar o ultimo elemento so arquivo
		if(ContadorTabulacao[i] == 0){ 		// Verifica se esse f e da threads atravez de tabulação 
			aux = v[i];
			if(aux.find("Final")>0)
				contadorNo++;
		}							//string aux recebe a linha do V[i] (f1, f2, f3, f4)
		else{
			if (i==(v.size()-1) || ContadorTabulacao[i+1] == 0){  			// Se é ultimo elemento do arquivo ou Se o proximo elemento e uma threads
				flagUltimoElementoThread = 1;//
				grafo[aux][i].push_back(n[contadorNo]);//grafo aponta para -1;	
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
							grafo[aux][i].push_back(n[contadorNo]);
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
		fim = n[auxiliar+1];
		tamanho = n[auxiliar+1]+1;
		
		//Zerando o vetor "vetorAuxCaminho"
		while(!vetorAuxCaminho.empty()){
			vetorAuxCaminho.pop_back();
		}
		while(!auxiliarCaminho.empty()){
			auxiliarCaminho.pop_back();
		}
		auxThread = i->first;
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
		
		for (int i = 0; i < indiceCaminho; ++i){
			vetorAuxCaminho.push_back(caminho[i]);
			//cout<<caminho[i]<<" ";
			//mapTodosOsCaminhos[auxThread] = vetorAuxCaminho;
		}
		mapTodosOsCaminhos[auxThread] = vetorAuxCaminho;
		auxiliarCaminho.push_back(vetorAuxCaminho); 
		while(!vetorAuxCaminho.empty()){
			vetorAuxCaminho.pop_back();
		}
		//cout<<endl;

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
