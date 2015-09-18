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

#define SLEEP_RECEIVE 5
#define SLEEP_SEND	10

void *sendHello(void *arg) {
	Nodo* nodo = ((Nodo*)arg);
	string id = nodo->getId();
	string namedpipe = "/tmp/pipe" + id;
	string msg;
	threadState s = THREAD_RUNNING;
//	while (nodo->getSendThreadState() != THREAD_STOP)
	while (true)
	{
		switch (s) //(nodo->getSendThreadState())
		{
		case THREAD_RUNNING:
		{
			//std::ostringstream s;
			pthread_mutex_lock(&nodo->mutex_hello);
			vector<Vertice> *vizinhos = &nodo->getVizinhos();
			//s << "sending hello, #vizinhos: " << vizinhos->size();
			//log (id, s.str());

			msg = nodo->getVizinhosStr();
			//TODO send MPR

			vector<Vertice>::const_iterator i;
			for (i = vizinhos->begin(); i != vizinhos->end(); i++)
			{
				//log (id, (*i).getId());
				string namedpipe = "/tmp/pipe" + (*i).getId();

				int myPipe = open(namedpipe.c_str(), O_RDWR);
				if (myPipe >= 0 )
				{
					//flush(myPipe);
					write(myPipe, msg.c_str(), msg.size());
					close(myPipe);
					log (id, "sent 'hello' to " + (*i).getId() + " msg: " + msg);
				}
				else
					log (id, "open pipe failed");
			}
			pthread_mutex_unlock(&nodo->mutex_hello);

			sleep(SLEEP_SEND);
		}
			break;
		case THREAD_PAUSE:
			sleep (1);
			break;
		default:
			break;
		}
	}
	return 0;
}

extern "C"
{
void *receiveHello(void *arg)
{
	Nodo* nodo = static_cast<Nodo*>(arg);
	string id = nodo->getId();

	string namedpipe = "/tmp/pipe" + id;
	int ret = unlink(namedpipe.c_str());
	if (ret < 0)
		log(id, "unlink com erro " + namedpipe);

	 ret = mknod(namedpipe.c_str(), S_IFIFO | S_IRUSR | S_IWUSR, 0);
	 if (ret < 0)
	 		log(id, "mknod com erro");

	 int myPipe = open(namedpipe.c_str(), O_RDONLY);
	 if (myPipe < 0)
	 {
		 log(id, "error opening pipe " + namedpipe);
		 return 0;
	 }


	char buffer[500];

	bool sai = false;
	while (!sai)
	{
		log(id, "checking hello ");

		//cout << nodo->mutex_hello << endl;
		//pthread_mutex_lock(nodo->mutex_hello);
		if (myPipe >= 0)
		{
			int length = read(myPipe, buffer, 500);
			buffer[length] = '\0';
			log (id, "received: " +  string(buffer));
			pthread_mutex_lock(&nodo->mutex_hello);
			nodo->updateTabela(string(buffer));
			pthread_mutex_unlock(&nodo->mutex_hello);
		}

		sleep(SLEEP_RECEIVE);
	}
	close(myPipe);
	unlink(namedpipe.c_str());

	return 0;
}
}

Nodo::Nodo() {
	x=0;
	y=0;
	threadSendHello=NULL;
	threadReceiveHello=NULL;
	receiveThreadState = THREAD_RUNNING;
	sendThreadState = THREAD_RUNNING;
	versaoHello = 0;
	pthread_mutex_init(&mutex_hello, NULL);
}

Nodo::Nodo(string i) {
	x=0;
	y=0;
	id =i;
//	Nodo();
	threadSendHello=NULL;
	threadReceiveHello=NULL;
	receiveThreadState = THREAD_RUNNING;
	sendThreadState = THREAD_RUNNING;
	pthread_mutex_init(&mutex_hello, NULL);
	versaoHello = 0;
}

