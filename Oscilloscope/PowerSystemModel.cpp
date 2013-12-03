#include "stdafx.h"
#include "PowerSystemModel.h"

//class BasicModel
void BasicModel::setModelType(ModelType mt)
{
	modelType = mt;
}

BasicModel::ModelType BasicModel::getModelType()
{
	return modelType;
}
//class BasicModel end
 
//class Bus
Bus::Bus(int _i, string _Name, NodeType _Type, double _V, double _cita, double _GP, double _GQ, double _LP, double _LQ, double _Qmax, double _Qmin, double _g, double _b, double _Vmax, double _Vmin)
{ 
	i = _i;
	name = _Name;
	type = _Type;
	V = _V;
	cita = _cita;
	GP = _GP;
	GQ = _GQ;
	LP = _LP;
	LQ = _LQ;
	Qmax = _Qmax;
	Qmin = _Qmin;
	b = _b;
	g = _g;
	Vmax = _Vmax;
	Vmin = _Vmin;
	setModelType(BasicModel::NORMALBUS);
}

void Bus::setName(string name)
{
	this->name = name;
}

void Bus::initial()
{
}

double Bus::getQmax() const
{
	return Qmax;
}

double Bus::getQmin() const
{
	return Qmin;
}

double Bus::getG() const
{
	return g;
}

double Bus::getB() const
{
	return b;
}

double Bus::getVmax() const
{
	return Vmax;
}

double Bus::getVmin() const
{
	return Vmin;
}

Complex Bus::getCU()const
{
	return cU;
}

Complex Bus::getCI() const
{
	return cI;
}
double Bus::getV() const
{
	return V;
}

void Bus::setCU(Complex u)
{
	cU = u;
	V = abs(cU);
	cita = (log(cU / V) * Complex(0, -1)).real();
}

void Bus::calCI()
{
	cI = 0;
}

Bus * Bus::getBusById(Bus ** buses, int num, int id)
{
	for  (int i = 0; i < num; i++)
	{
		if (buses[i]->i == id)
			return buses[i];
	}
	return NULL;
}

void Bus::print()
{
	cout << "Name= " << name << " ModelType=" << (int)getModelType() << " NodeType=" << type << " V=" << V << " cita=" << cita << endl;
}
//class Bus end

//class Branch
Branch::Branch(int _i, int _j, string _Name, BranchType _Type, double _R, double _X, double _B, double _K)
{
	i = _i;
	j = _j;
	name = _Name;
	type = _Type;
	R = _R;
	X = _X;
	B = _B;
	K = _K;
	setModelType(BasicModel::BRANCH);
}

double Branch::getR() const
{
	return R;
}

double Branch::getX() const
{
	return X;
}

double Branch::getB() const
{
	return B;
}

Branch::BranchType Branch::getType() const
{
	return type;
}

double Branch::getK() const
{
	return K;
}

int Branch::getI() const
{
	return i;
}

int Branch::getJ() const
{
	return j;
}

string Branch::getName() const
{
	return name;
}
//class Branch end

//class SimpleGenerator
SimpleGenerator::SimpleGenerator(double Xd1, double Tj)
{
	this->cUg = cUg;
	this->Xd1 = Xd1;
	this->Tj = Tj;
	step = 0.1;
	w = 1.0;
	setModelType(BasicModel::GENERATOR);
}

void SimpleGenerator::bindBus(Bus *bus)
{
	i = bus->i;
	name = bus->name;
	type = bus->type;
	V = bus->V;
	cita = bus->cita;
	GP = bus->GP;
	GQ = bus->GQ;
	LP = bus->LP;
	LQ = bus->LQ;
	Qmax = bus->Qmax;
	Qmin = bus->Qmin;
	b = bus->b;
	g = bus->g;
	Vmax = bus->Vmax;
	Vmin = bus->Vmin;
	cU = cUg = polar(V, cita);
}

void SimpleGenerator::setStep(double h)
{
	step = h;
}

void SimpleGenerator::initial()
{
	cU = cUg = polar(V, cita);
	calCIg0();
	calE1();
	calDelta();
	calPm();	
	calCYg();
	calPe();
}

void SimpleGenerator::nextStep(int n)
{
	double w0 = w;
	double delta0 = delta;
	w += step * n * (Pm - Pe) / Tj;
	delta += ((w0 + w) / 2.0 - 1) * 314 * step * n;
	calCE1();
	calCIg1();	
	cUg = cU;
	calCIg();
	calPe();
	calCI();
}

void SimpleGenerator::endStep(Complex U)
{
	setCU(U);
}

void SimpleGenerator::calCIg0()
{
	cIg = conj(Complex(GP, GQ) / cUg);
}

void SimpleGenerator::calCIg()
{
	cIg = cIg1 - cYg * cUg;
}

void SimpleGenerator::calPm()
{
	Pm = (cUg * conj(cIg)).real();
}

void SimpleGenerator::calPe()
{
	Pe = (cUg * conj(cIg)).real();
	//cout << Pe << ' ' << cUg << ' ' << cIg << endl;
}

void SimpleGenerator::calE1()
{
	E1 = abs(cUg + Complex(0, 1) * Xd1 * cIg);
}

void SimpleGenerator::calCE1()
{
	cE1 = polar(E1, delta);
}

