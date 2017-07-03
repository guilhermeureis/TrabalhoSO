#include "header.h"
#include <stdio.h>

void ImprimiMapeamento( map<string, vector<int> > mapTodosOsCaminhos){
	map<string,vector<int> > :: iterator i;
	for (i = mapTodosOsCaminhos.begin(); i != mapTodosOsCaminhos.end(); ++i){
		cout<<i->first<<": ";
		for (int j = 0; j!= i->second.size(); ++j){
			cout<<i->second[j]<<" ";
		}
		cout<<endl;
	}
}

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
	banqueiro (threads);
	//ImprimiVetor(vetorQtdeDeNo);
	//ImprimiVetor(vetorPrincipal);	
	//ImprimiVetor(vetorSemaforo);
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

	
	for (int i = 0; i < v.size(); i++){ 	//percorre ate encontar o ultimo elemento so arquivo
		if(ContadorTabulacao[i] == 0) 		// Verifica se esse f e da threads atravez de tabulação 
			aux = v[i];						//string aux recebe a linha do V[i] (f1, f2, f3, f4)
		else{
			if (i==(v.size()-1) || ContadorTabulacao[i+1] == 0){  			// Se é ultimo elemento do arquivo ou Se o proximo elemento e uma threads
				flagUltimoElementoThread = 1;//
				grafo[aux][i].push_back(-1);//grafo aponta para -1;	
			}
			else{
				//if(ContadorTabulacao[i+1] == 0){			//Se o proximo elemento e uma threads
				//	flagUltimoElementoThread = 1;//
				//	grafo[aux][i].push_back(-1);// grafo apontar pra -1
				//}
				//else{
					//Encontrar IF
				int posicaoIF = v[i].find("if"); //identifica que se possui IF 
				if(posicaoIF > -1){              //caso encontre o if, valor da posição sera maior que -1
					int k=i;					// cria uma variavel contador para percorrer ate o final do IF	
					do{			
						k++;				
						//posicaoELSE = v[k].find("else");	// determina se a posição k e um ELSE	
						if(/*(posicaoELSE >= 0 && ContadorTabulacao[i]==ContadorTabulacao[k]) ||*/ (ContadorTabulacao[i]==ContadorTabulacao[k])) { //determina se e um else e se e o else daquele if pela tabulação 
 							grafo[aux][i].push_back(k);
							break;
						}
					//	
					//	if((ContadorTabulacao[i]==ContadorTabulacao[k])){
					//		grafo[aux][i].push_back(k);
					//		break;
					//	}
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
	int aux=0;
	cout<<v.size()<<endl;
	for (int i = 1; i < v.size(); ++i)
	{
		if(tabulacao[i] == 0){
			vetorQuantidadeNO.push_back(aux);
			aux=0;
			i++;
		}
		aux++;
		if(i==(v.size()-1)){
			vetorQuantidadeNO.push_back(aux);
		}
	}

	return vetorQuantidadeNO;
}

int banqueiro (Thread t /*vector<int> disponivel*/){
	vector<string> v ;//= {"p 1","p 2","p 3","v 1","v 2","v 3"}; e o vetor lido do arquivo
		v.push_back("f1");	v.push_back("p 1"); v.push_back("p 0"); v.push_back("v 0"); v.push_back("p 1"); v.push_back("f2"); v.push_back("p 0"); v.push_back("v 1");v.push_back("p 1");v.push_back("p 0");
	 // tem um possivel erro devido ao salto da linhas em quando encontra uma threads
	vector<int> combinacoes ;//{1,2,5,6,7,8,3,4,-1,1,5,6,7,8,2,3,4,-1} ; são as combinaçoes entregues
	combinacoes.push_back(1); combinacoes.push_back(6); combinacoes.push_back(7); combinacoes.push_back(8); combinacoes.push_back(9); combinacoes.push_back(2); combinacoes.push_back(3); combinacoes.push_back(4); 
	
	map<string,vector<int> > alocacao; //matriz de alocação
	vector<int> e;
	e.resize(2);
	alocacao["f1"] = e ;
	alocacao["f2"] = e ;
	
	map<string,vector<int> > necessidade;
	necessidade["f1"] = e;
	necessidade["f2"] = e;
	
	vector<int> disponivel;
	disponivel.push_back(1);
	disponivel.push_back(1);

	// converter o vetor v em um de PeV e numeros;
	int numeros[combinacoes.size()];
	char PeV[combinacoes.size()];
	stringstream buffer;
	for(int i=0; i<combinacoes.size();i++){
			buffer<<v[combinacoes[i]];
			buffer >> PeV[i]; 
			buffer >> numeros[i]; 
			//cout << "PeV " << PeV[i] << " Numeros " << numeros[i] << "\n" << endl;
			buffer.clear();
	}

	map<string, vector<int> > :: iterator aloc;
	map<string, vector<int> > :: iterator nesc;		
	string Threads;
	for(int i=0; i<combinacoes.size();i++){		
		Threads = EncontarTheads(t,combinacoes,i); // descobrimos de qual threads e o P ou v
		if (PeV[i] == 'p'){  // encontramos o P
			//Threads = EncontarTheads(t,combinacoes,i); // descobrimos de qual threads e o P 
			//cout<< "g:  " << g << endl;
			for(aloc = alocacao.begin(),nesc=necessidade.begin(); aloc != alocacao.end()  && nesc != alocacao.end() ; aloc++,nesc++){	
				if(aloc->first== Threads && nesc->first== Threads){				//descobrimos a coluna na matriz esta aquele ptem que alterar 
					nesc->second[numeros[i]]++;
					if(disponivel[numeros[i]]== 0){
						//printf("esta travado\n");
					}
					else{
						//printf("não esta travado\n");
						aloc->second[numeros[i]]++;
						disponivel[numeros[i]]--;
						nesc->second[numeros[i]]--;
					}
				}
			}
		}
		if (PeV[i] == 'v'){
			//Threads = EncontarTheads(t,combinacoes,i);
			//cout<< "g:  " << g << endl;	
			for(aloc = alocacao.begin(),nesc=necessidade.begin(); aloc!= alocacao.end() && nesc!= alocacao.end() ; aloc++,nesc++){	
				if(aloc->first == Threads ){				//descobrimos a coluna na matriz esta aquele ptem que alterar 
					if(aloc->second[numeros[i]]==0){
						//printf("não tem menoria para liberar"); 
						disponivel[numeros[i]]++;	
						map<string, vector<int> > :: iterator auxiliar;
						auxiliar = alocacao.begin();
						while(1){
							if (auxiliar->second[numeros[i]] > 0){
								auxiliar->second[numeros[i]]--;
								break;		
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
		if (encontradeadlock(necessidade)){
			cout << "dead lock"<< endl;
			for(int o=0; o<=i;o++){
				Threads = EncontarTheads(t,combinacoes,o);
				cout << Threads<< ":" <<PeV[o] << numeros[o]<< " " << combinacoes[o] << endl;
			}//printar o caminhos com deadlock;
			break;	
		}
		//cout<< "necessidade" <<endl;	
		//ImprimiMapeamento(necessidade);
		//cout<< "alocacao" <<endl;
		//ImprimiMapeamento(alocacao);	
		//verificar matriz de nescessida se esta em deadlock
	}
	return 0;
}

string EncontarTheads(Thread t, vector<int>combinacoes, int l){
	
	Thread :: iterator i;
	map<int, vector<int> > :: iterator j;

	for (i = t.begin(); i != t.end(); ++i){
		for (j = i->second.begin(); j != i->second.end(); ++j){
			if(j->first == combinacoes[l]){
				string threads = i->first;
				return threads;
			}
			//for (int k = 0; k < j->second.size(); ++k){
			//	cout<<i->first<<" "<<j->first<<" "<<j->second[k]<<endl;
			//}
		}
	}
	string aux = "nao encontrado";
	return aux;
}
int encontradeadlock (map<string, vector<int> > necessidade){
	map<string, vector<int> > :: iterator j;
	int cont=0;
	for(int i=0; i< necessidade.size() ;i++){
		//cout << "necessidade.size(): " <<necessidade.size() << endl;
		for(j = necessidade.begin()  ; j!= necessidade.end();j++){
			//cout << "j->second[i]:"<< i << "  "<< j->second[i] << endl;
			if(j->second[i] == 1){
				cont++;
			}
		}
		if(cont>=2){
			return 1;
		}	
	}
	return 0;
}
