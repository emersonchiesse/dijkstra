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
	vector<string> vizinhos;
public:
	Tabela() {
		id = "";
	}
	Tabela(string i): id(i) {};
	Tabela(string i, vector<string> v): id(i)
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
};

#endif /* TABELA_H_ */
