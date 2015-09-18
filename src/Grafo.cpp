/*
 * Grafo.cpp
 *
 *  Created on: Jul 22, 2015
 *      Author: x
 */

#include "Grafo.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <stdio.h>
#include <sstream>

using namespace std;

Grafo::Grafo() {
	init();
	cout << nodos.size() << endl;
}

Grafo::~Grafo() {
	//pthread_exit(NULL);
//	for (int i = 0; i < nodos.size(); i++)
//		delete &nodos[i];
}

vector<string> tokenizeString(const string& str, const string& delimiters)
{
   vector<string> tokens;
   // Skip delimiters at beginning.
   string::size_type lastPos = str.find_first_not_of(delimiters, 0);
   // Find first "non-delimiter".
   string::size_type pos = str.find_first_of(delimiters, lastPos);

   while (string::npos != pos || string::npos != lastPos)
    {  // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
   }
    return tokens;
}

void Grafo::load() {
	std::string arq, dir;
	std::cout << "qual o nome do arquivo?\n";
	std::cin >> arq;
	dir="/home/x/msc/redessemfio/trab/";
	arq = dir + arq;
}

void Grafo::load(string arq) {
	ifstream ifs (arq.c_str());
	string line;

	 if (ifs.is_open())
	  {
	    while ( getline (ifs,line) )
	    {
	      cout << line << '\n';

	      if (line.size()==0)
	    	  continue;

	      // nodoA nodoB custo
	      // v[0] v[1] v[2]
	      vector<string> v = tokenizeString (line, " ");
	      cout << "id: " <<  v[0] << "\n";

	      //insere os 2 nodos
	      for (int i = 0; i <= 1; i++)
	      {
			  if (procura(v[i]) < 0)
			  {
				  cout << "inserindo nodo: " << v[i] << "\n";
				  Nodo *n = new Nodo(v[i]);
				  this->addNodo(n);
			  }
	      }

	      // adiciona vertice/vizinho
	      int a = procura(v[0]);
	      if (a >= 0)
	      {
	    	  int b = procura(v[1]);

	    	  //Vertice vert (&nodos[b], std::atoi(v[2].c_str()));
	    	  // vertice = {id, custo}
	    	  int custo = std::atoi(v[2].c_str());
	    	  Vertice vert (nodos[b].getId(), custo);
	    	  cout << "inserindo vertice: "
	    			  << nodos[a].getId() << " a " <<
	    			  //<< vert.getVizinho()->getId()
	    			  nodos[b].getId()
	    			  << " peso: " << v[2] << "\n";
	    	  nodos[a].addVizinho(vert);

	    	  // tambem adiciona vertice de b para a
	    	  Vertice vertb (nodos[a].getId(), custo);
	    	  nodos[b].addVizinho(vertb);
	      }

	    }
	    ifs.close();

	    cout << "Nodos: ";
	    for (int i = 0; i < nodos.size(); i++)
	    	cout << nodos[i].getId() << " ";
	    cout << endl;
	  }
	  else cout << "Unable to open file";
}

void Grafo::loadcoords(string arq) {
	string n=arq+".coord";
	ifstream ifs (n.c_str());
	string line;

	cout << "\ncarregando coordenadas...\n";
	 if (ifs.is_open())
	  {
	    while ( getline (ifs,line) )
	    {
	      cout << line << '\n';

	      if (line.size()==0)
	    	  continue;

	      vector<string> v = tokenizeString (line, " ");
	      //cout << "id: " <<  v[0] << "\n";

	      int a = procura(v[0]);
	      if (a >= 0)
	      {
	    	  nodos[a].setCoord(std::atoi(v[1].c_str()),
	    			  std::atoi(v[2].c_str()));
	      }

	    }
	    cout << "\nfinished\n";
	    ifs.close();
	  }
	  else cout << "Unable to open coord file";
}


void Grafo::list() {

	// imprime lista de nodos
	cout << "nodos:";
	for (int i=0; i<nodos.size();i++)
	{
		cout << nodos[i].getId() << " ";
	}
	cout << "\n";

	// imprime vizinhos
	for (int i = 0; i < nodos.size(); i++)
	{
		cout << "nodo: " << nodos[i].getId() << "\n";

		vector<Vertice> v = nodos[i].getVizinhos();

		int vizinhos = v.size();
		cout << "vertices: " << vizinhos << "\n";

		for (int j=0; j < vizinhos; j++)
			cout << "j: " << j << " " << v[j].getId() << //v[j].getVizinho()->getId() <<
				" peso: " << v[j].getCusto() << "\n";

		cout << "\n";
	}
}

int Grafo::procura(string id) {

	for (int i=0; i<nodos.size();i++)
	{
		if (id == nodos[i].getId())
			return i;
	}
	return -1;
}

