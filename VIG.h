#pragma once
#include <cassert>
#include <iomanip>
#include<iostream>
#include <list>
#include<Windows.h>
#include "HFSP.h"
#include"NEH_normal.h"
#include"MME.h"
#include<fstream>
#include<sstream>
using namespace std;


void Crossover(vector<int>& Seq1, vector<int>& Seq2)
{
	vector<int> tempSeq1;
	vector<int> tempSeq2;

	vector<int>::iterator it1;
	vector<int>::iterator it2;

	int pt1, pt2;
	do
	{
		pt1 = rand() % pJob;
		pt2 = rand() % pJob;
	} while (pt1 == pt2);

	if (pt2 < pt1)
	{
		int temp = pt2;
		pt2 = pt1;
		pt1 = temp;
	}

	int c = pt2 - pt1;
	int pos = pt1;
	while (tempSeq1.size() <= c)     //保留pt1到pt2元素
	{
		tempSeq1.push_back(Seq1[pos]);
		tempSeq2.push_back(Seq2[pos]);
		pos++;
	}

	//交叉前pt1个
	int c1 = 0, c2 = 0;
	for (int i = 0; i < pt1; ++i)
	{
		for (int j1 = c1; j1 < Seq1.size(); j1++)
		{
			c1++;
			it1 = find(tempSeq1.begin(), tempSeq1.end(), Seq2[j1]);
			if (it1 == tempSeq1.end())
			{
				tempSeq1.insert(begin(tempSeq1) + i, Seq2[j1]);
				break;
			}
		}

	}

	for (int i = 0; i < pt1; ++i)
	{
		for (int j2 = c2; j2 < Seq2.size(); j2++)
		{
			c2 ++;
			it2 = find(tempSeq2.begin(), tempSeq2.end(), Seq1[j2]);
			if (it2 == tempSeq2.end())
			{
				tempSeq2.insert(begin(tempSeq2) + i, Seq1[j2]);
				break;
			}
		}

	}
	//交叉后pjob-pt2
	int c3 = c1, c4 = c2;
	for (int i = pt2 + 1; i < Seq1.size(); ++i)
	{
		for (int k = c3; k < Seq1.size(); k++)
		{
			c3++;
			it1 = find(tempSeq1.begin(), tempSeq1.end(), Seq2[k]);
			if (it1 == tempSeq1.end())
			{
				tempSeq1.insert(begin(tempSeq1) + i, Seq2[k]);
				break;
			}

		}

	}

	for (int i = pt2 + 1; i < Seq1.size(); ++i)
	{
		for (int k = c4; k < Seq1.size(); k++)
		{
			c4++;
			it2 = find(tempSeq2.begin(), tempSeq2.end(), Seq1[k]);
			if (it2 == tempSeq2.end())
			{
				tempSeq2.insert(begin(tempSeq2) + i, Seq1[k]);
				break;
			}
		}

	}

	Seq1 = tempSeq1;
	Seq2 = tempSeq2;
}


int swap_1_WYT(vector<int>& Seq, int Span, int ward)
{
	int i = 0;
	int temp_span;
	while (i < pJob)
	{
		for (int k = i + 1; k < pJob; k++)
		{
			swap(Seq[i], Seq[k]);

			if (ward == 0)
			{
				temp_span = GetEN_CON_WYT(Seq, pJob);
			}
			else
			{
				temp_span = GetEN_CON_backward(Seq, pJob);
			}

			if (temp_span < Span)
			{
				Span = temp_span;
			}
			else
			{
				swap(Seq[i], Seq[k]);
			}
		}
		i++;
	}
	return Span;
}


int DR_normal(vector<int>& Seq, int d,int ward)
{
	//destruction
	vector<int> tempJobSeq_R;
	while (tempJobSeq_R.size() < d)
	{
		int pt = rand() % Seq.size();
		tempJobSeq_R.push_back(Seq[pt]);
		Seq.erase(begin(Seq) + pt);
	}

	//reconstruction    正常重建
	int span;
	int temp_span;

	for (int job : tempJobSeq_R)
	{
		int len = 1;
		span = INT_MAX;
		int best_pos = -1;
		for (int pos = 0; pos <= Seq.size(); pos++)
		{
			if (ward == 0)
			{
				temp_span = GetEN_CON_After_Insert_Job_WYT(Seq, job, pos);
			}
			else
			{
				temp_span = GetEN_CON_After_Insert_Job_backward(Seq, job, pos);
			}

			if (temp_span < span)
			{
				best_pos = pos;
				span = temp_span;
			}
		}
		Seq.insert(begin(Seq) + best_pos, job);
	}
	return span;
}

