#pragma once
#include<iostream>
#include<Windows.h>
#include "HFSP.h"


void RIS(vector<int>& Seq, vector<int> BestSeq)
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
		if (pos != pJob - 1) {
			for (int i = pos; i < pJob - 1; ++i)
			{
				tempSeq[i] = tempSeq[i + 1];

			}
		}

		tempSeq[pJob - 1] = -1;

		vector<int> tempSeq2(pJob, 0);
		tempSeq2 = tempSeq;
		for (int j = 0; j < pJob; ++j)
		{
			if (j == pJob - 1)
			{
				tempSeq2[j] = tempValue;
			}
			else {
				for (int i = pJob - 2; i >= j; i--)
				{
					tempSeq2[i + 1] = tempSeq2[i];
				}
				tempSeq2[j] = tempValue;
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

void InsertGreedy(vector<int>& Seq, vector<int> BestSeq)
{
	vector<int> tempSeq(pJob, 0);
	tempSeq = Seq;

	int bestValue = GetEN_CON(BestSeq, pJob);
	//int Iter = 1;


	//bestvalueSeq = BestSeq;

	/*while (Iter <= pJob)
	{*/
	vector<int> bestvalueSeq(pJob, -1);

	for (int k = 0; k < pJob; ++k)
	{
		int tempValue;
		tempValue = tempSeq[k];
		if (k != pJob - 1)
		{
			for (int j = k + 1; j < pJob; ++j)
			{
				tempSeq[j - 1] = tempSeq[j];
			}
		}
		tempSeq[pJob - 1] = -1;

		vector<int> tempSeq2(pJob, 0);
		tempSeq2 = tempSeq;
		for (int j = 0; j < pJob; ++j)
		{
			if (j == pJob - 1)
			{
				tempSeq2[j] = tempValue;
			}
			else {
				for (int i = pJob - 2; i >= j; i--)
				{
					tempSeq2[i + 1] = tempSeq2[i];
				}
				tempSeq2[j] = tempValue;
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