#pragma once
#include<iostream>
#include<Windows.h>
#include "HFSP.h"


void RSS(vector<int>& Seq, vector<int> BestSeq)
{
	vector<int> tempSeq(pJob, 0);
	tempSeq = Seq;
	vector<int> tempSeqRecord(pJob, 0);
	int tempValue;

	int bestValue = GetEN_CON(BestSeq, pJob);
	//int Iter = 1;
	int pos;


	//bestvalueSeq = BestSeq;

	/*while (Iter <= pJob)
	{*/
	vector<int> bestvalueSeq(pJob, -1);
	for (int k = 0; k < pJob; ++k)
	{

		for (int j = 0; j < pJob; ++j)
		{
			if (BestSeq[k] == tempSeq[j])
			{
				pos = j;
				tempValue = tempSeq[j];
				break;
			}
		}

		vector<int> tempSeq2(pJob, 0);
		tempSeq2 = tempSeq;
		for (int j = 0; j < pJob; ++j)
		{
			if (tempSeq2[j] != tempSeq2[pos])
			{
				int t = tempSeq2[j];
				tempSeq2[j] = tempSeq2[pos];
				tempSeq2[pos] = t;
			}
			else {
				continue;
			}
			int Value = GetEN_CON(tempSeq2, pJob);
			if (Value < bestValue)
			{
				bestValue = Value;
				bestvalueSeq = tempSeq2;
				//Iter = 1;
			}
			//else Iter++;

			tempSeq2 = tempSeq;
		}
		if (bestvalueSeq[0] != -1)
		{
			tempSeq = bestvalueSeq;
			Seq = bestvalueSeq;
		}
		else {

			tempSeq = Seq;
		}

	}

	//}

}

void SwapGreedy(vector<int>& Seq, vector<int> BestSeq)
{
	vector<int> tempSeq(pJob, 0);
	tempSeq = Seq;
	vector<int> tempSeqRecord(pJob, 0);
	int tempValue;

	int bestValue = GetEN_CON(BestSeq, pJob);
	//int Iter = 1;
	int pos;


	//bestvalueSeq = BestSeq;

	/*while (Iter <= pJob)
	{*/
	vector<int> bestvalueSeq(pJob, -1);
	for (int k = 0; k < pJob; ++k)
	{

		/*for (int j = 0; j < pJob; ++j)
		{
			if (BestSeq[k] == tempSeq[j])
			{
				pos = j;
				tempValue = tempSeq[j];
				break;
			}
		}*/

		vector<int> tempSeq2(pJob, 0);
		tempSeq2 = tempSeq;
		for (int j = 0; j < pJob; ++j)
		{
			if (tempSeq2[j] != tempSeq2[k])
			{
				int t = tempSeq2[j];
				tempSeq2[j] = tempSeq2[k];
				tempSeq2[k] = t;
			}
			else {
				continue;
			}
			int Value = GetEN_CON(tempSeq2, pJob);
			if (Value < bestValue)
			{
				bestValue = Value;
				bestvalueSeq = tempSeq2;
				//Iter = 1;
			}
			//else Iter++;

			tempSeq2 = tempSeq;
		}
		if (bestvalueSeq[0] != -1)
		{
			tempSeq = bestvalueSeq;
			Seq = bestvalueSeq;
		}
		else {

			tempSeq = Seq;
		}

	}

}