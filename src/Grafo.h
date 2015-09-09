/*
 * Grafo.h
 *
 *  Created on: Jul 22, 2015
 *      Author: x
 */

#ifndef GRAFO_H_
#define GRAFO_H_

#include "Nodo.h"

#include <string>
#include <pthread.h>

using namespace std;

#define NUM_THREADS 50

class Grafo {
private:
	std::vector<Nodo> nodos;
	int raio;
	pthread_t threads[NUM_THREADS];
	std::vector<pthread_mutex_t> mutex;

public:
	Grafo();
	virtual ~Grafo();

	void init();
	void load();
	void load(string arq);
	void loadcoords(string arq);
	void list();
	int procura(string id);
	int procura(int x, int y);
	void addNodo(Nodo *n);
	void delNodo(string id);
	int getCusto (string a, string b);
	const std::vector<Vertice>& getVizinhos (string id);

	void criaRandom (int count, int raio);
	void caminhomaiscurto (string n1, string n2);

	std::vector<Nodo>& getNodos() {
		return nodos;
	}
};

#endif /* GRAFO_H_ */
