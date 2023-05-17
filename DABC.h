#pragma once
#include <cassert>
#include <iomanip>
#include <climits>
#include "HFSP.h"
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <set>
#include<fstream>
#include<sstream>


// Global varibles for bees colony
int minForLeap = INT_MAX;
vector <vector <int> > pChrom;
vector <int> pAge;
vector<int> bestSeqsofar;
vector <int> Valuesofar;

int gLimit = 100;


void PairwiseSwap(vector <int>& p)
{
	int pt1, pt2;
	pt1 = rand() % (p.size() - 1);
	pt2 = pt1 + 1;

	int Temp = p[pt1];
	p[pt1] = p[pt2];
	p[pt2] = Temp;
}

void quickSwap(vector <int>& Seq, int len)
{

	vector <int>TempSeq(pJob, -1);
	TempSeq = Seq;

	int i = 0;
	int j = len - 1;
	while (i < j)
	{
		if (i < j)
		{
			swap(TempSeq[i], TempSeq[j]);
			if (GetEN_CON(TempSeq, len) < GetEN_CON(Seq, len))
			{
				Seq = TempSeq;
			}
			else {
				TempSeq = Seq;
			}
			j--;
			i++;
		}
	}
}

void mutation(vector <int>& p)
{
	int Times = rand() % 6 + 5;
	for (int i = 0; i < Times; i++)
		PairwiseSwap(p);
}

void Insert(vector <int>& p)
{
	//Find two distinct elements randomly;
	int  pt1, pt2, Ward;
	do
	{
		pt1 = rand() % pJob;
		pt2 = rand() % pJob;
	} while (pt1 == pt2);

	if (pt2 < pt1)
	{
		int  temp = pt2;
		pt2 = pt1;
		pt1 = temp;
	}

	Ward = rand() % 2;
	//insert foreword;
	if (Ward)
	{
		int  temp = p[pt2];
		do
		{
			p[pt2] = p[pt2 - 1];
			pt2 = pt2 - 1;
		} while (pt2 > pt1);
		p[pt2] = temp;
	}
	else
	{
		// insert backword;
		int  temp = p[pt1];
		do
		{
			p[pt1] = p[pt1 + 1];
			pt1 = pt1 + 1;
		} while (pt1 < pt2);
		p[pt1] = temp;
	}
}

void IGImprovement_Insertion(vector<int>& Seq)
{
	vector<int> tempJobSeq(pJob, 0);
	vector<int> tempJobSeq1(pJob, 0);

	tempJobSeq1 = Seq;
	int temp;
	int minValue = GetEN_CON(Seq, pJob);;
	int pos;


	/*bool improvement = true;
	while (improvement == true)
	{
		improvement = false;*/

	for (int k = 0; k < pJob; k++)
	{
		pos = k;
		int tempValue = tempJobSeq1[k];
		if (pos != pJob - 1) {
			for (int i = pos; i < pJob - 1; ++i)
			{
				tempJobSeq1[i] = tempJobSeq1[i + 1];

			}
		}
		tempJobSeq1[pJob - 1] = -1;
		vector<int> tempJobSeq2(pJob, 0);
		tempJobSeq2 = tempJobSeq1;
		vector<int> BestvalueSeq(pJob, -1);
		for (int j = 0; j < pJob; ++j)
		{
			if (j == pJob - 1)
			{
				tempJobSeq2[j] = tempValue;
			}
			else {
				for (int i = pJob - 2; i >= j; i--)
				{
					tempJobSeq2[i + 1] = tempJobSeq2[i];
				}
				tempJobSeq2[j] = tempValue;
			}
			int Value = GetEN_CON(tempJobSeq2, pJob);

			if (Value < minValue)
			{
				minValue = Value;
				BestvalueSeq = tempJobSeq2;

			}

			tempJobSeq2 = tempJobSeq1;
		}
		if (BestvalueSeq[0] != -1)
		{
			tempJobSeq1 = BestvalueSeq;
			Seq = BestvalueSeq;
			//improvement = true;
		}
		else {

			tempJobSeq1 = Seq;
		}
	}

	//NormalSwap(tempJobSeq1);

	//}

}

