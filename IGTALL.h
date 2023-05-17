#pragma once
#include <cassert>
#include <iomanip>
#include<iostream>
#include<Windows.h>
#include "HFSP.h"
#include"NEH_normal.h"
#include "RIS.h"
#include "RSS.h"
#include<fstream>
#include<sstream>

FILE* p;


void IGImprovement_Insertion(vector<int>& Seq, int len)
{
	vector<int> tempJobSeq(len, 0);
	vector<int> tempJobSeq1(len, 0);

	tempJobSeq1 = Seq;
	int temp;
	int minValue = GetEN_CON(Seq, len);;
	int pos;


	bool improvement = true;
	while (improvement == true)
	{
		improvement = false;

		for (int k = 0; k < len; k++)
		{
			pos = k;
			int tempValue = tempJobSeq1[k];
			if (pos != len - 1) {
				for (int i = pos; i < len - 1; ++i)
				{
					tempJobSeq1[i] = tempJobSeq1[i + 1];

				}
			}
			tempJobSeq1[len - 1] = -1;
			vector<int> tempJobSeq2(len, 0);
			tempJobSeq2 = tempJobSeq1;
			vector<int> BestvalueSeq(len, -1);
			for (int j = 0; j < len; ++j)
			{
				if (j == len - 1)
				{
					tempJobSeq2[j] = tempValue;
				}
				else {
					for (int i = len - 2; i >= j; i--)
					{
						tempJobSeq2[i + 1] = tempJobSeq2[i];
					}
					tempJobSeq2[j] = tempValue;
				}
				int Value = GetEN_CON(tempJobSeq2, len);

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
				improvement = true;
			}
			else {

				tempJobSeq1 = Seq;
			}
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

	vector<int> minValue(pJob + 1, INT_MAX);
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

		IGImprovement_Insertion(tempJobSeq_other1, pJob - d);

		int q, w, e;
		bestSeqSofar = tempJobSeq_other1;

		for (e = 0; e < d; ++e)
		{
			tempJobSeq_other1 = bestSeqSofar;
			for (q = 0; q < pJob - d + e; ++q)
			{
				tempJobSeq_other2 = tempJobSeq_other1;
				int minValue = INT_MAX;
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

		double r = rand() * 1.0 / RAND_MAX;
		if (r < jP)
		{
			RIS(tempJobSeq1, Seqb);
		}
		else
		{
			RSS(tempJobSeq1, Seqb);
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

int Evolve_Curve(vector<int> Seq, int d, int TimeLimit, vector<int>& Seqb, int Temperature, int jP,long Time,ofstream&ofile)
{
	vector<int> tempJobSeq(pJob, -1);
	vector<int> Seq2(pJob, -1);
	vector<int> tempJobSeq1(pJob, -1);
	vector<int> tempJobSeq_R(pJob, -1);
	vector<int> bestSeqSofar(pJob, -1);
	vector<int> tempJobSeq_other1(pJob, -1);
	vector<int> tempJobSeq_other2(pJob, -1);

	vector<int> minValue(pJob + 1, INT_MAX);
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

		IGImprovement_Insertion(tempJobSeq_other1, pJob - d);

		int q, w, e;
		bestSeqSofar = tempJobSeq_other1;

		for (e = 0; e < d; ++e)
		{
			tempJobSeq_other1 = bestSeqSofar;
			for (q = 0; q < pJob - d + e; ++q)
			{
				tempJobSeq_other2 = tempJobSeq_other1;
				int minValue = INT_MAX;
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

		double r = rand() * 1.0 / RAND_MAX;
		if (r < jP)
		{
			RIS(tempJobSeq1, Seqb);
		}
		else
		{
			RSS(tempJobSeq1, Seqb);
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

		//进化后的值输出
		int best_span = GetEN_CON(Seqb, pJob);
		long Localtime = GetTickCount() - Time;
		ofile << Localtime << "\t" << best_span << "\t" << endl;

	}
	ofile.close();
	return GetEN_CON(Seqb, pJob);
}



int IGTALL(int Ins, int TimeLimit)
{
	long Time = GetTickCount();
	int d = 3;
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

	pJobSeqb = pJobSeq;
	int bestValuesofar = Evolve(pJobSeq, d, TimeLimit, pJobSeqb, Temperature, 0.4);

	long endTime = GetTickCount();
	double runtime = (endTime - Time) * 1.0 / 1000;
	cout << setw(3) << bestValuesofar << "   " << setprecision(2) << fixed << runtime << "s";
	return bestValuesofar;
}

int IGTALL_evolveCurve(int Ins, int TimeLimit)
{
	long Time = GetTickCount();
	int d = 3;
	int bestValuesofar;
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
	bestValuesofar = NEH(pJobSeq);

	//初始值：
	ostringstream str;
	str << "IGTALL_" << "EvolveCurve_" << Ins + 1 << ".txt";
	ofstream ofile;
	ofile.open(str.str());
	long Localtime = GetTickCount() - Time;
	ofile << Localtime << "\t" << bestValuesofar << "\t" << endl;

	pJobSeqb = pJobSeq;
	//进化：
	bestValuesofar = Evolve_Curve(pJobSeq, d, TimeLimit, pJobSeqb, Temperature, 0.4,Time,ofile);

	long endTime = GetTickCount();
	double runtime = (endTime - Time) * 1.0 / 1000;
	cout << setw(3) << bestValuesofar << "   " << setprecision(2) << fixed << runtime << "s";
	return bestValuesofar;
}
