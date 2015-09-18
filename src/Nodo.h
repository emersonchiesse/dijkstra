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
#include "Tabela.h"

using namespace std;

vector<string> tokenizeString(const string& str, const string& delimiters);

typedef enum {
	THREAD_RUNNING,
	THREAD_STOP,
	THREAD_PAUSE
} threadState;

class Nodo {
private:
	std::string id;
	std::vector<Vertice> vizinhos;
	int x, y;
	pthread_t threadSendHello;
	pthread_t threadReceiveHello;
	pthread_t threadCheck;
	vector<Tabela> tabela;
	threadState sendThreadState;
	threadState receiveThreadState;
	vector<string> MPR;
	int versaoHello;
	void escolheMPR();

public:
	Nodo ();
	Nodo(string i);
	Nodo(string i, int x, int y);
	virtual ~Nodo();

	//thread_states_t estadoThread;
//	static void *sendHello();
	pthread_mutex_t mutex_hello;

	void addVizinho (Vertice v);
	bool isMPR();

	void startSendThread();
	void startReadThread();
	void startCheckThread();
	void threadPause();
	void threadRun();
	void threadStop();

	void updateTabela(string);
	int tabelaExiste(string n); // retorna versao da tabela
	string getTabela (string n);
	bool tabelaApaga(string n);

	void killAll();
	void checkAlives();
	const std::string& getId() const {
		return id;
	}

	void setId(const std::string& id) {
		this->id = id;
	}

	 std::vector<Vertice>& getVizinhos()  {
		return vizinhos;
	}

	string getVizinhosStr();

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

	threadState getReceiveThreadState() const {
		return receiveThreadState;
	}

	void setReceiveThreadState(threadState receiveThreadState) {
		this->receiveThreadState = receiveThreadState;
	}

	threadState getSendThreadState() const {
		return sendThreadState;
	}

	void setSendThreadState(threadState sendThreadState) {
		this->sendThreadState = sendThreadState;
	}
};

#endif /* NODO_H_ */
