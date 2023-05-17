#pragma once
#include <cassert>
#include <iomanip>
#include <windows.h>         
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "HFSP.h"
#include <conio.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include<iostream>
#include<fstream>
#include<sstream>

using namespace std;

vector<vector <int> > pSeq, pbSeq;
vector <int> pSpan, pbSpan;
vector <int> gbest;


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

int init(int PSize, int& minSpan)
{
	//initlization
	vector <int> TempSeq(pJob);
	for (int i = 0; i < pJob; i++)
		TempSeq[i] = i;

	pSeq.clear();
	pSpan.clear();
	for (int i = 0; i < PSize; i++)
	{
		random_shuffle(TempSeq.begin(), TempSeq.end());
		int TempSpan = GetEN_CON(TempSeq, pJob);
		pSeq.push_back(TempSeq);
		pSpan.push_back(TempSpan);
	}
	pbSeq = pSeq;
	pbSpan = pSpan;

	int pt = 0;
	minSpan = pbSpan[0];
	for (int i = 1; i < PSize; i++)
	{
		if (pbSpan[i] < minSpan)
		{
			minSpan = pbSpan[i];
			pt = i;
		}
	}
	gbest = pbSeq[pt];
	return minSpan;
}


void SA(vector <int>& Seq, vector <int>& bSeq, int& Span, int& bSpan, double t0, double t1, double alpha, int loop)
{
	double t = t0;
	vector <int> TempSeq = Seq;
	while (t > t1)
	{
		for (int i = 0; i < loop; i++)
		{
			TempSeq = Seq;
			Insert(TempSeq);
			int TempSpan = GetEN_CON(TempSeq, pJob);
			if (TempSpan <= Span)
			{
				Span = TempSpan;
				Seq = TempSeq;
				if (TempSpan < bSpan)
				{
					bSpan = Span;
					bSeq = TempSeq;
				}
			}
			else
			{
				double r = rand() / double(RAND_MAX);
				if (r < exp((Span - TempSpan) / t))
				{
					Span = TempSpan;
					Seq = TempSeq;
				}
			}
		}
		t = t * alpha;
	}
}

void Evolve(int& minSpan, int Timelimit)
{
	long initTime = GetTickCount64();
	while ((GetTickCount64() - initTime) < Timelimit)
	{
		for (int i = 0; i < pSeq.size(); i++)
		{
			SA(pSeq[i], pbSeq[i], pSpan[i], pbSpan[i], 100, 0.01, 0.99, 1);
		}

		//Find gbest;
		int pt = -1;
		for (int i = 0; i < pSeq.size(); i++)
		{
			if (pbSpan[i] < minSpan)
			{
				minSpan = pbSpan[i];
				pt = i;
			}
		}
		if (pt >= 0)
			gbest = pbSeq[pt];
	}
}

int Evolve_Curve(int& minSpan, int Timelimit,long Time, ofstream& ofile)
{			//进化曲线
	long initTime = GetTickCount64();
	while ((GetTickCount64() - initTime) < Timelimit)
	{
		for (int i = 0; i < pSeq.size(); i++)
		{
			SA(pSeq[i], pbSeq[i], pSpan[i], pbSpan[i], 100, 0.01, 0.99, 1);
		}

		//Find gbest;
		int pt = -1;
		for (int i = 0; i < pSeq.size(); i++)
		{
			if (pbSpan[i] < minSpan)
			{
				minSpan = pbSpan[i];
				pt = i;
			}
		}
		if (pt >= 0) {
			gbest = pbSeq[pt];
		}
		
		//进化后的值：
		long Localtime = GetTickCount() - Time;
		ofile << Localtime << "\t" << minSpan << "\t" << endl;

	}
	ofile.close();
	return minSpan;
}

FILE* p;
int DPSO(int ps, int Ins, int TimeLimit)
{
	long Time = GetTickCount();
	int minSpan = INT_MAX;
	minSpan = init(ps, minSpan);

	Evolve(minSpan, TimeLimit);

	long endTime = GetTickCount();
	double runtime = (endTime - Time) * 1.0 / 1000;
	cout << setw(3) << minSpan << "   " << setprecision(2) << fixed << runtime << "s";

	return minSpan;
}

int DPSO_evolveCurve(int ps, int Ins, int TimeLimit)
{
	long Time = GetTickCount();
	int minSpan = INT_MAX;
	minSpan = init(ps, minSpan);

	//初始值：
	ostringstream str;
	str << "DPSO_" << "EvolveCurve_" << Ins + 1 << ".txt";
	ofstream ofile;
	ofile.open(str.str());
	long Localtime = GetTickCount() - Time;
	ofile << Localtime << "\t" << minSpan << "\t" << endl;

	//进化：
	minSpan = Evolve_Curve(minSpan, TimeLimit,Time,ofile);

	long endTime = GetTickCount();
	double runtime = (endTime - Time) * 1.0 / 1000;
	cout << setw(3) << minSpan << "   " << setprecision(2) << fixed << runtime << "s";

	return minSpan;
}