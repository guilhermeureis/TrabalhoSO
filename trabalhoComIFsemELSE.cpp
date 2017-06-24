#include "header.h"


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
int main(){
	Thread threads;
	vector<string> vetorPrincipal;
	vector<int> vetorSemaforo;
	vector<int> vetorContadorTabulacao;
	vetorSemaforo = EntradaSemaforo();
	vetorPrincipal = EntradaPadrao();
	vetorContadorTabulacao = ContadorTabulacao(vetorPrincipal);
	threads = HistoricoThreads(vetorPrincipal,vetorContadorTabulacao);
	//ImprimiVetor(vetorContadorTabulacao);
	ImprimiGrafo(threads);
	ImprimiVetor(vetorPrincipal);	
	//ImprimiVetor(vetorSemaforo);
	return 0;
}
// valores
/*

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

Thread HistoricoThreads(vector<string> v, vector<int> ContadorTabulacao ){
	
	string aux;
	int i=0, posicao=0, proxPosicao=0,aux2;
	bool flagUltimoElementoThread = 0;
	vector<string> vetorAux;
	map<string,vector<string> > mapAux;
	Thread grafo;

	for (int i = 0; i < v.size(); i++){
		posicao = v[i].find("f");
		if(posicao == 0){
			aux = v[i];
		}
		else{
			// Se é ultimo elemento do arquivo
			if (i==(v.size()-1)){
				grafo[aux][i].push_back(-1);
			}
			else{
				// Se é o ultimo elemento das threads
				proxPosicao = v[i+1].find("f");
				if(proxPosicao == 0){
					flagUltimoElementoThread = 1;
					cout<<"Entrei aqui 2... k = "<<"i = "<<i<<endl;
					grafo[aux][i].push_back(-1);
				}
				else{
					//Encontrar IF
					int posicaoIF = v[i].find("if");
					if(posicaoIF >= 0){
						int k=i;
						do{
							cout<<"k = "<<k<<endl;
							cout<<"FLAG:  "<<flagUltimoElementoThread<<endl;
							k++;
							if(ContadorTabulacao[i]==ContadorTabulacao[k]){
								cout<<"Entrei aqui"<<endl;
								grafo[aux][i].push_back(k);
								break;
							}
							if(k==(v.size()-1) || flagUltimoElementoThread == 1){
								cout<<"Entrei aqui 2... k = "<<k<<" i = "<<i<<endl;
								grafo[aux][i].push_back(-1);
								flagUltimoElementoThread = 0;
								break;				
							}
							
							
						}while(1);
					}
					grafo[aux][i].push_back(i+1);
					flagUltimoElementoThread = 0;
				}
			}			
		}
	}	
	return grafo;
}

