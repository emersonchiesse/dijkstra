/*
 * Config.h
 *
 *  Created on: 28 de ago de 2015
 *      Author: x
 */

#ifndef CONFIG_H_
#define CONFIG_H_

namespace setup {

class Config {
private:
	bool mostraPesos;
	int raioNodo;
	int raioGrafo;
	// taxa de vertices

public:
	Config();

	//virtual ~Config();

	bool isMostraPesos() const {
		return mostraPesos;
	}

	void setMostraPesos(bool mostraPesos) {
		this->mostraPesos = mostraPesos;
	}

	int getRaioNodo ()
	{ return raioNodo; }

	int getRaioGrafo ()
	{ return raioGrafo; }

};

} /* namespace config */

#endif /* CONFIG_H_ */
