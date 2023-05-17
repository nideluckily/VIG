#pragma once
#include <numeric>
//NEH.h

void NEHInit(vector <int>& chrom) //the largest overall processing time
{
	Pair* ch = new Pair[pJob];
	for (int j = 0; j < pJob; j++)
	{
		ch[j].dim = j;
		ch[j].value = 0;
		for (int k = 0; k < pStage; k++)
		{
			ch[j].value += pTime[k][j];
		}
	}

	sort(ch, ch + pJob, PairGreater()); //按每个工件各自的总加工时间降序排列
	for (int j = 0; j < pJob; j++)
		chrom[j] = ch[j].dim;
	delete[]ch;
}
void SPT(vector <int>& chrom) //the largest overall processing time
{
	Pair* ch = new Pair[pJob];
	for (int j = 0; j < pJob; j++)
	{
		ch[j].dim = j;
		ch[j].value = 0;
		for (int k = 0; k < pStage; k++)
		{
			ch[j].value += pTime[k][j];
		}
	}

	sort(ch, ch + pJob, PairGreater()); //按每个工件各自的总加工时间降序排列
	for (int j = 0; j < pJob; j++)
		chrom[j] = ch[j].dim;
	delete[]ch;
}
int NEHInsert(vector <int> Oldchrom, vector <int>& Newchrom)
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

			int tempTF = GetEN_CON(PartSeq, h + 1);

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
int NEH(vector <int>& Seq)
{
	vector <int> TempSeq(pJob);
	NEHInit(TempSeq);
	int TF = NEHInsert(TempSeq, Seq);
	return TF;
}

int NEHInsert_WYT(const vector<int>& Oldchrom, vector <int>& Newchrom)
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
			{
				PartSeq[j] = Newchrom[j];
			}

			PartSeq[j] = tempch;
			for (j++; j <= h; j++)
			{
				PartSeq[j] = Newchrom[j - 1];
			}

			int tempTF = GetEN_CON_WYT(PartSeq, h + 1);

			if (tempTF < min)
			{
				pt = i;
				min = tempTF;
			}
		}

		// insert ch in position pt
		for (i = h; i > pt; i--)
		{
			Newchrom[i] = Newchrom[i - 1];
		}
		Newchrom[pt] = tempch;
	}
	return min;
}
int NEH_WYT(vector <int>& Seq)
{
	vector <int> TempSeq(pJob);
	iota(begin(TempSeq), end(TempSeq), 0);
	vector<int> total_ptime_of_job(pJob, 0);
	for (int j = 0; j < pJob; j++)
	{
		for (int k = 0; k < pStage; k++)
		{
			total_ptime_of_job[j] += pTime[k][j];
		}
	}
	sort(begin(TempSeq), end(TempSeq), [&](int a, int b) { return total_ptime_of_job[a] > total_ptime_of_job[b]; });

	int TF = NEHInsert_WYT(TempSeq, Seq);
	return TF;
}
