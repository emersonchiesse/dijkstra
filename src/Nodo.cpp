/*
 * Nodo.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: x
 */

#include "Nodo.h"
#include <iostream>

#include <unistd.h>         // std::this_thread::sleep_for


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
	bool sai = false;
	while (!sai)
	{
//		cout << "[nodo " << id << "] sending hello " << endl;
		log (id, "sending hello");
		Nodo* nodo2 = ((Nodo*)arg);
		nodo2->sendHELLO();
//		vector<Vertice> *vizinhos = &nodo->getVizinhos();
//		cout << "x " << nodo->getX() << endl;
//
//		cout << "num vizinhos: " << nodo->getVizinhos2()->size() << endl;
//
//		vector<Vertice>::const_iterator i;
//		for (i = vizinhos->begin(); i != vizinhos->end(); i++)
//		{
//			log (id, (*i).getId());
//		}


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
	while (!sai)
	{
		cout << "[nodo " << id << "] checking hello " << endl;



		sleep(10);
	}
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
	int rc = pthread_create(&threadSendHello, NULL,
			sendHello,
			//(void *)id.c_str());
			(void *)this);
	if (rc){
	         cout << "Error:unable to create thread," << rc << endl;
	}

	rc = pthread_create(&threadReceiveHello, NULL,
			receiveHello,
			//(void *)id.c_str());
			(void *)this);
	if (rc){
	         cout << "Error:unable to create thread," << rc << endl;
	}
}
