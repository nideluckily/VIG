
#pragma once
#include<iostream>
#include<Windows.h>
#include "HFSP.h"

void MM(vector <int>& Seq, int alpha) // MM heuristic by Ronconi, 2004,IJPE A note on constructive heuristics for the flowshop problem wiht blocking.
{
	int i, j;
	vector <int> ch1(pJob);
	vector <bool> bSelected(pJob);

	for (i = 0; i < pJob; i++)
		bSelected[i] = false;

	// get Total process time of each job
	for (i = 0; i < pJob; i++)
	{
		ch1[i] = 0;
		for (j = 0; j < pStage; j++)
			ch1[i] += pTime[j][i];
	}

	// select the first job;
	int MinTPTime = INT_MAX;
	int Sjob = 0;
	for (i = 0; i < pJob; i++)
	{
		if (MinTPTime > ch1[i])
		{
			MinTPTime = ch1[i];      //Mintpime存放ch1的最小值
			Sjob = i;                 //SJOB 存放最小值的下标
		}
	}

	// set the first job of chrom;
	Seq[0] = Sjob;
	bSelected[Sjob] = true;

	//select the last job;        
	MinTPTime = INT_MAX;
	for (i = 0; i < pJob; i++)
	{
		if (bSelected[i]) continue;
		if (MinTPTime > ch1[i])         //选择次大的值
		{
			MinTPTime = ch1[i];
			Sjob = i;
		}
	}

	// set the last job of chrom
	Seq[pJob - 1] = Sjob;
	bSelected[Sjob] = true;

	// get the total ;
	vector<int> Block(pJob);
	for (i = 0; i < pJob; i++)
	{
		Block[i] = 0;
		for (j = 0; j < pStage; j++)
		{
			Block[i] += RecordJobBlockTime[j][i];         //w:这有问题
		}
	}

	// set other jobs for chrom
	for (i = 1; i < pJob - 1; i++)
	{
		int MinBlock = INT_MAX;
		for (j = 0; j < pJob; j++)
		{
			if (bSelected[j]) continue;
			if (MinBlock > alpha * Block[j] + (100 - alpha) * ch1[j])
			{
				MinBlock = alpha * Block[j] + (100 - alpha) * ch1[j];
				Sjob = j;
			}
		}
		Seq[i] = Sjob;
		bSelected[Sjob] = true;
	}

	//	CheckChrom(chrom);
}

int NEH_Insert(vector <int> Oldchrom, vector <int>& Newchrom)
{
	int h = 1, min, i, j, pt;
	int tempch;
	vector <int> PartSeq(pJob, 0);

	Newchrom[0] = Oldchrom[0];

	for (h = 1; h < pJob; h++)
	{
		tempch = Oldchrom[h];
		min = INT_MAX;
		pt = -1;

		for (i = 0; i <= h; i++)
		{
			for (j = 0; j < i; j++)
				PartSeq[j] = Newchrom[j];

			PartSeq[j] = tempch;
			for (j++; j <= h; j++)
				PartSeq[j] = Newchrom[j - 1];

			//int tempTF = GetEN_CON(PartSeq, h + 1);
			int tempTF = GetEN_CON_backward(PartSeq, h + 1);

			if (tempTF < min)
			{
				pt = i;
				min = tempTF;
			}
		}

		// insert ch in position pt
		for (i = h; i > pt; i--)
			Newchrom[i] = Newchrom[i - 1];
		Newchrom[pt] = tempch;
	}
	return min;
}



int MME(vector <int>& Seq)
{
	vector <int> TempSeq(pJob);
	MM(TempSeq, 80);
	int minvalue = NEH_Insert(TempSeq, Seq);
	return minvalue;
}