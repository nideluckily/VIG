#pragma once
#include <cassert>
#include <iomanip>
#include "HFSP.h"
#include "service.h"
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <set>
#include<fstream>
#include<sstream>

using namespace std;


class  Bird
{
public:
	vector<int> pJobSeq;   //job sequence
	int pAge;               //bird age
	int pTF;               //Total flow-time
	//类成员函数声明
	Bird();                 //重载无参构造函数
	Bird(const Bird& obj);  //重载copy构造函数
	void AgePlus();         //年龄加一
	Bird& operator= (const Bird& obj);  //赋值操作符
	void fprintfBird(FILE* p1);          //打印个体信息
	void LeaderUpdateAndCreateSL(multiset<Bird, less<Bird>>& LeftShare, multiset<Bird, less<Bird>>& RightShare, int nb, int nbs, int nSelect, double ap); //Leader进化
	void BirdsUpdateAndReFillSL(multiset<Bird, less<Bird>>& ShareList, int nb, int nbs, int nSelect, double ap);  //Birds进化
	int BirdLeapingMechanism(const int limit, int& minForLeap);
};


FILE* p;

//1-insertion 2-swap 3-pairwise
int LSelect = 1;   //insertion
int RSelect = 3;   //pairwise

double InitialP = 0.99;
double FinalP = 0.10;


void Bird::AgePlus()
{
	pAge++;
}

Bird::Bird()
{
	pJobSeq.resize(pJob);
	for (int i = 0; i < pJob; i++)
	{
		pJobSeq[i] = i;
	}
	random_shuffle(pJobSeq.begin(), pJobSeq.end());

	pTF = GetEN_CON(pJobSeq, pJob);
	pAge = 0;

}

Bird::Bird(const Bird& obj)
{
	this->pJobSeq = obj.pJobSeq;
	this->pAge = obj.pAge;
	this->pTF = obj.pTF;
}

Bird& Bird::operator= (const Bird& obj)
{
	this->pJobSeq = obj.pJobSeq;
	this->pAge = obj.pAge;
	this->pTF = obj.pTF;

	return *this;
}

void Bird::fprintfBird(FILE* p1)
{
	fprintf(p1, "工件序列：\n");
	for (int i = 0; i < pJob; i++)
	{
		fprintf(p1, "%d ", this->pJobSeq[i]);
	}
	fprintf(p1, "个体年龄: %d", this->pAge);
	fprintf(p1, "个体目标值: %d", this->pTF);
}

void Bird::LeaderUpdateAndCreateSL(multiset<Bird, less<Bird>>& LeftShare, multiset<Bird, less<Bird>>& RightShare, int nb, int nbs, int nSelect, double ap)
{

	multiset<Bird, less<Bird>> LeaderNeighbors;

	//generate neighbors
	for (int i = 0; i < nb; i++)
	{
		Bird tempBird = *this;
		tempBird.pAge = 0;

		int a = rand() % pJob;
		int b = rand() % pJob;
		while (a == b)
		{
			a = rand() % pJob;
			b = rand() % pJob;
		}
		GenerateNeighbors(nSelect, tempBird.pJobSeq, a, b);
		tempBird.pTF = GetEN_CON(tempBird.pJobSeq, pJob);
		LeaderNeighbors.insert(tempBird);
	}

	//create SL
	multiset<Bird, less<Bird>>::iterator it = LeaderNeighbors.begin();
	if ((it->pTF) < (this->pTF))
	{
		*this = *it;
		LeaderNeighbors.erase(it);
		it = LeaderNeighbors.begin();
		for (int i = 0; i < nbs; i++)
		{
			LeftShare.insert(*it);
			it++;
			RightShare.insert(*it);
			it++;
		}
	}
	else
	{
		this->pAge++;
		for (int i = 0; i < nbs; i++)
		{
			LeftShare.insert(*it);
			it++;
			RightShare.insert(*it);
			it++;
		}
	}
}

