#include "stdafx.h"
#include "PowerSystemModel.h"
#include "AnalysisModule.h"

int main(int argc, char *argv[])
{
	AnalysisModule am(1, 20);
	if (!am.readDataFile("C:\\Users\\YaoPeng\\Desktop\\4G2A.dat"))
	{
		cout << "no legitimate file" << endl;
	}
	am.simulate();
	
	
	//cout << arg(polar(1.414, 1.414 / 2)) /3.14 * 180<< endl;
	//cout << (log(Complex(1, 1)/abs(Complex(1, 1))) * Complex(0, -1)).real() /3.14 * 180<< endl;
	system("PAUSE");
	return 0;
}