int Grafo::procura(int x, int y) {

	for (int i=0; i<nodos.size();i++)
	{
		int x0=nodos[i].getX();
		int y0=nodos[i].getY();

		if ((abs(x-x0)<1) &&
				(abs(y-y0)<1))
			return i;
	}
	return -1;
}


void Grafo::addNodo(Nodo *n) {
	nodos.push_back(*n);
}

int min (int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}

void Grafo::init() {
	if (nodos.size()>0)
	{
		nodos.clear();
	}
}

string Grafo::caminhomaiscurto(string n1, string n2) {
	vector<string> N; // conjunto dos nodos com custo conhecido
	vector<string> P; // nodo anterior (p(v))
	vector<int> D;  // distancias calculadas
	int nnodos = nodos.size();

	int iu = procura(n1);
	string u = nodos[iu].getId();

	int ifinal = procura(n2);

	std::ostringstream out;
	out << "nodo inicial: " << u << ", nodo final: " << n2 << endl;

	// N = {u}
	N.push_back(u);

	// inicializacao do vetor D; custos infinitos
	for (int i = 0; i < nodos.size(); i++)
	{
		//out << "nodo[i]: " << nodos[i].getId() << " ";
		D.push_back(getCusto(u, nodos[i].getId()));
		P.push_back(u);
//		out << "custo: " << D[i] << "\n";
	}
//	out << endl;

	string w;
	int ind = iu;
	bool stop = false;

	do
	{
		int Dw=Vertice::INFINITO;
		// encontra w fora de N
		for (int v = 0; v < D.size() ; v++)
		{
			// verifica se v nodo nao esta em N
			string id = nodos[v].getId();
//			out << id << " está em N?"<< endl;
			if (find (N.begin(), N.end(), id ) == N.end())
			{
//				out << id << " nao está em N"<< endl;

				// se nao esta, procura menor D(w)
				if (D[v] < Dw)
				{
					Dw = D[v];
//					out << "w: " << id <<
//							" D(w): " << Dw << "\n";
					ind = v;
				}
			}
		}

		// se não encontrou ninguem menor, sai fora
		// o nodo está isolado
		if (Dw == Vertice::INFINITO)
			break;

		// acrescenta w a N
		w = nodos[ind].getId();
		//out << "acrescenta nodo " << w << " a N\n";
		N.push_back(w);

		// imprime N
// 		out << "N'= ";
//		for (int i = 0; i < N.size(); i++)
//			cout << N[i] ;
//		out << "\n";

		// atualiza D(v) para todo v adjacente a w e nao em N
		int wi = procura(w);
		vector<Vertice> vizinhosW = nodos[wi].getVizinhos();
		int nvizinhos = vizinhosW.size();
//		out << "vizinhos de " << w << ": " << nvizinhos << "\n";
//		for (int i = 0; i < nvizinhos; i++)
//			out << vizinhosW[i].getId() << " ";
//		out << "\n";

		for (int i = 0; i < nvizinhos ; i++)
		{
			string id = vizinhosW[i].getId();
			int indv = procura(id);

			string v = nodos[indv].getId();

			// se v nao estah em N'
			if (find (N.begin(), N.end(), v ) == N.end())
			{
				// atualiza D(v) se v adjacente a w e nao em N'
				if (D[indv] > Dw + getCusto(w, id))
				{
					D[indv] = Dw + getCusto(w, id);
					P[indv]=w;
				}

//				out << "D(v)(" << v << ")= " << D[indv] << "\n";
			}
		}

//		out << "D: ";
//		for (int i = 0; i < D.size(); i++)
//			out << D[i] << " ";
//		out << "\n";

		// verifica se todos os nodos estao em N
		stop = true;
		for (int j = 0; j < nnodos; j++)
		{
			string id = nodos[j].getId();
			if (find (N.begin(), N.end(), id ) == N.end())
			{
				stop = false;
				break;
			}
		}

	}
	while (!stop);

	out << "\nTabela de custos do nodo " << n1 << ": " << endl;

	// imprime N
	out << "N: ";
	for (int i = iu; i < ifinal; i++)
		out << nodos[i].getId() << " ";
	out << "\n";

	out << "custo para nodo " << n2 << ": " << D[ifinal] << endl;
	// imprime D
	out << "D: ";
	for (int i = 0; i < D.size(); i++)
		out << D[i] << " ";
	out << "\n";

	out << "P: ";
	for (int i = 0; i < P.size(); i++)
		out << P[i] << " ";
	out << "\n";

	out << "caminho de " << n1 << " para " << n2 << ":" << endl;

	bool sai = false;
	vector<string> path;
	int n0 = 0;
	n0 = procura (P[ifinal]);
	int nf = procura(P[n0]);
	path.push_back(n2);
	path.push_back(nodos[n0].getId());
	while (!sai)
	{
		n0 = procura(P[n0]);
		if (n0 == iu)
			break;
		path.push_back(nodos[n0].getId());

	}
	path.push_back(n1);
	for (int i = path.size()-1; i >= 0; i--)
		out << path[i] << " ";
	out << endl;


	return out.str();
}

