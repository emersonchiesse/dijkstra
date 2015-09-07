/*
 * Nodo.h
 *
 *  Created on: Jul 21, 2015
 *      Author: x
 */

#ifndef NODO_H_
#define NODO_H_

#include <vector>
#include <string>
#include <pthread.h>

#include "Vertice.h"

using namespace std;

//#define NUM_THREADS 4

class Nodo {
private:
	std::string id;
	std::vector<Vertice> vizinhos;
	int x, y;
	pthread_t threadSendHello;
	pthread_t threadReceiveHello;

public:
	Nodo(string i);
	Nodo(string i, int x, int y);
	virtual ~Nodo();

//	static void *sendHello();

	void addVizinho (Vertice v);
	void startThreads();

	const std::string& getId() const {
		return id;
	}

	void setId(const std::string& id) {
		this->id = id;
	}

	 std::vector<Vertice>& getVizinhos()  {
		return vizinhos;
	}

	const std::vector<Vertice>* getVizinhos2() const {
		return &vizinhos;
	}

	void setVizinhos(const std::vector<Vertice>& vizinhos) {
		this->vizinhos = vizinhos;
	}

	int getCustoVizinho(string id);
	int procuraVizinho (string id);

	int getX() const {
		return x;
	}
	int getY() const {
			return y;
	}

	void setCoord(int x, int y) {
		this->x = x;
		this->y = y;
	}

	void sendHELLO();

};

#endif /* NODO_H_ */
