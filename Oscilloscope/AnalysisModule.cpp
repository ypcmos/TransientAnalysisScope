#include "stdafx.h"
#include "AnalysisModule.h"

AnalysisModule::AnalysisModule(double step, double time)
{
	bun = 0;
	bran = 0;
	this->time = time;
	this->step = step;
}

bool AnalysisModule::readDataFile(string path)
{
	fstream fp;

	fp.open(path.c_str(), ios::in);
	if (!fp.is_open())
	{
		return false;
	}
	SimpleGenerator **gBuses = new SimpleGenerator*[144];
	int gBun = 0;
	while (!fp.eof())
	{
		char line[256];
		fp.getline(line, 256);
		string text = ExString::Trim(string(line));
		if (text.find("EM BUS") != -1)
		{
			buses = new Bus*[144];
			while (!fp.eof())
			{
				fp.getline(line, 256);
				text = ExString::Trim(string(line));
				if (text.find("=====") != -1)
				{
					break;
				}
				if (text.length() == 0 || text[0] == '#' || text.find("REM BUS") != -1)
				{
					continue;
				}else{
					string sp[] = {" "};
					StrArray split = ExString::Splite(text, StrArray(sp, sp + 1));
					if (split.size() != 17)
					{
						return false;
					}
					//Bus(int _i, string _Name, NodeType _Type, double _V, double _cita, double _GP,
					//double _GQ, double _LP, double _LQ, double _Qmax, double _Qmin, double _g, double _b, double _Vmax, double _Vmin);
					buses[bun++] = new Bus(atoi(split[1].substr(1, split[1].length()).c_str()) - 1, 
						split[1], Bus::NodeType(atoi(split[10].c_str())), atof(split[2].c_str()), 
						atof(split[3].c_str()), atof(split[4].c_str()), atof(split[5].c_str()), 
						atof(split[6].c_str()), atof(split[7].c_str()), atof(split[11].c_str()), 
						atof(split[12].c_str()), atof(split[8].c_str()), atof(split[9].c_str()), 
						atof(split[13].c_str()), atof(split[14].c_str()));
				}
			}		
		}
		if (text.find("REM --") != -1)
		{
			branches = new Branch*[144];
			while (!fp.eof())
			{
				fp.getline(line, 256);
				text = ExString::Trim(string(line));
				if (text.find("=====") != -1)
				{
					break;
				}
				if (text.length() == 0 || text[0] == '#' || text.find("REM --") != -1)
				{
					continue;
				}else{
					string sp[] = {" "};
					StrArray split = ExString::Splite(text, StrArray(sp, sp + 1));
					if (split.size() != 7)
					{
						return false;
					}
					//Branch(int _i, int _j, string _Name, BranchType _Type, double _R, double _X, double _B, double _K)
					branches[bran++] = new Branch(atoi(split[1].substr(1, split[1].length()).c_str()) - 1, 
						atoi(split[2].substr(1, split[1].length()).c_str()) - 1, split[1], 
						Branch::LINE, atof(split[3].c_str()), atof(split[4].c_str()), 
						atof(split[5].c_str()), atof(split[6].c_str()));
				}
			}
		}

		if (text.find("REM MAC") != -1)
		{
			
			while (!fp.eof())
			{
				fp.getline(line, 256);
				text = ExString::Trim(string(line));
				if (text.find("=====") != -1)
				{
					break;
				}
				if (text.length() == 0 || text[0] == '#' || text.find("REM MAC") != -1)
				{
					continue;
				}else{
					string sp[] = {" "};
					StrArray split = ExString::Splite(text, StrArray(sp, sp + 1));
					if (split.size() != 19)
					{
						return false;
					}
					gBuses[gBun] = new SimpleGenerator(atof(split[7].c_str()), 2 * 314 * atof(split[16].c_str()));
					gBuses[gBun++]->setName(split[2]);
				}
			}
		}

	}

	fp.clear();
	fp.close();
	for (int i = 0; i < gBun; i++)
	{
		for (int j = 0; j < bun; j++)
		{
			if (gBuses[i]->name == buses[j]->name)
			{
				gBuses[i]->bindBus(buses[j]);
				//gBuses[i]->initial();
				gBuses[i]->setStep(step);
				delete buses[j];
				buses[j] = gBuses[i];
				break;
			}
		}
	}
	delete [] gBuses;

	for (int i = 0; i < bun; i++)
	{
		if (buses[i]->LP > 1e-5)
		{
			SimpleLoad *sl = new SimpleLoad();
			sl->bindBus(buses[i]);
			//sl->initial();
			delete buses[i];
			buses[i] = sl;
		}
	}
	return true;
}
void AnalysisModule::printBuses() const
{
	for (int i = 0; i < bun; i++)
	{
		buses[i]->print();
	}
}

