
#pragma once
#include"HFSP.h"

//--------------函数声明--------------------


void GloverOKL(vector <int> p1, vector <int>& c, int h); //Glover operator
void GenerateNeighbors(int select, vector<int>& tempJobSeq, int a, int b);  //Generate neighbors
void LocalSearchInsert(vector<int>& JobSeq, int& JobTF);  //Local Search + Insert
void LocalSearchPairwise(vector<int>& JobSeq, int& JobTF);  //Local Search + Pairwise
void TpCrossver(vector<int> LeftOne, vector<int> RightOne, vector<int>& ResultOne); //two-point crossver
void TpSOCrossver(vector <int> LeftOne, vector <int> RightOne, vector <int>& ResultOne);   // two-point similar order crossover

//---------------函数实现--------------------
//Glover operator
void GloverOKL(vector <int> p1, vector <int>& c, int h)
{
	c.clear();
	for (int spt = 1; spt <= h; spt++)
	{
		for (int pt = spt; pt <= p1.size(); pt += h)
			c.push_back(p1[pt - 1]);
	}
}

void GenerateNeighbors(int select, vector<int>& tempJobSeq, int a, int b)
{
	if (select == 1)  //insertion
	{
		if (b > a)
		{
			int tempValue = tempJobSeq[b];
			for (int i = b; i > a; i--)
			{
				tempJobSeq[i] = tempJobSeq[i - 1];
			}
			tempJobSeq[a] = tempValue;
		}
		else
		{
			int tempValue = tempJobSeq[b];
			for (int i = b; i < a; i++)
			{
				tempJobSeq[i] = tempJobSeq[i + 1];
			}
			tempJobSeq[a] = tempValue;
		}

	}
	if (select == 2)  //swap
	{
		if (a == (tempJobSeq.size() - 1))
		{
			swap(tempJobSeq[a], tempJobSeq[0]);
		}
		else
		{
			swap(tempJobSeq[a], tempJobSeq[a + 1]);
		}
	}
	if (select == 3)  //pariwise
	{
		swap(tempJobSeq[a], tempJobSeq[b]);
	}

	if (select == 4) //insertion greedy 
	{
		int bestValue = INT_MAX;
		vector<int> temp3JobSeq = tempJobSeq;  //现将tempJobSeq 保存下来

		for (int c = 0; c < pJob; c++)
		{
			vector<int> temp2JobSeq = temp3JobSeq;

			if (b > c)
			{
				int tempValue = temp2JobSeq[b];
				for (int i = b; i > c; i--)
				{
					temp2JobSeq[i] = temp2JobSeq[i - 1];
				}
				temp2JobSeq[c] = tempValue;
			}
			else
			{
				int tempValue = temp2JobSeq[b];
				for (int i = b; i < c; i++)
				{
					temp2JobSeq[i] = temp2JobSeq[i + 1];
				}
				temp2JobSeq[c] = tempValue;
			}

			if (GetEN_CON(temp2JobSeq, pJob) < bestValue)
			{
				bestValue = GetEN_CON(temp2JobSeq, pJob);
				tempJobSeq = temp2JobSeq;
			}
		}
	}

	if (select == 5) //pairwise greedy
	{
		int bestValue = INT_MAX;
		vector<int> temp3JobSeq = tempJobSeq;  //现将tempJobSeq 保存下来

		for (int i = 0; i < pJob; i++)
		{
			vector<int> temp2JobSeq = temp3JobSeq;

			swap(temp2JobSeq[i], temp2JobSeq[b]);

			if (GetEN_CON(temp2JobSeq, pJob) < bestValue)
			{
				bestValue = GetEN_CON(temp2JobSeq, pJob);
				tempJobSeq = temp2JobSeq;
			}
		}
	}

	if (select == 6) //insertion best
	{
		int bestValue = INT_MAX;
		vector<int> temp3JobSeq = tempJobSeq;
		for (int i = 0; i < pJob; i++)
		{
			vector<int> temp2JobSeq = temp3JobSeq;
			a = rand() % pJob;
			b = rand() % pJob;
			while (a == b)
			{
				a = rand() % pJob;
				b = rand() % pJob;
			}

			if (b > a)
			{
				int tempValue = temp2JobSeq[b];
				for (int i = b; i > a; i--)
				{
					temp2JobSeq[i] = temp2JobSeq[i - 1];
				}
				temp2JobSeq[a] = tempValue;
			}
			else
			{
				int tempValue = temp2JobSeq[b];
				for (int i = b; i < a; i++)
				{
					temp2JobSeq[i] = temp2JobSeq[i + 1];
				}
				temp2JobSeq[a] = tempValue;
			}

			if (GetEN_CON(temp2JobSeq, pJob) < bestValue)
			{
				bestValue = GetEN_CON(temp2JobSeq, pJob);
				tempJobSeq = temp2JobSeq;
			}
		}
	}

	if (select == 7) //pairwise best
	{
		int bestValue = INT_MAX;
		vector<int> temp3JobSeq = tempJobSeq;
		for (int i = 0; i < pJob; i++)
		{
			vector<int> temp2JobSeq = temp3JobSeq;
			a = rand() % pJob;
			b = rand() % pJob;

			swap(temp2JobSeq[a], temp2JobSeq[b]);

			if (GetEN_CON(temp2JobSeq, pJob) < bestValue)
			{
				bestValue = GetEN_CON(temp2JobSeq, pJob);
				tempJobSeq = temp2JobSeq;
			}
		}
	}
}


