#pragma once
#include <cassert>
#include <iomanip>
#include<iostream>
#include<Windows.h>
#include "HFSP.h"
#include<fstream>
#include<sstream>

class Gene
{
public:
	vector<int> pJobSeq;
	int EC;
public:
	Gene();
	Gene(const Gene& obj);
	Gene& operator= (const Gene& obj);
};

Gene::Gene()
{
	pJobSeq.resize(pJob);
	for (int i = 0; i < pJob; i++)
	{
		pJobSeq[i] = i;
	}
	random_shuffle(pJobSeq.begin(), pJobSeq.end());

	EC = GetEN_CON(pJobSeq, pJob);
}

Gene::Gene(const Gene& obj)
{
	this->pJobSeq = obj.pJobSeq;
	this->EC = obj.EC;
}

Gene& Gene::operator= (const Gene& obj)
{
	this->pJobSeq = obj.pJobSeq;
	this->EC = obj.EC;

	return *this;
}

FILE* p;
void Crossover(int pt1, int pt2, vector<int>& childSeq1, vector<int>& childSeq2, vector<Gene>& Population)
{
	vector<int> temp2Seq;
	temp2Seq.resize(2 * pJob);

	int seq = 0;
	for (int i = 0; i < 2 * pJob; i += 2)
	{
		temp2Seq[i] = Population[pt1].pJobSeq[seq];
		temp2Seq[i + 1] = Population[pt2].pJobSeq[seq];
		seq++;
	}

	int seq1 = 0;
	int seq2 = 0;
	for (int i = 0; i < 2 * pJob; i++)
	{
		vector<int>::iterator it = find(childSeq1.begin(), childSeq1.end(), temp2Seq[i]);
		if (it == childSeq1.end())
		{
			childSeq1[seq1] = temp2Seq[i];
			seq1++;
		}
		else
		{
			childSeq2[seq2] = temp2Seq[i];
			seq2++;
		}

	}

}

void InitPop(vector<Gene>& Population, int ps)
{
	Population.resize(ps);
}

void EvolvePop(vector<Gene>& Population, int ps, double pc, double pm, int TimeLimit)
{
	long initTime = GetTickCount64();

	while ((GetTickCount64() - initTime) < TimeLimit)
	{
		for (int i = 0; i < ps; i++)
		{
			int pt1, pt2;
			do
			{
				pt1 = rand() % ps;
				pt2 = rand() % ps;
			} while (pt1 == pt2);

			//crossover
			vector<int> childSeq1(pJob, pJob), childSeq2(pJob, pJob);
			double p = rand() * 1.0 / RAND_MAX;
			if (p < pc)    //需要将1.0改为pc
			{
				Crossover(pt1, pt2, childSeq1, childSeq2, Population);  //为了得到childSeq1,childSeq2
			}
			else
			{
				childSeq1 = Population[pt1].pJobSeq;
				childSeq2 = Population[pt2].pJobSeq;
			}

			//mutation
			if ((rand() * 1.0 / RAND_MAX) < pm)
			{
				int a, b;
				do
				{
					a = rand() % pJob;
					b = rand() % pJob;
				} while (a == b);
				swap(childSeq1[a], childSeq1[b]);
			}
			if ((rand() * 1.0 / RAND_MAX) < pm)
			{
				int a, b;
				do
				{
					a = rand() % pJob;
					b = rand() % pJob;
				} while (a == b);
				swap(childSeq2[a], childSeq2[b]);
			}

			int tempTF1 = GetEN_CON(childSeq1, pJob);
			int tempTF2 = GetEN_CON(childSeq2, pJob);

			//Generation scheme------find the worst individual
			int maxEC = Population[0].EC;
			int maxIndex = 0;

			for (int i = 1; i < ps; i++)
			{
				if (Population[i].EC > maxEC)
				{
					maxEC = Population[i].EC;
					maxIndex = i;
				}
			}

			if (tempTF1 < maxEC)
			{
				Population[maxIndex].pJobSeq = childSeq1;
				Population[maxIndex].EC = tempTF1;
			}

			//repeat for child 2 -------find the worst individual

			maxEC = Population[0].EC;
			maxIndex = 0;

			for (int i = 1; i < ps; i++)
			{
				if (Population[i].EC > maxEC)
				{
					maxEC = Population[i].EC;
					maxIndex = i;
				}
			}

			if (tempTF2 < maxEC)
			{
				Population[maxIndex].pJobSeq = childSeq2;
				Population[maxIndex].EC = tempTF2;
			}
		}
	}
}