void SimpleGenerator::calDelta()
{
	delta = (log((cUg + Complex(0, 1) * Xd1 * cIg) / E1) * Complex(0, -1)).real();
}

void SimpleGenerator::calCYg()
{
	cYg = 1 / Xd1 * Complex(0, -1);
}

void SimpleGenerator::calCIg1()
{
	cIg1 = cYg * cE1;
}

void SimpleGenerator::calCI()
{
	cI = cIg1;
}

Complex SimpleGenerator::getCYg() const
{
	return cYg;
}

double SimpleGenerator::getDelta() const
{
	return delta;
}

void SimpleGenerator::print()
{
	cout << "Name= " << name << " ModelType=" << (int)getModelType() << " delta=" << delta / 3.14159 * 180 << " NodeType=" << type << " V=" << V << " cita=" << cita << endl;
}
#ifdef DEBUG
	void SimpleGenerator::logPrint()
	{
		calCIg0();
		calE1();
		calDelta();
		//cout << cUg + Complex(0, 1) * Xd1 * cIg << endl;
		//cout << (log(cUg + Complex(0, 1) * Xd1 * cIg) / E1 * Complex(0, -1)).real() << endl;
		cout << "Ig=" << cIg << endl;
		cout << "E1=" << E1 << endl;
		cout << "delta=" << delta << endl;
	}
#endif
//class SimpleGenerator end

//class SimpleLoad

SimpleLoad::SimpleLoad()
{
	setModelType(BasicModel::LOAD);
}

void SimpleLoad::bindBus(Bus *bus)
{
	i = bus->i;
	name = bus->name;
	type = bus->type;
	V = bus->V;
	cita = bus->cita;
	GP = bus->GP;
	GQ = bus->GQ;
	LP = bus->LP;
	LQ = bus->LQ;
	Qmax = bus->Qmax;
	Qmin = bus->Qmin;
	b = bus->b;
	g = bus->g;
	Vmax = bus->Vmax;
	Vmin = bus->Vmin;
	cU = cUl = polar(V, cita);
}

void SimpleLoad::initial()
{
	cU = cUl = polar(V, cita);
	calCYl();
}

Complex SimpleLoad::getCYl() const
{
	return cYl;
}

void SimpleLoad::calCYl()
{
	cYl = (LP - Complex(0, LQ)) / pow(abs(cUl), 2.0);
}
//class SimpleLoad end

//class SimplePowerNet
SimplePowerNet::SimplePowerNet(Bus **buses, Branch **branches, int dim, int bran)
{
	this->buses = buses;
	this->branches = branches;
	this->dim = dim;
	this->bran = bran;	
	cY = NULL;
}

void SimplePowerNet::calcY()
{
	freecY();

	cY = new Complex*[dim];
	for (int i = 0; i < dim; i++)
	{
		cY[i] = new Complex[dim];
		for (int j = 0; j < dim; j++)
		{
			cY[i][j] = 0;
		}
	}

	for (int i = 0; i < bran; i++)
	{
		if (branches[i]->getType() == Branch::LINE)
		{
			Complex y = Complex(1) / Complex(branches[i]->getR(), branches[i]->getX());
			cY[branches[i]->getI()][branches[i]->getJ()] = cY[branches[i]->getJ()][branches[i]->getI()] = -y;
			cY[branches[i]->getI()][branches[i]->getI()] += y + Complex(0, branches[i]->getB() / 2);
			cY[branches[i]->getJ()][branches[i]->getJ()] += y + Complex(0, branches[i]->getB() / 2);
		} else {
			//no k != 1
		}
	}

	for (int i = 0; i < dim; i++)
	{
		Bus * bus = buses[i];
		if (bus != NULL)
		{
			if (bus->getModelType() == BasicModel::GENERATOR)
			{
				cY[bus->i][bus->i] += ((SimpleGenerator*)bus)->getCYg();
			}
			if (bus->getModelType() == BasicModel::LOAD)
			{
				cY[bus->i][bus->i] += ((SimpleLoad*)bus)->getCYl();
			}
			cY[bus->i][bus->i] += Complex(bus->g, bus->b);
		}
	}
	Matrix<Complex>(cY, dim, dim).ToFile();
}

void SimplePowerNet::freecY()
{
	if (cY != NULL)
	{
		for (int i = 0; i < dim; i++)
		{
			delete [] cY[i];
		}
		delete [] cY;
	}
}

Complex*  SimplePowerNet::NodeVoltageMethod() const
{
	Complex * I = new Complex[dim];
	Complex * U = new Complex[dim];

	for (int i = 0; i < dim; i++)
	{		
		I[i] = buses[i]->getCI();		
	}
	Matrix<Complex> mU = Matrix<Complex>(cY, dim, dim).Inverse() * Matrix<Complex>(I, dim, 1);

	for (int i = 0; i < mU.GetRow(); i++)
	{
		U[i] = mU.At(i, 0);
	}
	delete [] I;
	return U;

}

SimplePowerNet::~SimplePowerNet()
{
	freecY();
}

bool SimplePowerNet::setcYById(int id, Complex value) const
{
	if (id < 0 || id >= dim)
	{
		return false;
	}

	cY[id][id] = value;
	return true;
}
//class SimplePowerNet end