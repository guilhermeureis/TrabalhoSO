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
void ImprimeMapeamento( map<string, vector<int> > mapCaminhos){
	map<string,vector<int> > :: iterator i;
	for (i = mapCaminhos.begin(); i != mapCaminhos.end(); ++i){
		cout<<i->first<<": ";
		for (int j = 0; j!= i->second.size(); ++j){
			cout<<i->second[j]<<" ";
		}
		cout<<endl;
	}
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
	//ImprimeVetor(vetorQtdeDeNo);
	//ImprimeGrafo(threads);
	//ImprimeVetor(vetorQtdeDeNo);
	//ImprimeVetor(vetorPrincipal);	
	//ImprimeVetor(vetorSemaforo);
	caminhosGrafos(threads,vetorQtdeDeNo);
	//cout<<endl;
	//ImprimeMapeamento();
	InicializarTuplas();
	//ImprimeTuplas();
	OrdenaCaminhos(threads,vetorPrincipal,vetorSemaforo);
	
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
					int k=i;
					//cout<<k<<endl;						// cria uma variavel contador para percorrer ate o final do IF	
					do{			
						k++;										
						if((ContadorTabulacao[i]==ContadorTabulacao[k])||(ContadorTabulacao[k]==0)) { //determina se e um else e se e o else daquele if pela tabulação 
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

void salvaTodosCaminhosEncontrados(int inicio, int fim, bool visitado[], int caminho[], int &indiceCaminho, map < int,vector < int > > g){
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

void OrdenaCaminhos(Thread t, vector<string> v, vector<int> disponivel){

	vector<pair<string,int> > :: iterator i;
	map<int, vector<int> > :: iterator aux;
	Thread :: iterator j;
	vector<string> vetorAux;
	bool resultado;
	//
	vector<pair<string,int> > maioresCaminhosEstadoInicial = maioresCaminhos;
	for (j = t.begin(); j != t.end(); ++j){
		for (aux = j->second.begin(); aux != j->second.end(); ++aux)
		{
			vetorAux.push_back(j->first);
			//cout<<j->first<<endl;	
		}
		
	}
	
	do{
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

		ImprimeVetor(caminhoAleatorio);
		ImprimeVetor(disponivel);
		if(AlgoritmoBanqueiro (t,v,disponivel,caminhoAleatorio) == true)
			return;
		
	}while(resultado = next_permutation(vetorAux.begin(),vetorAux.end()));
	
}
bool AlgoritmoBanqueiro (Thread t, vector<string> v, vector<int> disponivel, vector<int> combinacoes){
	

	vector<pair<string,int> > :: iterator i;
	map<string,vector<int> > necessidade;
	map<string,vector<int> > alocacao; //matriz de alocação
	vector<int> iniciaVetor;
	iniciaVetor.resize(disponivel.size());
	Thread :: iterator inicial;		
	

	//ImprimeVetor(combinacoes);
	//cout<<endl;
	for (inicial = t.begin(); inicial != t.end(); ++inicial){
		necessidade[inicial->first] = iniciaVetor;
		alocacao[inicial->first] = iniciaVetor;		
	}
	
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
		Threads = EncontraThreads(t,combinacoes,i); // descobrimos de qual threads e o P ou v
		cout<<"Thread: "<<Threads<<endl;			
		if (PeV[i] == 'p'){  // encontramos o P
			//Threads = EncontraThreads(t,combinacoes,i); // descobrimos de qual threads e o P 
			cout<< "entrei no PPPPPPPPP:  " << endl;
			for(aloc = alocacao.begin(),nesc=necessidade.begin(); aloc != alocacao.end()  && nesc != alocacao.end() ; aloc++,nesc++){	
				if(aloc->first== Threads && nesc->first== Threads){				//descobrimos a coluna na matriz esta aquele ptem que alterar 
					nesc->second[numeros[i]]++;
					if(disponivel[numeros[i]]!= 0){
						aloc->second[numeros[i]]++;
						disponivel[numeros[i]]--;
						nesc->second[numeros[i]]--;
						//printf("esta travado\n");
					}
					//else{
						//printf("não esta travado\n");
					//	aloc->second[numeros[i]]++;
					//	disponivel[numeros[i]]--;
					//	nesc->second[numeros[i]]--;
					//}
				}
			}
		}
		if (PeV[i] == 'v'){
			//Threads = EncontraThreads(t,combinacoes,i);
			cout<< "entrei no VVVVVVVVVVVVVVV:  " << endl;	
			for(aloc = alocacao.begin(),nesc=necessidade.begin(); aloc!= alocacao.end() && nesc!= alocacao.end() ; aloc++,nesc++){	
				if(aloc->first == Threads ){				//descobrimos a coluna na matriz esta aquele ptem que alterar 
					/*if(nesc->second[numeros[i]]>0){
							nesc->second[numeros[i]]--;
							aloc->second[numeros[i]]++;
						
						}*/
					if(aloc->second[numeros[i]]==0){
						//printf("não tem menoria para liberar"); 
							
						map<string, vector<int> > :: iterator auxiliar;
						auxiliar = alocacao.begin();
						while(1){
							if (auxiliar->second[numeros[i]] > 0){
								auxiliar->second[numeros[i]]--;
								disponivel[numeros[i]]++;
								if(nesc->second[numeros[1]] > 0)
											
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
		cout<< "disponivel" <<endl;
		ImprimeVetor(disponivel);
		cout<< "necessidade" <<endl;	
		ImprimeMapeamento(necessidade);
		cout<< "alocacao" <<endl;
		ImprimeMapeamento(alocacao);

		if (EncontraDeadlock(necessidade,alocacao, disponivel) == true){
			cout << "dead lock"<< endl;
			for(int o=0; o<=i;o++){
				Threads = EncontraThreads(t,combinacoes,o);
				cout << Threads<< ": " <<v[combinacoes[o]] << endl;
			}//printar o caminhos com deadlock;
			return true;	
		}
		//verificar matriz de nescessida se esta em deadlock
	}
	return false;
}

string EncontraThreads(Thread t, vector<int>combinacoes, int l){
	
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
			//cout << "termino["<<k<<"]: "<< termino[k] << endl;		
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
					//cout << "cont:  " << cont << endl;
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