void EvolvePop_Curve(vector<Gene>& Population, int ps, double pc, double pm, int TimeLimit, long Time, ofstream& ofile)
{
	long initTime = GetTickCount64();

	while ((GetTickCount64() - initTime) < TimeLimit)
	{
		for (int i = 0; i < ps; i++)
		{
			int pt1, pt2;
			do
			{
				pt1 = rand() % ps;
				pt2 = rand() % ps;
			} while (pt1 == pt2);

			//crossover
			vector<int> childSeq1(pJob, pJob), childSeq2(pJob, pJob);
			double p = rand() * 1.0 / RAND_MAX;
			if (p < pc)    //需要将1.0改为pc
			{
				Crossover(pt1, pt2, childSeq1, childSeq2, Population);  //为了得到childSeq1,childSeq2
			}
			else
			{
				childSeq1 = Population[pt1].pJobSeq;
				childSeq2 = Population[pt2].pJobSeq;
			}

			//mutation
			if ((rand() * 1.0 / RAND_MAX) < pm)
			{
				int a, b;
				do
				{
					a = rand() % pJob;
					b = rand() % pJob;
				} while (a == b);
				swap(childSeq1[a], childSeq1[b]);
			}
			if ((rand() * 1.0 / RAND_MAX) < pm)
			{
				int a, b;
				do
				{
					a = rand() % pJob;
					b = rand() % pJob;
				} while (a == b);
				swap(childSeq2[a], childSeq2[b]);
			}

			int tempTF1 = GetEN_CON(childSeq1, pJob);
			int tempTF2 = GetEN_CON(childSeq2, pJob);

			//Generation scheme------find the worst individual
			int maxEC = Population[0].EC;
			int maxIndex = 0;

			for (int i = 1; i < ps; i++)
			{
				if (Population[i].EC > maxEC)
				{
					maxEC = Population[i].EC;
					maxIndex = i;
				}
			}

			if (tempTF1 < maxEC)
			{
				Population[maxIndex].pJobSeq = childSeq1;
				Population[maxIndex].EC = tempTF1;
			}

			//repeat for child 2 -------find the worst individual

			maxEC = Population[0].EC;
			maxIndex = 0;

			for (int i = 1; i < ps; i++)
			{
				if (Population[i].EC > maxEC)
				{
					maxEC = Population[i].EC;
					maxIndex = i;
				}
			}

			if (tempTF2 < maxEC)
			{
				Population[maxIndex].pJobSeq = childSeq2;
				Population[maxIndex].EC = tempTF2;
			}
		}

		//进化后的值
		int minEC = Population[0].EC;
		for (int i = 1; i < Population.size(); i++)
		{
			if (Population[i].EC < minEC)
			{
				minEC = Population[i].EC;
			}
		}
		long Localtime = GetTickCount() - Time;
		ofile << Localtime << "\t" << minEC << "\t" << endl;
	}
	ofile.close();
}

int OutputBest(vector<Gene>& Population, int ins,double runtime)
{
	int minEC = Population[0].EC;
	for (int i = 1; i < Population.size(); i++)
	{
		if (Population[i].EC < minEC)
		{
			minEC = Population[i].EC;
		}
	}

	//	fprintf(p, "%d,", minEC);
	cout << setw(3) << minEC << "   " << setprecision(2) << fixed << runtime << "s";
	return minEC;
}

int GA(int ps, double pc, double pm, int Ins, int TimeLimit)
{
	long Time = GetTickCount();
	//define the population
	vector<Gene> Population;

	InitPop(Population, ps);

	EvolvePop(Population, ps, pc, pm, TimeLimit);
	long endTime = GetTickCount();
	double runtime = (endTime - Time) * 1.0 / 1000;
	int bestValuesofar = OutputBest(Population, Ins,runtime);
	return bestValuesofar;
}

int GA_evolveCurve(int ps, double pc, double pm, int Ins, int TimeLimit)
{
	long Time = GetTickCount();
	//define the population
	vector<Gene> Population;

	InitPop(Population, ps);

	//进化前的值
	ostringstream str;
	str << "GA_" << "EvolveCurve_" << Ins + 1 << ".txt";
	ofstream ofile;
	ofile.open(str.str());

	EvolvePop_Curve(Population, ps, pc, pm, TimeLimit,Time,ofile);
	long endTime = GetTickCount();
	double runtime = (endTime - Time) * 1.0 / 1000;
	int bestValuesofar = OutputBest(Population, Ins, runtime);
	return bestValuesofar;
}