void Bird::BirdsUpdateAndReFillSL(multiset<Bird, less<Bird>>& ShareList, int nb, int nbs, int nSelect, double ap)
{
	multiset<Bird, less<Bird>> BirdNeighbors;

	//generate neighbors
	for (int i = 0; i < (nb - nbs); i++)
	{
		Bird tempBird = *this;
		tempBird.pAge = 0;

		int a = rand() % pJob;
		int b = rand() % pJob;
		while (a == b)
		{
			a = rand() % pJob;
			b = rand() % pJob;
		}

		//swap(tempBird.pJobSeq[a], tempBird.pJobSeq[b]); //pairwise swap
		GenerateNeighbors(nSelect, tempBird.pJobSeq, a, b);
		tempBird.pTF = GetEN_CON(tempBird.pJobSeq, pJob);
		BirdNeighbors.insert(tempBird);
	}

	//Update and Refill

	multiset<Bird, less<Bird>>::iterator it1 = BirdNeighbors.begin();
	multiset<Bird, less<Bird>>::iterator it2 = ShareList.begin();


	if ((it1->pTF < this->pTF) && (it2->pTF < this->pTF))  //better && better
	{
		if ((it1->pTF) < (it2->pTF))
		{
			*this = *it1;
		}
		else
		{
			double p = rand() * 1.0 / RAND_MAX;

			if (p <= ap)
			{
				*this = *it2;
				ShareList.erase(it2);
				ShareList.insert(*it1);
			}
			else
			{
				*this = *it1;
			}
		}
	}
	else if ((it1->pTF < this->pTF) && (it2->pTF >= this->pTF)) //better && worse
	{
		*this = *it1;
	}
	else if ((it1->pTF >= this->pTF) && (it2->pTF < this->pTF)) //worse && better
	{
		double p = rand() * 1.0 / RAND_MAX;
		if (p <= ap)
		{
			*this = *it2;
			ShareList.erase(it2);
			ShareList.insert(*it1);
		}
		else
		{
			this->pAge++;
		}
	}
	else if (it1->pTF >= this->pTF && it2->pTF >= this->pTF) //worse &&worse
	{
		this->pAge++;
	}

}

int Bird::BirdLeapingMechanism(const int limit, int& minForLeap)
{
	if ((this->pAge) > limit)
	{
		if ((this->pTF) < minForLeap)
		{
			minForLeap = this->pTF;
		}
		vector<int> tempSeq;
		tempSeq = pJobSeq;
		int h = rand() % (pJob / 2) + 1;
		GloverOKL(tempSeq, pJobSeq, h);

		//Local search
		int tempTF = GetEN_CON(pJobSeq, pJob);
		LocalSearchInsert(pJobSeq, tempTF);
		//LocalSearchPairwise(pJobSeq, tempTF);
		//年龄和函数值要进行重置
		this->pAge = 0;
		this->pTF = tempTF;
	}
	return minForLeap;
}

//----------------------------------------------------------
//全局函数声明
ostream& operator<<(ostream& out, const Bird& a);   //重载左移操作符
bool operator==(const Bird& a, const Bird& b);     //重载==操作符
bool operator> (const Bird& a, const Bird& b);     //重载>号运算符
bool operator< (const Bird& a, const Bird& b);     //重载>号运算符
void InitPop(vector<Bird>& LeftBirds, vector<Bird>& RightBirds, const int ps); //初始化群体
void CheckAndOutPop(Bird& Leader, vector<Bird>& LeftBirds, vector<Bird>& RightBirds); //输出群体
bool BirdCompLess(Bird a, Bird b);    //函数指针参数
void outputNeighbors(multiset < Bird, less<Bird>>& Neighbors);  //输出产生的邻居
void ModifyFormation(vector<Bird>& LeftBirds, vector<Bird>& RighrBirds, const int nt);  //更改位置以及左右 
void ModifyBirdsOrder(Bird& Leader, vector<Bird>& LeftBirds, vector<Bird>& RightBirds, bool& LeftSelected, bool& RightSelected);  //tour结束后变更Leader
void EvovlePop(Bird& Leader, vector<Bird>& LeftBirds, vector<Bird>& RightBirds, const int nb, const int nbs, const int nt, int N, int limit, int& minForLeap, int TimeLimit);  //MBO进化
void OutputBest(const Bird& Leader, const vector<Bird>& LeftBirds, const vector<Bird>& RightBirds, const int ins, int& minForLeap);  //输出最优值
void MBO(const int ps, const int nb, const int nbs, const int nt, int TimeLimit);  //声明MBO算法

//全局函数实现
ostream& operator<< (ostream& out, const Bird& a)
{
	out << "工件序列:" << endl;
	for (int i = 0; i < pJob; i++)
	{
		out << a.pJobSeq[i] << " ";
	}
	out << endl;
	out << "个体年龄：" << a.pAge << endl;
	out << "个体目标函数值：" << a.pTF << endl;

	return out;

}

