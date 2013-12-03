#pragma once

#ifndef _ANALYSISMODULE_H
#define _ANALYSISMODULE_H
#endif

#include "stdafx.h"
#include "PowerSystemModel.h"
#include "pw_common.h"
#include "pw_flow.h"

class AnalysisModule
{
public:
	AnalysisModule(double step = 0.1, double time = 1.0);
	bool readDataFile(string path);
	void printBuses() const;
	void simulate(int n = 1) const;
	SimplePowerNet * readySimulate() const;
	void simulateStep(SimplePowerNet *sn, double * del, double * th) const;
	void endSimulate(SimplePowerNet *sn) const;
	virtual void print() const;
	~AnalysisModule();
	Branch* cuteLine(int id);
	void recloseLine(Branch* bra);
	bool busShortCircuit(SimplePowerNet *sn, int id) const;
	int getGenNum() const;
private:
	Bus ** buses;
	int bun;
	Branch ** branches;
	int bran;
	double time;
	double step;
};

