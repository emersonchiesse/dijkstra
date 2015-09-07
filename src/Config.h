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

public:
	Config();

	//virtual ~Config();

	bool isMostraPesos() const {
		return mostraPesos;
	}

	void setMostraPesos(bool mostraPesos) {
		this->mostraPesos = mostraPesos;
	}

};

} /* namespace config */

#endif /* CONFIG_H_ */