bool operator==(const Bird& a, const Bird& b)
{
	if (a.pTF == b.pTF)
	{
		return true;
	}
	else
		return false;
}

bool operator> (const Bird& a, const Bird& b)
{
	if (a.pTF > b.pTF)
	{
		return true;
	}
	else
		return false;
}

bool operator< (const Bird& a, const Bird& b)
{
	if (a.pTF < b.pTF)
	{
		return true;
	}
	else
		return false;
}


void InitPop(vector<Bird>& LeftBirds, vector<Bird>& RightBirds, const int ps)
{
	LeftBirds.resize((ps - 1) / 2);

	RightBirds.resize((ps - 1) / 2);
}

void CheckAndOutPop(Bird& Leader, vector<Bird>& LeftBirds, vector<Bird>& RightBirds)
{
	FILE* p1;

	p1 = fopen("E:\\test\\vs2013\\MBO_HFS03\\data\\popInf.txt", "w");

	if (p1 == NULL)
	{
		cout << "打开文件popInf.txt失败" << endl;
	}
	else
	{
		fprintf(p1, "Leader: \n");
		Leader.fprintfBird(p1);

		for (int i = 0; i < LeftBirds.size(); i++)
		{
			fprintf(p1, " \n LeftBird[%d]: \n", i);
			LeftBirds[i].fprintfBird(p1);
		}

		for (int i = 0; i < RightBirds.size(); i++)
		{
			fprintf(p1, "\n RightBirds[%d]: \n", i);
			RightBirds[i].fprintfBird(p1);
		}
	}

	fclose(p1);

	cout << "Leader:" << endl;
	cout << Leader << endl;

	cout << "LeftBirds:" << endl;
	for (int i = 0; i < LeftBirds.size(); i++)
	{
		cout << LeftBirds[i] << endl;
	}
	cout << "RightBirds:" << endl;
	for (int i = 0; i < RightBirds.size(); i++)
	{
		cout << RightBirds[i] << endl;
	}
}



bool BirdCompLess(Bird a, Bird b)
{
	return a.pTF < b.pTF;
}


void outputNeighbors(multiset < Bird, less<Bird>>& Neighbors)
{
	multiset < Bird, less<Bird>>::iterator it;
	for (it = Neighbors.begin(); it != Neighbors.end(); it++)
	{
		cout << *it << endl;
	}
}


void ModifyFormation(vector<Bird>& LeftBirds, vector<Bird>& RightBirds, const int nt)
{

	int tmp = LeftBirds.size();

	//LeftBirds调整次序
	for (int i = 0; i < nt; i++)
	{
		int a = rand() % tmp;
		int b = rand() % tmp;
		while (a == b)
		{
			a = rand() % tmp;
			b = rand() % tmp;
		}

		if ((a > b) && (LeftBirds[a] < LeftBirds[b]))
		{
			Bird tempBird;
			tempBird = LeftBirds[a];
			LeftBirds[a] = LeftBirds[b];
			LeftBirds[b] = tempBird;
		}
		if ((b > a) && LeftBirds[b] < LeftBirds[a])
		{
			Bird tempBird;
			tempBird = LeftBirds[a];
			LeftBirds[a] = LeftBirds[b];
			LeftBirds[b] = tempBird;
		}
	}

	//RightBirds调整次序
	for (int i = 0; i < nt; i++)
	{
		int a = rand() % tmp;
		int b = rand() % tmp;
		while (a == b)
		{
			a = rand() % tmp;
			b = rand() % tmp;
		}

		if ((a > b) && (RightBirds[a] < RightBirds[b]))
		{
			Bird tempBird;
			tempBird = RightBirds[a];
			RightBirds[a] = RightBirds[b];
			RightBirds[b] = tempBird;
		}
		if ((b > a) && RightBirds[b] < RightBirds[a])
		{
			Bird tempBird;
			tempBird = RightBirds[a];
			RightBirds[a] = RightBirds[b];
			RightBirds[b] = tempBird;
		}
	}

	//LeftBirds和RightBirds的interaction
	for (int i = 0; i < nt; i++)
	{
		int a = rand() % tmp;
		//modification 1

		Bird tempBird;
		tempBird = LeftBirds[a];
		LeftBirds[a] = RightBirds[a];
		RightBirds[a] = tempBird;

		Bird tempBird2;
		TpCrossver(LeftBirds[a].pJobSeq, RightBirds[a].pJobSeq, tempBird2.pJobSeq);
		tempBird2.pTF = GetEN_CON(tempBird2.pJobSeq, pJob);

		if (tempBird2.pTF <= LeftBirds[a].pTF || tempBird2.pTF <= RightBirds[a].pTF)
		{
			if (LeftBirds[a].pTF < RightBirds[a].pTF)
			{
				RightBirds[a] = tempBird2;
			}
			else
			{
				LeftBirds[a] = tempBird2;
			}
		}

	}
}

