/*
 * LabSingleton.h
 *
 *  Created on: Feb 13, 2013
 *      Author: golgauth
 */

#ifndef LABSINGLETON_H_
#define LABSINGLETON_H_

namespace elps {

class LabSingleton
{
    public:
        static LabSingleton& GetInstance()
        {
            static LabSingleton instance; 	      // Guaranteed to be destroyed. Instantiated on first use.
            return instance;
        }
    private:									  // Make it uninstantiable
        LabSingleton();
        LabSingleton(LabSingleton const&);        // Don't Implement
        void operator=(LabSingleton const&);      // Don't implement
};
} /* namespace elps */
#endif /* LABSINGLETON_H_ */