int Evolve_WYT(vector<int>& BestSeq, int best_span, vector<int>& Seq1, int Span1, vector<int>& Seq2, int Span2, int d, int TimeLimit, int Temperature)
{
	vector<int> best_seq;

	vector<int> best_seq1 = Seq1;
	int best_span1 = Span1;

	vector<int> temp_seq1 = Seq1;
	int temp_span1 = Span1;

	vector<int> best_seq2 = Seq2;
	int best_span2 = Span2;

	vector<int> temp_seq2 = Seq2;
	int temp_span2 = Span2;

	const long start_time = GetTickCount();
	int iterator_count = 0;
	int iter_crosscnt = 0;
	int fail_count = 0;

	int len = 1;

	while ((GetTickCount() - start_time) < TimeLimit)
	{

		temp_span1 = DR_normal(temp_seq1, d, 0); //破坏顺序重构

		if (temp_span1 < best_span1)
		{
			best_seq1 = temp_seq1;
			best_span1 = temp_span1;
		}

		temp_span1 = swap_1_WYT(temp_seq1, temp_span1, 0);

		if (temp_span1 <= Span1)
		{
			Seq1 = temp_seq1;
			Span1 = temp_span1;
			if (temp_span1 < best_span1)
			{
				best_seq1 = temp_seq1;
				best_span1 = temp_span1;
			}
		}
		else
		{
			if ((static_cast<double>(rand()) / RAND_MAX) <= exp(-(temp_span1 - Span1) / Temperature))//模拟退火
			{
				Seq1 = temp_seq1;
				Span1 = temp_span1;
			}
			else
			{
				temp_seq1 = Seq1;
				temp_span1 = Span1;
			}
		}


		temp_span2 = DR_normal(temp_seq2, d, 1);


		if (temp_span2 < best_span2)
		{
			best_seq2 = temp_seq2;
			best_span2 = temp_span2;
		}

		temp_span2 = swap_1_WYT(temp_seq2, temp_span2, 1);

		if (temp_span2 <= Span2)
		{
			Seq2 = temp_seq2;
			Span2 = temp_span2;
			if (temp_span2 < best_span2)
			{
				best_seq2 = temp_seq2;
				best_span2 = temp_span2;
			}
		}
		else
		{
			if ((static_cast<double>(rand()) / RAND_MAX) <= exp(-(temp_span2 - Span2) / Temperature))//模拟退火
			{
				Seq2 = temp_seq2;
				Span2 = temp_span2;
			}
			else
			{
				temp_seq2 = Seq2;
				temp_span2 = Span2;
			}
		}

		if (best_span > best_span1 || best_span > best_span2)
		{
			if (best_span1 < best_span2)
			{
				best_span = best_span1;
				best_seq = best_seq1;
			}
			else
			{
				best_span = best_span2;
				best_seq = best_seq2;
			}
			fail_count = 0;
		}
		else
		{
			fail_count++;
		}

		int s = 0;
		int Crossover_Threshold = 2200 / pJob;
		if (fail_count == Crossover_Threshold)
		{
			fail_count = 0;
			Crossover(temp_seq1, temp_seq2);

			iter_crosscnt++;
		}

		++iterator_count;
	}
	std::cout << "(" << setw(2) << iterator_count <<"+"<< iter_crosscnt << ")";
	BestSeq = best_seq;
	return best_span;
}


int VIG(int Ins, int TimeLimit)
{
	long Time = GetTickCount();
	int d = 3; //d的取值范围3或4

	vector<int> pJobSeqbest1(pJob, 0);
	vector<int> pJobSeqbest2(pJob, 0);
	vector<int> BestSeq(pJob, 0);
	int bestValuesofar1;
	int bestValuesofar2;
	int bestValue;

	int totalPtime = 0;
	for (int j = 0; j < pJob; ++j)
	{
		for (int k = 0; k < pStage; ++k)
		{
			totalPtime += pTime[k][j];
		}
	}

	double Temperature = (totalPtime / 10.0 * pJob * pStage) * 0.50;

	bestValuesofar1 = NEH_WYT(pJobSeqbest1); //初始化

	GetEN_CON_MME(pJobSeqbest1,pJob);      //MME前要对RecordJobBlockTime初始化
	bestValuesofar2 = MME(pJobSeqbest2);

	bestValue = min(bestValuesofar1, bestValuesofar2);
	bestValue = Evolve_WYT(BestSeq, bestValue, pJobSeqbest1, bestValuesofar1, pJobSeqbest2, bestValuesofar2, d, TimeLimit, Temperature);

	long endTime = GetTickCount();
	double runtime = (endTime - Time) * 1.0 / 1000;
	cout << setw(3) << bestValue << "   " << setprecision(2) << fixed << runtime << "s";
	return bestValue;
}