void *threadrun(void *threadid)
{
   long tid;
   tid = (long)threadid;
   cout << "running thread ID " << tid << endl;
   pthread_exit(NULL);
}


#define PI 3.14159265
void Grafo::criaRandom(int count, int r) {

	raio = r;
	int x, y;
	float ang = 360/count;

	// cria nodos
	for (int i = 0; i < count; i++)
	{
		// calcula coordenadas
		x = raio*cos(ang*i*PI/180);
		y = raio*sin(ang*i*PI/180);

		// adiciona nodo
		char Result[16]; // string which will contain the number
		sprintf ( Result, "%d", i );
		string s = string(Result);

		// inicia mutex
		pthread_mutex_t *m = new pthread_mutex_t();
		pthread_mutex_init(m, NULL);
		mutex.push_back(*m);

		// ajusta coordenadas de tela, desloca para cima e direita
		Nodo *n = new Nodo(s, x+2*raio, y+2*raio);
		//n->mutex_hello = m;
		this->addNodo(n);

//		int rc = pthread_create(&threads[i], NULL,
//				threadrun, (void *)i);
//	  if (rc)
//		 cout << "Error:unable to create thread," << rc << endl;

	  cout << "adicionado nodo: " << s << endl;
	}

	// cria vertices para todos os nodos
	for (int i = 0; i < count; i++)
	{
		cout << "adiciona vertices ao nodo: " << i << endl;

		// define quantos vertices para o nodo i
		int numv = rand() % count;
		for (int j = 0; j < numv; j++)
		{
			// define vertice destino
			int dest = rand() % count;

			// ignora se o destino eh o mesmo nodo, ou anterior
			if (dest <= i)
				continue;

			char Result[16];
			sprintf ( Result, "%d", dest );
			string s = string(Result);

			// se já é vizinho, continua
			int ind = nodos[i].procuraVizinho (s);
			if (ind >=0)
				continue;

			// custo fixo 1
			//int custo = 1;
			int custo = rand() %10;

			Vertice v (s, custo);
			nodos[i].addVizinho(v);

			// adiciona vizinho para tras
			ind = procura(s);
			Vertice b (nodos[i].getId(), custo);
			nodos[ind].addVizinho(b);

			cout << "destino: " << s << endl;
		}
	}

	// read tem q ser antes de send pois send cria o pipe
	for (int i = 0; i < count; i++)
		nodos[i].startReadThread();

	for (int i = 0; i < count; i++)
		nodos[i].startSendThread();

	for (int i = 0; i < count; i++)
		nodos[i].startCheckThread();

}

bool Grafo::delNodo(string id) {

	int ind = procura(id);
	if (ind >=0)
	{
		nodos.erase(nodos.begin()+ind);
		return true;
	}
	else
		return false;
}

// retorna custo de vertice, independente de direcao
int Grafo::getCusto(string a, string b) {
	int result = Vertice::INFINITO;
	int ind = procura(a);
	if (ind >= 0)
	{
		result = nodos[ind].getCustoVizinho(b);
		if (result == Vertice::INFINITO)
		{
			ind = procura(b);
			if (ind >= 0)
				result = nodos[ind].getCustoVizinho(a);
			else
				result = Vertice::INFINITO;
		}
	}

	return result;
}

const std::vector<Vertice>& Grafo::getVizinhos(string id) {
	int ind = procura (id);
	if (ind >= 0)
	{
		return nodos[ind].getVizinhos();
	}

//	return NULL;
}

string Grafo::getRotas(string nodo) {
	int ind = procura (nodo);
	if (ind >= 0)
	{
		return nodos[ind].getTabela(nodo);
	}

	return "";
}

string Grafo::calculaRotas() {
	ofstream myfile;
	  myfile.open ("/tmp/calc.txt");
	std::ostringstream out;
	int size = nodos.size();
	for (int i=0; i< size;i++)
	{
		for (int j = i+1; j < size ; j++)
		{
			std::ostringstream out1;
			out1 << caminhomaiscurto (nodos[i].getId(), nodos[j].getId());
			out << out1.str() << "----" << endl;
			cout << out1.str();
		}
	}

	cout << out.str();
	myfile << out.str();
	 myfile.close();
	return out.str();
}

void Grafo::pause() {



}
