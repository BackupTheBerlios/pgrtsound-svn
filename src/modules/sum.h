#ifndef SUM_H
#define SUM_H

#include "module.h"

/**
 * No description
 */
class Sum : public Module {
	public:
		Sum();
		~Sum();
		void Process();
		static Module* Create();
		static string GetTypeStatic();
		string GetType();
		
	private:
		Input i1, i2;
		Output oSum;
};

#endif // SUM_H