Nodo::Nodo(string i, int x, int y) {
//	Nodo();
	id=i;
	this->x=x;
	this->y=y;
	versaoHello = 0;
	pthread_mutex_init(&mutex_hello, NULL);
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

void Nodo::startReadThread() {
	int rc;
	log(id, "starting receive thread");
	rc = pthread_create(&threadReceiveHello, NULL,
			receiveHello,
			(void *)this);
	if (rc){
	         cout << "Error:unable to create receive thread," << rc << endl;
	}
}

void Nodo::startSendThread() {
		int rc;
	log(id, "starting send thread");
	rc = pthread_create(&threadSendHello, NULL,
			sendHello,
			(void *)this);
	if (rc){
	         cout << "Error:unable to create send thread," << rc << endl;
	}

}

string Nodo::getVizinhosStr() {

	// formato do hello: id'v'n | v1 | v2 | ... | vn,versao
	std::ostringstream s;
	s << getId() << "v" << vizinhos.size();
	vector<Vertice>::const_iterator v;
	for (v = vizinhos.begin(); v != vizinhos.end(); v++)
	{
		s << "|" << (*v).getId();
	}
	s << ',' << versaoHello << ".";

	return s.str();
}

void Nodo::updateTabela(string msg) {

	// atualiza a vizinhanca do vizinho
	// se o vizinho nodo nao esta na tabela, insere
	// senao atualiza

	if (msg.size()==0)
		return;

	string nodo = "";
	vector<string> vizinhos;

	// formato msg: nv#|n1|...|nz,versao.


	// se nao tem . nem 'v', eh mensagem mal formada
	if ((msg.find('v')==string::npos) ||
		(msg.find('.')==string::npos)
		// || (msg.find('.')!=msg.length()-1)
		)
		{
			log(id, "mensagem truncada");
			return;
		}

	// extrai todas as mensagens (pode chegar mais de uma mensagem hello)
	vector<string> m = tokenizeString (msg, ".");
	vector<string>::iterator i;
	for (i = m.begin(); i!= m.end(); i++)
	{
		// separa nodo
		vector<string> v1 = tokenizeString ((*i), "v");
		nodo = v1[0];

		// vizinhos
		log(nodo, "vizinhos: " + v1[1]);

		// retira versao
		int versaomsg = 0;
		vector<string> w = tokenizeString (v1[1], ",");
		versaomsg = std::atoi (w[1].c_str());
		log(nodo, "versao hello: " + w[1]);

		vector<string> v = tokenizeString (w[0], "|");

		// v[0] = qtde de vizinhos
		int count = std::atoi (v[0].c_str());

		// se nao tem '|', msg truncada
		if (v.size() <= count)
		{
			log(id, "mensagem truncada");
			return;
		}

		//v = tokenizeString (msg, "|");
		if (v.size() == 0)
		{
			log(id, "mensagem vazia.");
			return;
		}



		// extrai vizinhos
		int versao = tabelaExiste(nodo);
		std::ostringstream s;
		s << "versao existente: " << versao;
		log(id, s.str());
		if (versao == versaomsg)
		{
			log(id, "versao de tabela igual. sem atualizacao");
			return;
		}

		// atualiza tabela

		if (versao >= 0)
		{
			log(nodo, "ja existe, apagando...");
			tabelaApaga (nodo);
		}

		// TODO: pode ter ficado sem. tem q apagar se tiver vizinhos
		if (v.size() == 1)
		{
			log(id, "sem vizinhos (msg: " + v[1] + ")");
			//return;
		}

		log(nodo, "inserindo nova rota...");
		Tabela *t = new Tabela (nodo, versaomsg, vizinhos);
		// -1 para ignorar o "."
		// [0] eh o proprio nodo e #vizinhos
		//
		//t->setVersao(0);
		for (int i = 1; i <= count; i++)
		{
			// ignora o proprio nodo
			if (v[i] == id)
				continue;
			t->addVizinho(v[i]);
			log (nodo, "update vizinho: " + v[i]);
		}
		tabela.push_back(*t);
	}

	escolheMPR();
}

int Nodo::tabelaExiste(string n)
{
	vector<Tabela>::const_iterator t;
	for (t = tabela.begin(); t != tabela.end(); t++)
	{
		if ((*t).getId() == n)
		{
			return (*t).getVersao();
		}
	}
	return -1;
}

bool Nodo::tabelaApaga(string n)
{
	bool result = false;
	vector<Tabela>::iterator t;
	for (t = tabela.begin(); t != tabela.end(); t++)
	{
		if ((*t).getId() == n)
		{
			tabela.erase(t);
			return true;
		}
	}
	return result;
}

string Nodo::getTabela (string n)
{
	std::ostringstream s;
	s << "Nodo: " << getId() << endl;
	//s << getVizinhosStr() << endl;
	s << "\n#vizinhos: " << vizinhos.size() << endl;

//	vector<Vertice>::const_iterator v;
//	for (v = vizinhos.begin(); v != vizinhos.end(); v++)
//	{
//		s << "\n" << (*v).getId();
//	}

	vector<Tabela>::const_iterator t;
	for (t = tabela.begin(); t != tabela.end(); t++)
	{
		s << (*t).getId() << endl;

		const vector<string>* vizinhos2 = &(*t).getVizinhos();
		vector<string>::const_iterator v2;
		for (v2 = vizinhos2->begin();
				v2 != vizinhos2->end(); v2++)
		{
			s << "  " << (*v2) << endl;
		}
	}

	return s.str();


}

void Nodo::escolheMPR() {

	log (id, "define MPR");
	// avalia todos os vizinhos
	// verifica qual vizinho tem mais vizinhos em comum

	int max = 0;
	int i = 0;
	string mpr;

	vector<Tabela>::const_iterator t;
	for (t = tabela.begin(); t != tabela.end(); t++)
	{
		const vector<string>* vizinhos2 = &(*t).getVizinhos();
		vector<string>::const_iterator v2;
		int nvizinhos = vizinhos2->size();
		if (nvizinhos == 0)
		{
			MPR.push_back((*t).getId());
			log (id, "novo MPR: " + (*t).getId());
		}
//		else
//		{
//			if (nvizinhos > max)
//			{
//				max = nvizinhos;
//				mpr = (*t).getId();
//			}
//		}
		i++;
	}
//	if (max > 0)
//	{
//		MPR.push_back(mpr);
//		log (id, "novo MPR: " + mpr);
//	}

}

void Nodo::threadPause() {
	sendThreadState = THREAD_PAUSE;
	receiveThreadState = THREAD_PAUSE;
}

void Nodo::threadRun() {
	sendThreadState = THREAD_RUNNING;
	receiveThreadState = THREAD_RUNNING;
}

void Nodo::threadStop() {
	sendThreadState = THREAD_STOP;
	receiveThreadState = THREAD_STOP;
}

bool Nodo::isMPR() {
	return MPR.size() > 0;
}