void LocalSearchInsert(vector<int>& JobSeq, int& JobTF)
{
	bool improvement = true;
	while (improvement)
	{
		improvement = false;
		vector<int> tempJobSeq = JobSeq;
		int tempJobTF = JobTF;

		for (int k = 0; k < pJob; k++)
		{
			int b;
			do
			{
				b = rand() % pJob;
			} while (b == k);

			if (k > b)
			{
				int tempValue = tempJobSeq[k];
				for (int i = k; i > b; i--)
				{
					tempJobSeq[i] = tempJobSeq[i - 1];
				}
				tempJobSeq[b] = tempValue;
			}
			else
			{
				int tempValue = tempJobSeq[k];
				for (int i = k; i < b; i++)
				{
					tempJobSeq[i] = tempJobSeq[i + 1];
				}
				tempJobSeq[b] = tempValue;
			}
			tempJobTF = GetEN_CON(tempJobSeq, pJob);
			if (tempJobTF < JobTF)
			{
				JobSeq = tempJobSeq;
				JobTF = tempJobTF;
				improvement = true;
				break;
			}
		}
	}
}


void LocalSearchPairwise(vector<int>& JobSeq, int& JobTF)
{
	bool improvement = true;
	while (improvement)
	{
		vector<int> tempJobSeq = JobSeq;
		int tempJobTF = JobTF;

		improvement = false;
		for (int k = 0; k < pJob; k++)
		{
			int b;
			do
			{
				b = rand() % pJob;
			} while (b == k);

			swap(tempJobSeq[b], tempJobSeq[k]); //pairwise
			tempJobTF = GetEN_CON(tempJobSeq, pJob);
			if (tempJobTF < JobTF)
			{
				JobSeq = tempJobSeq;
				JobTF = tempJobTF;
				improvement = true;
				break;
			}
		}
	}
}


void TpCrossver(vector<int> LeftOne, vector<int> RightOne, vector<int>& ResultOne)
{
	int a, b, m;  //交叉点
	int temp;

	for (int i = 0; i < pJob; i++)
	{
		ResultOne[i] = pJob;
	}

	//双点交叉操作  
	do
	{
		a = rand() % pJob;
		b = rand() % pJob;
	} while (a == b);

	if (a > b)   //assure that a<b
	{
		m = a;
		a = b;
		b = m;
	}

	if (a != 0)
	{
		temp = 0;
	}
	else
	{
		temp = b + 1;
	}
	for (int i = a; i <= b; i++)
	{
		ResultOne[i] = LeftOne[i];
	}

	for (int i = 0; i < pJob; i++)
	{
		vector<int>::iterator found = find(ResultOne.begin(), ResultOne.end(), RightOne[i]);
		//find用于查找前两个参数组成范围中与最后一个参数值相同的元素
		if (found == ResultOne.end())
		{
			ResultOne[temp] = RightOne[i];
			temp++;

			if (temp == a)
			{
				temp = b + 1;
			}
		}
	}
}

void TpSOCrossver(vector <int> LeftOne, vector <int> RightOne, vector <int>& ResultOne)
{
	int a, b, m;  //交叉点
	int temp;

	for (int i = 0; i < pJob; i++)
	{
		ResultOne[i] = pJob;
	}

	//similar job order

	for (int i = 0; i < pJob; i++)
	{
		if (LeftOne[i] == RightOne[i])
		{
			ResultOne[i] = LeftOne[i];
		}
	}

	//双点交叉操作  
	do
	{
		a = rand() % pJob;
		b = rand() % pJob;
	} while (a == b);

	if (a > b)   //assure that a<b
	{
		m = a;
		a = b;
		b = m;
	}

	if (a != 0)
	{
		temp = 0;
	}
	else
	{
		temp = b + 1;
	}
	for (int i = a; i <= b; i++)
	{
		ResultOne[i] = LeftOne[i];
	}

	for (int i = 0; i < pJob; i++)
	{
		vector<int>::iterator found = find(ResultOne.begin(), ResultOne.end(), RightOne[i]);
		if (found == ResultOne.end())
		{
			ResultOne[temp] = RightOne[i];
			temp++;

			if (temp == a)
			{
				temp = b + 1;
			}
		}
	}
}