void AnalysisModule::simulate(int n) const
{
	PowerSystem::FastPowerFlow fpf;
	fpf.InitBaBFromMemory(buses, bun, branches, bran);
	fpf.CreateYMatrix();
	fpf.FlowCal(PowerSystem::FastPowerFlow::BX, 100);
	fpf.ToFile();
	fpf.ToMemory(buses);
	
	for (int i = 0; i < bun; i++)
	{
		buses[i]->initial();
	}
	cout << "Time:0-" << endl;
	print();
	//printBuses();
	SimplePowerNet * sn = new SimplePowerNet(buses, branches, bun, bran);
	sn->calcY();

	double timed = time + step * n / 10;
	for (double t = 0; t < timed; t += n * step)
	{
		cout << "Time:" << t << "s\n";
		
		for (int i = 0; i < bun; i++)
		{
			if (buses[i]->getModelType() == BasicModel::GENERATOR)
			{
				((SimpleGenerator*)buses[i])->nextStep(n);
			}
		}
		
		Complex * U = sn->NodeVoltageMethod();

		for (int i = 0; i < bun; i++)
		{
			if (buses[i]->getModelType() == BasicModel::GENERATOR)
			{
				((SimpleGenerator*)buses[i])->endStep(U[i]);
			} else {
				buses[i]->setCU(U[i]);
			}
		}
		delete [] U;
		print();
		//system("pause");
		//printBuses();
	}
	delete sn;
}

SimplePowerNet * AnalysisModule::readySimulate() const
{
	PowerSystem::FastPowerFlow fpf;
	fpf.InitBaBFromMemory(buses, bun, branches, bran);
	fpf.CreateYMatrix();
	fpf.FlowCal(PowerSystem::FastPowerFlow::BX, 100);
	fpf.ToFile();
	fpf.ToMemory(buses);
	
	for (int i = 0; i < bun; i++)
	{
		buses[i]->initial();
	}
	cout << "Time:0-" << endl;
	print();
	//printBuses();
	SimplePowerNet * sn = new SimplePowerNet(buses, branches, bun, bran);
	sn->calcY();
	return sn;
}

void AnalysisModule::simulateStep(SimplePowerNet * sn, double * del, double * v) const
{
	for (int i = 0; i < bun; i++)
	{
		if (buses[i]->getModelType() == BasicModel::GENERATOR)
		{
			((SimpleGenerator*)buses[i])->nextStep();
		}
	}

	Complex * U = sn->NodeVoltageMethod();

	for (int i = 0; i < bun; i++)
	{
		if (buses[i]->getModelType() == BasicModel::GENERATOR)
		{
			((SimpleGenerator*)buses[i])->endStep(U[i]);
		} else {
			buses[i]->setCU(U[i]);
		}
	}
	delete [] U;

	int num = 0;
	for (int i = 0; i < bun; i++)
	{
		if (buses[i]->getModelType() == BasicModel::GENERATOR)
		{
			SimpleGenerator* sg = (SimpleGenerator*)buses[i];
			del[num] = sg->getDelta();
			v[num] = sg->getV();
			num++;
		}
	}

}

void AnalysisModule::endSimulate(SimplePowerNet *sn) const
{
	if (sn != NULL)
	{
		delete sn;
		sn = NULL;
	}
}
void AnalysisModule::print() const
{
	for (int i = 0; i < bun; i++)
	{
		if (buses[i]->getModelType() == BasicModel::GENERATOR)
		{
			buses[i]->print();
		}
	}
	cout << "*******************  end  *********************" << endl;
}

AnalysisModule::~AnalysisModule()
{
	for (int i = 0; i < bun; i++)
	{
		delete buses[i];
	}
	delete [] buses;

	for (int i = 0; i < bran; i++)
	{
		delete branches[i];
	}
	delete [] branches;
}

Branch* AnalysisModule::cuteLine(int id)
{
	//To be achieved
	return NULL;
	
}
void AnalysisModule::recloseLine(Branch* bra)
{
}

int AnalysisModule::getGenNum() const
{
	int num = 0;
	for (int i = 0; i < bun; i++)
	{
		if (buses[i]->getModelType() == BasicModel::GENERATOR)
		{
			num++;
		}
	}
	return num;
}

bool AnalysisModule::busShortCircuit(SimplePowerNet *sn, int id) const
{
	return sn->setcYById(id, Complex(ULONG_MAX, ULONG_MAX));
}