void ModifyBirdsOrder(Bird& Leader, vector<Bird>& LeftBirds, vector<Bird>& RightBirds, bool& LeftSelected, bool& RightSelected)
{
	if (LeftSelected)
	{
		int i;
		Bird tempBird = Leader;
		Leader = LeftBirds[0];
		for (i = 1; i < LeftBirds.size(); i++)
		{
			LeftBirds[i - 1] = LeftBirds[i];
		}

		LeftBirds[i - 1] = tempBird;
	}
	if (RightSelected)
	{
		int i;
		Bird tempBird = Leader;
		Leader = LeftBirds[0];
		for (i = 1; i < RightBirds.size(); i++)
		{
			RightBirds[i - 1] = RightBirds[i];
		}

		RightBirds[i - 1] = tempBird;
	}
}

void EvovlePop(Bird& Leader, vector<Bird>& LeftBirds, vector<Bird>& RightBirds, const int nb, const int nbs, const int nt, int N, int limit, int& minForLeap, int TimeLimit)
{
	bool LeftSelected = true;
	bool RightSelected = false;
	long time1 = GetTickCount64();

	int totalTour = 0;  //总的tour次数

	while ((GetTickCount64() - time1) < TimeLimit)
	{
		for (int tour = 0; tour < nt; tour++) //nt=10
		{
			totalTour++;
			double ap = InitialP - totalTour * (InitialP - FinalP) / N;
			if (ap < FinalP)
			{
				ap = FinalP;
			}

			//initialize LeftShare and RightShare
			multiset<Bird, less<Bird>> LeftShare;
			multiset<Bird, less<Bird>> RightShare;
			//Evolve Leader
			if (LeftSelected == true)
			{
				Leader.LeaderUpdateAndCreateSL(LeftShare, RightShare, nb, nbs, RSelect, ap);
			}
			else
			{
				Leader.LeaderUpdateAndCreateSL(LeftShare, RightShare, nb, nbs, LSelect, ap);
			}

			Leader.BirdLeapingMechanism(limit, minForLeap);
			//Evolve LeftBirds and RightBirds
			for (int i = 0; i < LeftBirds.size(); i++)
			{
				LeftBirds[i].BirdsUpdateAndReFillSL(LeftShare, nb, nbs, LSelect, ap);
				LeftBirds[i].BirdLeapingMechanism(limit, minForLeap);
				RightBirds[i].BirdsUpdateAndReFillSL(RightShare, nb, nbs, RSelect, ap);
				RightBirds[i].BirdLeapingMechanism(limit, minForLeap);
			}
			//cout << "第" << tour << "次tour结束"<< endl;
		}

		//modify leader of the flock
		ModifyBirdsOrder(Leader, LeftBirds, RightBirds, LeftSelected, RightSelected);

		//modify order of the flock
		ModifyFormation(LeftBirds, RightBirds, nt);

		if (LeftSelected == true)
		{
			LeftSelected = false;
		}
		else
		{
			LeftSelected = true;
		}
		if (RightSelected == true)
		{
			RightSelected = false;
		}
		else
		{
			RightSelected = true;
		}
	}
}

