/*
 * Tabela.h
 *
 *  Created on: 8 de set de 2015
 *      Author: x
 */

#ifndef TABELA_H_
#define TABELA_H_

#include <string>
#include <vector>

using namespace std;

class Tabela {
private:
	string id;
	int versao;
	vector<string> vizinhos;
	bool alive;

public:
	Tabela() {
		id = "";
		versao = 0;
		alive = true;
	}

	Tabela(string i): id(i), versao(0), alive(true) {};
	Tabela(string i, int ver, vector<string> v):
		id(i), versao(ver), alive(true)
	{
		vizinhos = v;
	};
	virtual ~Tabela();

	bool existe (string);
	void addVizinho (string);

	const string& getId() const {
		return id;
	}

	void setId(const string& id) {
		this->id = id;
	}

	const vector<string>& getVizinhos() const {
		return vizinhos;
	}

	void setVizinhos(const vector<string>& vizinhos) {
		this->vizinhos = vizinhos;
	}

	int getVersao() const {
		return versao;
	}

	void setVersao(int versao) {
		this->versao = versao;
	}

	bool isAlive() const {
		return alive;
	}

	void setAlive(bool alive) {
		this->alive = alive;
	}
};

#endif /* TABELA_H_ */
