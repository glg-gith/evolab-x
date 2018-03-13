/*
 * Singleton.h
 *
 *  Created on: 17 juil. 2012
 *      Author: golgauth
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_


namespace elp {

template <typename T>
class Singleton
{
protected:
	// Constructeur/destructeur
	Singleton () { }
	~Singleton () { /*std::cout << "destroying singleton." << std::endl;*/ }

public:
	// Interface publique
	static T *GetInstance ()
	{
		if (NULL == _singleton)
		{
			//std::cout << "creating singleton." << std::endl;
			_singleton = new T;
		}
		else
		{
			//std::cout << "singleton already created!" << std::endl;
		}

		return (static_cast<T*> (_singleton));
	}

	static void Kill ()
	{
		if (NULL != _singleton)
		{
			delete _singleton;
			_singleton = NULL;
		}
	}

private:
	// Unique instance
	static T *_singleton;
};

template <typename T>
T *Singleton<T>::_singleton = NULL;

} /* namespace elp */
#endif /* SINGLETON_H_ */