int InitPop(int PSize, int& minForLeap)
{
	vector <int> Seq(pJob);
	for (int i = 0; i < pJob; i++)
		Seq[i] = i;

	pChrom.resize(PSize);
	for (int i = 0; i < PSize; i++)
		pChrom[i].resize(pJob);
	Valuesofar.resize(PSize);
	pAge.resize(PSize);


	for (int i = 0; i < PSize; i++)
	{
		pChrom[i] = Seq;
		random_shuffle(pChrom[i].begin(), pChrom[i].end());
		Valuesofar[i] = GetEN_CON(pChrom[i], pJob);
	}

	for (int i = 0; i < PSize; i++) pAge[i] = 0;

	int pt = 0;
	minForLeap = Valuesofar[0];
	for (int i = 1; i < PSize; i++)
	{
		if (Valuesofar[i] < minForLeap)
		{
			minForLeap = Valuesofar[i];
			pt = i;
		}
	}

	bestSeqsofar = pChrom[pt];
	return minForLeap;
}


void EvovlePop(int& minForLeap, int TimeLimit, int PSize)
{


	long time1 = GetTickCount64();

	while ((GetTickCount64() - time1) < TimeLimit)
	{
		vector <int> TempSeq;
		double TempSpan;

		//Employ bees 
		for (int i = 0; i < PSize; i++)
		{
			TempSeq = pChrom[i];
			mutation(TempSeq);
			TempSpan = GetEN_CON(TempSeq, pJob);
			if (TempSpan <= Valuesofar[i])
			{
				Valuesofar[i] = TempSpan;
				pChrom[i] = TempSeq;
				pAge[i] = 0;
			}
			else
				pAge[i]++;
		}

		//Onlooker bees;
		for (int i = 0; i < PSize; i++)
		{
			int pt1, pt2, pt;
			do {
				pt1 = rand() % PSize;
				pt2 = rand() % PSize;
			} while (pt1 == pt2);

			pt = (Valuesofar[pt1] <= Valuesofar[pt2]) ? pt1 : pt2;

			TempSeq = pChrom[pt];
			mutation(TempSeq);
			TempSpan = GetEN_CON(TempSeq, pJob);

			//find the worst individual
			int maxOb = 0, maxpt = -1;
			for (int k = 0; k < PSize; k++)
			{
				if (Valuesofar[k] > maxOb)
				{
					maxOb = Valuesofar[k];
					maxpt = k;
				}
			}

			if (TempSpan < maxOb)
			{
				bool bExist = false;
				for (int k = 0; k < PSize; k++)
				{
					if (TempSeq == pChrom[k])
					{
						bExist = true;
						break;
					}
				}

				if (!bExist)
				{
					Valuesofar[maxpt] = TempSpan;
					pChrom[maxpt] = TempSeq;
					pAge[maxpt] = 0;
				}
			}
			else
				pAge[pt]++;
		}


		//scount bees
		for (int i = 0; i < PSize; i++)
		{
			if (pAge[i] > gLimit)
			{
				Insert(pChrom[i]);
				Insert(pChrom[i]);
				Insert(pChrom[i]);
				Valuesofar[i] = GetEN_CON(pChrom[i], pJob);
				pAge[i] = 0;
			}
		}

		//Find the best individual
		int pt = -1;
		double TempminSpan = INT_MAX;
		for (int i = 0; i < PSize; i++)
		{
			if (Valuesofar[i] < TempminSpan)
			{
				TempminSpan = Valuesofar[i];
				pt = i;
			}
		}


		if (Valuesofar[pt] < minForLeap)
		{
			bestSeqsofar = pChrom[pt];
			minForLeap = Valuesofar[pt];
		}

	}
}

