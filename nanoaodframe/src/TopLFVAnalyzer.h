/*
 * TopLFVAnalyzer.h
 *
 *  Created on: April 9, 2020
 *      Author: Tae Jeong Kim 
 */

#ifndef TopLFVAnalyzer_H_
#define TopLFVAnalyzer_H_

#include "NanoAODAnalyzerrdframe.h"

class TopLFVAnalyzer: public NanoAODAnalyzerrdframe
{
	public:
		TopLFVAnalyzer(TTree *t, std::string outfilename, std::string year="", std::string syst="", std::string jsonfname="", string globaltag="", int nthreads=1);
		void defineCuts();
		void defineMoreVars(); // define higher-level variables from
		void bookHists();
        private:
                std::string _year;
                std::string _syst;
                //bool isSTLFVcat = false;
                //bool isTTLFVcat = false;

};



#endif /* TopLFVAnalyzer_H_ */