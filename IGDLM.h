#pragma once
#include <cassert>
#include <iomanip>
#include<iostream>
#include<Windows.h>
#include "HFSP.h"
#include"NEH_normal.h"
#include<fstream>
#include<sstream>
//#include"MME.h"
//#include "RIS.h"
//#include "RSS.h"
//#include "Jaya.h"

FILE* p;

void NormalSwap(vector <int>& p)
{
	//Find two distinct elements randomly;
	for (int i = 0; i < pJob; ++i)
	{
		vector <int> p1(pJob, 0);
		p1 = p;
		int  pt1, pt2;
		do
		{
			pt1 = rand() % pJob;
			pt2 = rand() % pJob;
		} while (pt1 == pt2);

		swap(p1[pt1], p1[pt2]);
		if (GetEN_CON(p1, pJob) < GetEN_CON(p, pJob))
		{
			p = p1;
		}
	}

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

int Evolve(vector<int> Seq, int d, int TimeLimit, vector<int>& Seqb, int Temperature, int jP)
{

	vector<int> tempJobSeq(pJob, -1);
	vector<int> Seq2(pJob, -1);
	vector<int> tempJobSeq1(pJob, -1);
	vector<int> tempJobSeq_R(pJob, -1);
	vector<int> bestSeqSofar(pJob, -1);
	vector<int> tempJobSeq_other1(pJob, -1);
	vector<int> tempJobSeq_other2(pJob, -1);


	int count2 = 0;
	long time1 = GetTickCount64();

	while ((GetTickCount64() - time1) < TimeLimit)
	{
		int pt;
		int len = 0;
		int count = 1;



		vector<bool> PosIsSelected(pJob, 0);

		tempJobSeq = Seq;


		//destruction
		while (count <= d)
		{
			pt = rand() % pJob;
			if (PosIsSelected[pt] == false) {
				PosIsSelected[pt] = 1;
				count++;
				tempJobSeq_R[len] = tempJobSeq[pt];
				len++;
				tempJobSeq[pt] = -1;
			}

		}
		len = 0;
		for (int k = 0; k < pJob; ++k)
		{
			if (tempJobSeq[k] != -1) {

				tempJobSeq_other1[len] = tempJobSeq[k];
				len++;
			}
		}

		//construction
		int q, w, e;
		bestSeqSofar = tempJobSeq_other1;

		for (e = 0; e < d; ++e)
		{
			tempJobSeq_other1 = bestSeqSofar;
			int minValue = INT_MAX;
			for (q = 0; q < pJob - d + e; ++q)
			{
				tempJobSeq_other2 = tempJobSeq_other1;
				for (w = pJob - d + e - 1; w >= q; --w)
				{
					tempJobSeq_other2[w + 1] = tempJobSeq_other2[w];
				}
				tempJobSeq_other2[q] = tempJobSeq_R[e];

				vector<int> tempJobSeq_other3(pJob - d + e + 1, -1);
				tempJobSeq_other3 = tempJobSeq_other2;
				if (GetEN_CON(tempJobSeq_other3, pJob - d + e + 1) < minValue)
				{
					minValue = GetEN_CON(tempJobSeq_other3, pJob - d + e + 1);
					bestSeqSofar = tempJobSeq_other3;
				}
			}

		}


		tempJobSeq1 = bestSeqSofar;

		//IGImprovement_Swap(tempJobSeq1);


		if (GetEN_CON(tempJobSeq1, pJob) < GetEN_CON(Seqb, pJob)) {
			Seqb = tempJobSeq1;
		}

		int r = rand() * 1.0 / RAND_MAX;
		if (r < 0.3) {
			NormalSwap(tempJobSeq1);
		}
		int r1 = rand() * 1.0 / RAND_MAX;
		if (r1 < 0.3) {
			quickSwap(tempJobSeq1, pJob);
		}

		if (GetEN_CON(tempJobSeq1, pJob) <= GetEN_CON(Seq, pJob))
		{
			Seq = tempJobSeq1;
			if (GetEN_CON(Seq, pJob) < GetEN_CON(Seqb, pJob)) {
				Seqb = tempJobSeq1;
			}
		}

		else if (((double)rand() / RAND_MAX) <= exp(-(GetEN_CON(tempJobSeq1, pJob) - GetEN_CON(Seq, pJob)) / Temperature)) {

			Seq = tempJobSeq1;

		}

	}

	return GetEN_CON(Seqb, pJob);
}


int IGDLM(int Ins, int TimeLimit)
{
	long Time = GetTickCount();
	int d = 4;
	vector <int>pJobSeq(pJob, 0);
	vector <int>pJobSeqb(pJob, 0);
	int totalPtime = 0;
	for (int j = 0; j < pJob; ++j)
	{
		for (int k = 0; k < pStage; ++k)
		{
			totalPtime += pTime[k][j];
		}
	}
	double Temperature = (totalPtime / 10.0 * pJob * pStage) * 0.50;
	NEH(pJobSeq);
	//IGImprovement_Swap(pJobSeq);
	//MME(pJobSeq);	
	pJobSeqb = pJobSeq;
	int bestValuesofar = Evolve(pJobSeq, d, TimeLimit, pJobSeqb, Temperature, 0.4);

	long endTime = GetTickCount();
	double runtime = (endTime - Time) * 1.0 / 1000;
	cout << setw(3) << bestValuesofar << "   " << setprecision(2) << fixed << runtime << "s";
	return bestValuesofar;
}