void EvovlePop_Curve(int& minForLeap, int TimeLimit, int PSize, long Time, ofstream& ofile)
{   //进化曲线


	long time1 = GetTickCount64();

	while ((GetTickCount64() - time1) < TimeLimit)
	{
		vector <int> TempSeq;
		double TempSpan;

		//Employ bees 
		for (int i = 0; i < PSize; i++)
		{
			TempSeq = pChrom[i];
			mutation(TempSeq);
			TempSpan = GetEN_CON(TempSeq, pJob);
			if (TempSpan <= Valuesofar[i])
			{
				Valuesofar[i] = TempSpan;
				pChrom[i] = TempSeq;
				pAge[i] = 0;
			}
			else
				pAge[i]++;
		}

		//Onlooker bees;
		for (int i = 0; i < PSize; i++)
		{
			int pt1, pt2, pt;
			do {
				pt1 = rand() % PSize;
				pt2 = rand() % PSize;
			} while (pt1 == pt2);

			pt = (Valuesofar[pt1] <= Valuesofar[pt2]) ? pt1 : pt2;

			TempSeq = pChrom[pt];
			mutation(TempSeq);
			TempSpan = GetEN_CON(TempSeq, pJob);

			//find the worst individual
			int maxOb = 0, maxpt = -1;
			for (int k = 0; k < PSize; k++)
			{
				if (Valuesofar[k] > maxOb)
				{
					maxOb = Valuesofar[k];
					maxpt = k;
				}
			}

			if (TempSpan < maxOb)
			{
				bool bExist = false;
				for (int k = 0; k < PSize; k++)
				{
					if (TempSeq == pChrom[k])
					{
						bExist = true;
						break;
					}
				}

				if (!bExist)
				{
					Valuesofar[maxpt] = TempSpan;
					pChrom[maxpt] = TempSeq;
					pAge[maxpt] = 0;
				}
			}
			else
				pAge[pt]++;
		}


		//scount bees
		for (int i = 0; i < PSize; i++)
		{
			if (pAge[i] > gLimit)
			{
				Insert(pChrom[i]);
				Insert(pChrom[i]);
				Insert(pChrom[i]);
				Valuesofar[i] = GetEN_CON(pChrom[i], pJob);
				pAge[i] = 0;
			}
		}

		//Find the best individual
		int pt = -1;
		double TempminSpan = INT_MAX;
		for (int i = 0; i < PSize; i++)
		{
			if (Valuesofar[i] < TempminSpan)
			{
				TempminSpan = Valuesofar[i];
				pt = i;
			}
		}


		if (Valuesofar[pt] < minForLeap)
		{
			bestSeqsofar = pChrom[pt];
			minForLeap = Valuesofar[pt];
		}
		//进化后的值
		long Localtime = GetTickCount() - Time;
		ofile << Localtime << "\t" << minForLeap << "\t" << endl;

	}
	ofile.close();
}

int OutputBest(const int ins, int& minForLeap, double runtime)
{

	//cout << "实例" << ins << "的makespan： " << minForLeap << "    " << runtime << "s" << endl; ;
	cout << setw(3) << minForLeap << "   " << setprecision(2) << fixed << runtime << "s";
	return minForLeap;
}


int DABC(const int ps, const int ins, int TimeLimit)  //在这里面建立种群;
{
	long Time = GetTickCount();
	InitPop(ps, minForLeap);

	EvovlePop(minForLeap, TimeLimit, ps);
	long endTime = GetTickCount();
	double runtime = (endTime - Time) * 1.0 / 1000;
	int bestValuesofar = OutputBest(ins, minForLeap,runtime);

	return bestValuesofar;
}

int DABC_evolveCurve(const int ps, const int ins, int TimeLimit)  
{    //进化曲线
	long Time = GetTickCount();

	int bestValuesofar = InitPop(ps, minForLeap);

	//初始值：
	ostringstream str;
	str << "DABC_" << "EvolveCurve_" << ins + 1 << ".txt";
	ofstream ofile;
	ofile.open(str.str());
	long Localtime = GetTickCount() - Time;
	ofile << Localtime << "\t" << bestValuesofar << "\t" << endl;
	//进化
	EvovlePop_Curve(minForLeap, TimeLimit, ps,Time,ofile);

	long endTime = GetTickCount();
	double runtime = (endTime - Time) * 1.0 / 1000;
	bestValuesofar = OutputBest(ins, minForLeap, runtime);

	return bestValuesofar;
}