void EvovlePop_Curve(Bird& Leader, vector<Bird>& LeftBirds, vector<Bird>& RightBirds, const int nb, const int nbs, const int nt, int N, int limit, int& minForLeap, int TimeLimit,int ins,long Time, ofstream& ofile)
{
	bool LeftSelected = true;
	bool RightSelected = false;
	long time1 = GetTickCount64();

	int totalTour = 0;  //总的tour次数

	while ((GetTickCount64() - time1) < TimeLimit)
	{
		for (int tour = 0; tour < nt; tour++) //nt=10
		{
			totalTour++;
			double ap = InitialP - totalTour * (InitialP - FinalP) / N;
			if (ap < FinalP)
			{
				ap = FinalP;
			}

			//initialize LeftShare and RightShare
			multiset<Bird, less<Bird>> LeftShare;
			multiset<Bird, less<Bird>> RightShare;
			//Evolve Leader
			if (LeftSelected == true)
			{
				Leader.LeaderUpdateAndCreateSL(LeftShare, RightShare, nb, nbs, RSelect, ap);
			}
			else
			{
				Leader.LeaderUpdateAndCreateSL(LeftShare, RightShare, nb, nbs, LSelect, ap);
			}

			int bestValuesofar =  Leader.BirdLeapingMechanism(limit, minForLeap);

			//Evolve LeftBirds and RightBirds
			for (int i = 0; i < LeftBirds.size(); i++)
			{
				LeftBirds[i].BirdsUpdateAndReFillSL(LeftShare, nb, nbs, LSelect, ap);
				LeftBirds[i].BirdLeapingMechanism(limit, minForLeap);
				RightBirds[i].BirdsUpdateAndReFillSL(RightShare, nb, nbs, RSelect, ap);
				RightBirds[i].BirdLeapingMechanism(limit, minForLeap);
			}
			//cout << "第" << tour << "次tour结束"<< endl;
		}

		//modify leader of the flock
		ModifyBirdsOrder(Leader, LeftBirds, RightBirds, LeftSelected, RightSelected);

		//modify order of the flock
		ModifyFormation(LeftBirds, RightBirds, nt);

		if (LeftSelected == true)
		{
			LeftSelected = false;
		}
		else
		{
			LeftSelected = true;
		}
		if (RightSelected == true)
		{
			RightSelected = false;
		}
		else
		{
			RightSelected = true;
		}
		//进化后的值：
		long Localtime = GetTickCount() - Time;
		ofile << Localtime << "\t" << minForLeap << "\t" << endl;
	}
	ofile.close();
}

int OutputBest(const Bird& Leader, const vector<Bird>& LeftBirds, const vector<Bird>& RightBirds, const int ins, int& minForLeap, double runtime)
{
	int LeaderTF = Leader.pTF;

	int minBirdsTF = LeaderTF;
	for (int i = 0; i < LeftBirds.size(); i++)
	{
		if (LeftBirds[i].pTF < minBirdsTF)
		{
			minBirdsTF = LeftBirds[i].pTF;
		}
	}
	for (int i = 0; i < RightBirds.size(); i++)
	{
		if (RightBirds[i].pTF < minBirdsTF)
		{
			minBirdsTF = RightBirds[i].pTF;
		}
	}

	if (minForLeap < minBirdsTF)
	{
		minBirdsTF = minForLeap;
	}

	cout << setw(3) << minBirdsTF << "   " << setprecision(2) << fixed << runtime << "s";
	return minBirdsTF;

}



int MBO(const int ps, const int nb, const int nbs, const int nt, const int ins, int N, int limit, int TimeLimit)  //在这里面建立种群
{
	long Time = GetTickCount();
	int minForLeap = INT_MAX;

	Bird Leader;
	vector<Bird> LeftBirds;
	vector<Bird> RightBirds;

	InitPop(LeftBirds, RightBirds, ps);

	EvovlePop(Leader, LeftBirds, RightBirds, nb, nbs, nt, N, limit, minForLeap, TimeLimit);
	long endTime = GetTickCount();
	double runtime = (endTime - Time) * 1.0 / 1000;

	int bestValuesofar = OutputBest(Leader, LeftBirds, RightBirds, ins, minForLeap,runtime);
	return bestValuesofar;

}

int MBO_evolveCurve(const int ps, const int nb, const int nbs, const int nt, const int ins, int N, int limit, int TimeLimit)  //在这里面建立种群
{
	long Time = GetTickCount();
	int minForLeap = INT_MAX;

	Bird Leader;
	vector<Bird> LeftBirds;
	vector<Bird> RightBirds;

	InitPop(LeftBirds, RightBirds, ps);

	//进化前
	ostringstream str;
	str << "MBO_" << "EvolveCurve_" << ins + 1 << ".txt";
	ofstream ofile;
	ofile.open(str.str());
	//进化
	EvovlePop_Curve(Leader, LeftBirds, RightBirds, nb, nbs, nt, N, limit, minForLeap, TimeLimit,ins,Time,ofile);
	long endTime = GetTickCount();
	double runtime = (endTime - Time) * 1.0 / 1000;

	int bestValuesofar = OutputBest(Leader, LeftBirds, RightBirds, ins, minForLeap, runtime);
	return bestValuesofar;

}
