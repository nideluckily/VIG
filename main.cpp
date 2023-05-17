#define _CRT_SECURE_NO_WARNINGS
#include"HFSP.h"
//#include"IGDLM.h"
//#include"DABC.h"
//#include"GA.h"
//#include"IGA.h"
//#include"MBO.h"
//#include"DPSO.h"
//#include"IGTALL.h"
#include"VIG.h"


#include<iostream>
#include<fstream>


int run(int ps, int ins, int CPU, int pJob, int pStage, int limit)
{
	cout << "实例" << setw(3) << ins << ":\t";
	//ofstream outfile("data.txt",ios::trunc);
	int TimeLimit = pJob * pStage * CPU;
	int bestValuesofar = INT_MAX;

	//bestValuesofar = DABC(ps,ins, TimeLimit);
	//bestValuesofar = GA(ps, 0.7, 0.1, ins, TimeLimit);
	//bestValuesofar = MBO(ps, 3, 1, 10, ins, 1000, limit, TimeLimit);
	//bestValuesofar = DPSO(ps,ins, TimeLimit);
	//bestValuesofar = IGTALL(ins, TimeLimit);
	//bestValuesofar = IGDLM(ins, TimeLimit);

	bestValuesofar = VIG(ins, TimeLimit);

	cout << endl;
	return bestValuesofar;
}

//主函数

int main()
{
	int ps = 20;
	int CPU = 10;
	int limit;

	//ofstream outfile("DABC_data.txt", ios::trunc);
	//ofstream outfile("GA_data.txt", ios::trunc);
	//ofstream outfile("EMBO_data.txt", ios::trunc);
	//ofstream outfile("DPSO_data.txt", ios::trunc);
	//ofstream outfile("IGTALL_data.txt", ios::trunc);
	//ofstream outfile("IGDLM_data.txt", ios::trunc);

	ofstream outfile("VIG_data.txt", ios::trunc);


	int bestValue;
	for (int Ins = 0; Ins < 100; Ins++)
	{
		int InPara = Ins / 10;
		int InJob = Jobs[InPara / 2];
		int InStage = Stages[InPara % 2];
		GenerateInstances(InJob, InStage, Ins + 1234);

		STime = vector<vector<int>>(pStage, vector<int>(pJob));
		CTime = vector<vector<int>>(pStage, vector<int>(pJob));

		MIdleTime.resize(pStage); //机器开始空闲的时间
		for (int k = 0; k < pStage; k++)
		{
			MIdleTime[k].resize(pMachines[k]);
		}

		if (InJob == 10)
		{
			limit = 20;
		}

		if (InJob == 20)
		{
			limit = 30;
		}
		if (InJob == 40 || InJob == 60)
		{
			limit = 50;
		}
		if (InJob == 80 || InJob == 100)
		{
			limit = 80;
		}
		if (InJob == 200)
		{
			limit = 200;
		}
		if (InJob == 300)
		{
			limit = 300;
		}
		if (InJob == 400)
		{
			limit = 400;
		}
		if (InJob == 500)
		{
			limit = 500;
		}
		if (InJob == 600)
		{
			limit = 600;
		}
		srand(GetTickCount());
		for (int rep = 0; rep < 30; rep++)
		{
			bestValue = run(ps, Ins, CPU, pJob, pStage, limit);
			outfile << bestValue << "     ";
		}
		outfile << endl;
	}

	cout << "___End____" << endl;
	outfile.close();
	return 0;
}
