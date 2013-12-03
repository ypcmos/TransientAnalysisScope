#pragma once

#ifndef _POWERSYSTEMMODEL_H
#define _POWERSYSTEMMODEL_H
#endif

#include "stdafx.h"
#include "matrix.h"


#ifdef Complex
#error Duplicate definition for 'Complex'.
#endif
#define Complex complex<double>

class BasicModel
{
public:
	enum ModelType 
	{
		GENERATOR,
		LOAD,
		NORMALBUS,
		BRANCH,
		NET,		
	};
public:
	virtual ~BasicModel(){}
	virtual void setModelType(ModelType mt);
	virtual ModelType getModelType();

private:
	ModelType modelType;
};

class Bus : public BasicModel
{
public:
	enum NodeType {SLACK = 1, PV, PQ}; 
public:
	Bus()
	{
		setModelType(BasicModel::NORMALBUS);
	}
	Bus(int _i, string _Name, NodeType _Type, double _V, double _cita, double _GP, double _GQ, double _LP, double _LQ, double _Qmax, double _Qmin, double _g, double _b, double _Vmax, double _Vmin);
	void setName(string name);
	virtual Complex getCU()const;
	virtual Complex getCI()const;
	virtual double getV() const;
	double getQmax() const;
	double getQmin() const;
	double getG() const;
	double getB() const;
	double getVmax() const;
	double getVmin() const;
	virtual void print();
	virtual void initial();
	static Bus * getBusById(Bus ** buses, int num, int id);
	virtual void setCU(Complex u);
protected:
	virtual void calCI();

public:
	int i;
	string name;
	NodeType type;
	double V, cita, GP, GQ, LP, LQ, Qmax, Qmin, g, b, Vmax, Vmin;
	Complex cU, cI;
};

class Branch : public BasicModel
{
public:
	enum BranchType {LINE = 1, TRANSFORMER};
public:
	Branch()
	{
		setModelType(BasicModel::BRANCH);
	}	
	Branch(int _i, int _j, string _Name, BranchType _Type, double _R, double _X, double _B, double _K);
	double getR() const;
	double getX() const;
	double getB() const;
	BranchType getType() const;
	double getK() const;
	int getI() const;
	int getJ() const;
	string getName() const;

protected:
	int	i;   
	int j;    
	string name;	
	BranchType type;
	double R;
	double X;  
	double B;  
	double K; 
};

class SimpleGenerator : public Bus
{
public:
	SimpleGenerator(double Xd1, double Tj);
	void bindBus(Bus *bus);
	void setStep(double h);
	void nextStep(int n = 1);
	void endStep(Complex U);
	virtual void print();
	Complex getCYg() const;
	virtual void initial();
	double getDelta() const;

#ifdef DEBUG
	void logPrint();
#endif

private:
	virtual void calCI();
	void calCIg0();
	void calCIg();
	void calCIg1();
	void calPm();
	void calPe();
	void calE1();
	void calCE1();
	void calDelta();
	void calCYg();

private:
	double Pm, Pe;
	double Tj;
	double w, delta;
	double Xd1, E1;
	Complex cE1, cUg, cIg, cYg, cIg1;
	double step;
};

class SimpleLoad : public Bus
{
public:
	SimpleLoad();
	void bindBus(Bus *bus);
	virtual void initial();
	Complex getCYl() const;
	
private:
	void calCYl();
private:
	Complex cYl;
	Complex cUl;
};

class SimplePowerNet : public BasicModel
{
public:
	SimplePowerNet(Bus **buses, Branch **branches, int dim, int bran);
	void calcY();
	bool setcYById(int id, Complex value) const;
	void freecY();
	virtual ~SimplePowerNet();
	Complex* NodeVoltageMethod() const;
private:
private:
	Bus **buses;
	Branch **branches;
	Complex **cY;
	int dim;
	int bran;
};