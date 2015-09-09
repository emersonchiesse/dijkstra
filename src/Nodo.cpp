/*
 * Nodo.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: x
 */

#include "Nodo.h"
#include <iostream>
#include <sstream>

#include <unistd.h>         // std::this_thread::sleep_for
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstdlib>

using namespace std;


void log (string id, const string msg)
{
	cout << "[nodo " << id << "] " <<  msg << endl;
}

Nodo::Nodo(string i) {
	id =i;
	x=0;
	y=0;
}

void *sendHello(void *arg)
{
	Nodo* nodo = ((Nodo*)arg);
	string id = nodo->getId();

	string namedpipe = "/tmp/pipe" + id;

	string msg;

	bool sai = false;
	while (!sai)
	{
//		Nodo* nodo2 = ((Nodo*)arg);
		//nodo2->sendHELLO();
		vector<Vertice> *vizinhos = &nodo->getVizinhos();
		std::ostringstream s;
		s << "sending hello, #vizinhos: " << vizinhos->size();
		log (id, s.str());

		vector<Vertice>::const_iterator i;
		for (i = vizinhos->begin(); i != vizinhos->end(); i++)
		{
			//log (id, (*i).getId());
			string namedpipe = "/tmp/pipe" + (*i).getId();

			msg = nodo->getVizinhosStr();
			pthread_mutex_lock(nodo->mutex_hello);
			int myPipe = open(namedpipe.c_str(), O_WRONLY);
			if (myPipe >= 0 )
			{
				write(myPipe, msg.c_str(), msg.size());
				close(myPipe);
				log (id, "sent 'hello' to " + (*i).getId() + " msg: " + msg);
			}
			else
				log (id, "open pipe failed");
			pthread_mutex_unlock(nodo->mutex_hello);
		}


		sleep(5);
	}
	return 0;
}

extern "C"
{
void *receiveHello(void *arg)
{
	Nodo* nodo = static_cast<Nodo*>(arg);
	string id = nodo->getId();
	bool sai = false;

	string namedpipe = "/tmp/pipe" + id;
	unlink(namedpipe.c_str());
	 mknod(namedpipe.c_str(), S_IFIFO | S_IRUSR | S_IWUSR, 0);
	 int myPipe = open(namedpipe.c_str(), O_RDONLY);

	while (!sai)
	{
		log(id, "checking hello ");

		//cout << nodo->mutex_hello << endl;
		//pthread_mutex_lock(nodo->mutex_hello);
		if (myPipe >= 0)
		{
			char buffer[21];
			int length = read(myPipe, buffer, 20);
			buffer[length] = '\0';
			log (id, "received: " +  string(buffer));
			nodo->updateTabela(string(buffer));
		}
		//pthread_mutex_unlock(nodo->mutex_hello);

		sleep(10);
	}
	close(myPipe);
	unlink(namedpipe.c_str());

	return 0;
}
}

Nodo::Nodo(string i, int x, int y) {
	id=i;
	this->x=x;
	this->y=y;
}

Nodo::~Nodo() {
	//pthread_exit(NULL);
}



void Nodo::addVizinho(Vertice v) {
	vizinhos.push_back(v);
}

int Nodo::getCustoVizinho(string id) {

	// se eh vizinho, tem custo
	// senao eh infinito

	// se eh ele mesmo, tem custo 0;
	if (id == this->getId())
		return 0;

	// se nao eh vizinho imediato, tem custo infinito
	int ind = procuraVizinho (id);
	if (ind >= 0)
	{
		return vizinhos[ind].getCusto();
	}
	else
		return Vertice::INFINITO;
}

int Nodo::procuraVizinho(string id) {
	for (int i = 0; i < vizinhos.size(); i++)
	{
		//if (vizinhos[i].getVizinho()->getId() == id)
		if (vizinhos[i].getId() == id)
			return i;
	}

	return -1;
}

void Nodo::sendHELLO() {
	cout << "nodo " << getId () << " " << "vizinhos.size: " << vizinhos.size() << endl;
	for (int i = 0; i < vizinhos.size(); i++)
	{
		cout << "sending hello to " << vizinhos[i].getId() << endl;
	}

}

void Nodo::startThreads() {
	int rc;
	rc = pthread_create(&threadReceiveHello, NULL,
			receiveHello,
			(void *)this);
	if (rc){
	         cout << "Error:unable to create receive thread," << rc << endl;
	}

	rc = pthread_create(&threadSendHello, NULL,
			sendHello,
			(void *)this);
	if (rc){
	         cout << "Error:unable to create send thread," << rc << endl;
	}

}

string Nodo::getVizinhosStr() {
	std::ostringstream s;
	s << getId() << "|" << vizinhos.size();
	vector<Vertice>::const_iterator v;
	for (v = vizinhos.begin(); v != vizinhos.end(); v++)
	{
		s << "|" << (*v).getId();
	}
	s << "|";

	return s.str();
}

void Nodo::updateTabela(string msg) {

	// atualiza a vizinhanca do vizinho
	// se o vizinho nodo nao esta na tabela, insere
	// senao atualiza
	string nodo = "";
	vector<string> vizinhos;
	int count;

	// extrai nodo
	 vector<string> v = tokenizeString (msg, "|");
	 if (v.size() == 0)
	 {
		 cout << "mensagem vazia." << endl;
		 return;
	 }

	 if (v.size() == 1)
	 {
		 cout << "sem vizinhos." << endl;
		 return;
	 }

	 nodo = v[0];
	 count = std::atoi (v[1].c_str());

	 log(nodo, "vizinhos: " + v[1]);
	 for (int i = 2; i < count; i++)
	 {
		 vizinhos.push_back(v[i]);
		 log (nodo, v[i]);
	 }

	// extrai vizinhos

	bool existe = tabelaExiste(nodo);
	if (!existe)
	{
		log(nodo, "inserindo nova rota...");
		Tabela *t = new Tabela (nodo, vizinhos);
		tabela.push_back(*t);
	}
	else
	{
		//vector<Tabela>
	}
}
bool Nodo::tabelaExiste(string n)
{
	bool result = false;
	vector<Tabela>::const_iterator t;
	for (t = tabela.begin(); t != tabela.end(); t++)
	{
		if ((*t).getId() == n)
			return true;
	}
	return result;
}
