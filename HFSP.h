#pragma once
#include<vector>
#include<algorithm>
using namespace std;

// -------------for ordering----------
typedef struct
{
	int dim;
	int value;
} Pair;

class PairGreater {
public:
	bool operator () (Pair a, Pair b)    //�������� Ҳ��֮Ϊα����
	{
		return a.value > b.value;
	}
};

class PairLess {
public:
	bool operator () (Pair a, Pair b)
	{
		return a.value < b.value;
	}
};
//-------------end ----------------

//-----------------------
int Jobs[10] = { 20,40,60,80,100 };
int Stages[2] = { 5, 10 };

//-----------------------

int pJob;
int pStage;
int gREF;
vector<int> pMachines;   //number of machines of each stage  [1,5]
vector<vector<int>> pTime;//p(k,j) the processing time of jth job on the kth stage  [1,31]
vector<vector<int>> IdleTime; //IdleTime(i,j) the Idle time of jth machine at stage i
vector<vector<int>> BlockTime; //BlockTime(i,j,k) the Blocking time of kth job on machine j at stage i
vector<vector<int>> RecordJobBlockTime; //RecordJobBlockTime(k,j) record the blocking time of job j at stage k;

vector<vector<int>> STime, CTime;
vector<vector<int>> MIdleTime;   //������ʼ���е�ʱ��

//vector<vector<int>> IdleConsumption; //IdleConsumption(i,j) the idle-energy consumption per unit time of the machine j at stage i
//vector<vector<int>> BlockConsumption; //BlockConsumption(i,j) the  blocking consumption of the machine j at stage i per unit time
//vector<vector<int>> pTimeConsumption; //IdleConsumption(i,j) the  Idle consumption of the machine j at stage i per unit time

//��������
int GetEN_CON(vector<int> Seq, int Len); //get total energy consumption
void GenerateInstances(int InJob, int InStage, int seed);   //generate instances

//����ʵ��
void GenerateInstances(int InJob, int InStage, int seed)
{
	srand(seed);
	pJob = InJob;
	pStage = InStage;

	pMachines.resize(InStage);
	for (int i = 0; i < InStage; i++)
	{
		pMachines[i] = 1 + rand() % 5;  //range of machines [1-5]

	}

	pTime.resize(InStage);
	for (int i = 0; i < InStage; i++)
	{
		pTime[i].resize(InJob);
	}

	for (int i = 0; i < InStage; i++)
	{
		for (int j = 0; j < InJob; j++)
		{
			pTime[i][j] = 1 + rand() % 99;  //[1-99]

		}
	}


	BlockTime.resize(pStage);
	for (int k = 0; k < pStage; k++)
	{
		BlockTime[k].resize(pMachines[k]);
	}


	IdleTime.resize(pStage);
	for (int k = 0; k < pStage; k++)
	{
		IdleTime[k].resize(pMachines[k]);
	}



	RecordJobBlockTime.resize(pStage);
	for (int k = 0; k < pStage; k++)
	{
		RecordJobBlockTime[k].resize(pJob);
	}

}


//�������
int GetEN_CON(vector<int> Seq, int Len)   //��Ŀ�꺯��ֵ For energy consumption
{
	//vector<vector<int>> STime, CTime;
	//vector<vector<int>> mIdleTime;   //������ʼ���е�ʱ��
	//mIdleTime.resize(pStage);
	//for (int k = 0; k < pStage; k++)
	//{
	//	mIdleTime[k].resize(pMachines[k]);
	//}
	////all machines are availale at 0  ��ʵ���Բ���Ҫ Ĭ�϶�Ϊ0
	//for (int k = 0; k < pStage; k++)
	//{
	//	for (int i = 0; i < pMachines[k]; i++)
	//	{
	//		mIdleTime[k][i] = 0;
	//	}
	//}
	//BlockTime = mIdleTime;
	//IdleTime = mIdleTime;
	//STime.resize(pStage);
	//for (int k = 0; k < pStage; k++)
	//{
	//	STime[k].resize(pJob);
	//}
	//for (int i = 0; i < pStage; ++i)
	//{
	//	for (int j = 0; j < pJob; ++j) {
	//		STime[i][j] = 0;
	//	}
	//}
	//CTime = STime;
	for (auto& vec : MIdleTime)
	{
		fill(begin(vec), end(vec), 0);
	}


	int mpt1 = INT_MAX;

	for (int j = 0; j < Len; ++j)
	{
		for (int k = 0; k < pStage; ++k)
		{
			//Select a machine
			int mpt, minIdle = INT_MAX;

			//ѡ�����ȿ��л���
			for (int i = 0; i < pMachines[k]; i++)
			{
				if (MIdleTime[k][i] < minIdle)
				{
					minIdle = MIdleTime[k][i];
					mpt = i;
				}

			}

			//��1�׶λ�����ʼ��ʱ��
			//if (Seq[j] == -1)break;//IG�㷨�õ��ж�

			if (k == 0) { STime[k][Seq[j]] = minIdle; mpt1 = mpt; }

			//�����׶�
			else {
				STime[k][Seq[j]] = max(minIdle, CTime[k - 1][Seq[j]]);

				if (minIdle >= CTime[k - 1][Seq[j]])
				{
					BlockTime[k - 1][mpt1] = minIdle - CTime[k - 1][Seq[j]];
					RecordJobBlockTime[k - 1][Seq[j]] = BlockTime[k - 1][mpt1];
					//BlockTotalConsumption += BlockTime[k - 1][mpt1] * BlockConsumption[k - 1][mpt1];
				}

				CTime[k - 1][Seq[j]] = STime[k][Seq[j]];
				MIdleTime[k - 1][mpt1] = CTime[k - 1][Seq[j]];
				mpt1 = mpt;
			}
			CTime[k][Seq[j]] = STime[k][Seq[j]] + pTime[k][Seq[j]];
			MIdleTime[k][mpt] = CTime[k][Seq[j]];

		}
	}

	int TF = -1;
	for (int i = 0; i < Len; i++)
	{
		if (CTime[pStage - 1][Seq[i]] > TF)
		{
			TF = CTime[pStage - 1][Seq[i]];
		}
	}

	return TF;
}

int GetEN_CON_MME(vector<int> Seq, int Len)   //��Ŀ�꺯��ֵ For energy consumption
{

	for (auto& vec : MIdleTime)
	{
		fill(begin(vec), end(vec), 0);
	}

	BlockTime = MIdleTime;
	//IdleTime = mIdleTime;


	int mpt1 = INT_MAX;

	for (int j = 0; j < Len; ++j)
	{
		for (int k = 0; k < pStage; ++k)
		{
			//Select a machine
			int mpt, minIdle = INT_MAX;

			//ѡ�����ȿ��л���
			for (int i = 0; i < pMachines[k]; i++)
			{
				if (MIdleTime[k][i] < minIdle)
				{
					minIdle = MIdleTime[k][i];
					mpt = i;
				}

			}

			//��1�׶λ�����ʼ��ʱ��
			//if (Seq[j] == -1)break;//IG�㷨�õ��ж�

			if (k == 0) { STime[k][Seq[j]] = minIdle; mpt1 = mpt; }

			//�����׶�
			else {
				STime[k][Seq[j]] = max(minIdle, CTime[k - 1][Seq[j]]);

				if (minIdle >= CTime[k - 1][Seq[j]])
				{
					BlockTime[k - 1][mpt1] = minIdle - CTime[k - 1][Seq[j]];
					RecordJobBlockTime[k - 1][Seq[j]] = BlockTime[k - 1][mpt1];
					//BlockTotalConsumption += BlockTime[k - 1][mpt1] * BlockConsumption[k - 1][mpt1];
				}

				CTime[k - 1][Seq[j]] = STime[k][Seq[j]];
				MIdleTime[k - 1][mpt1] = CTime[k - 1][Seq[j]];
				mpt1 = mpt;
			}
			CTime[k][Seq[j]] = STime[k][Seq[j]] + pTime[k][Seq[j]];
			MIdleTime[k][mpt] = CTime[k][Seq[j]];

		}
	}

	int TF = -1;
	for (int i = 0; i < Len; i++)
	{
		if (CTime[pStage - 1][Seq[i]] > TF)
		{
			TF = CTime[pStage - 1][Seq[i]];
		}
	}

	return TF;
}

//�������
int GetEN_CON_backward(vector<int> Seq, int Len)   //�������
{
	for (auto& vec : MIdleTime)
	{
		fill(begin(vec), end(vec), 0);
	}


	int mpt1 = INT_MAX;

	for (int j = Len - 1; j >= 0; --j)
	{
		for (int k = pStage - 1; k >= 0; --k)
		{
			//Select a machine
			int mpt, minIdle = INT_MAX;

			//ѡ�����ȿ��л���
			for (int i = 0; i < pMachines[k]; i++)
			{
				if (MIdleTime[k][i] < minIdle)
				{
					minIdle = MIdleTime[k][i];
					mpt = i;
				}

			}

			//���һ���׶λ�����ʼ��ʱ��

			if (k == pStage - 1) { CTime[k][Seq[j]] = minIdle; mpt1 = mpt; }

			//�����׶�
			else {
				CTime[k][Seq[j]] = max(minIdle, STime[k + 1][Seq[j]]);

				//if (minIdle >= STime[k + 1][Seq[j]])
				//{
				//	BlockTime[k + 1][mpt1] = minIdle - STime[k + 1][Seq[j]];
				//	RecordJobBlockTime[k + 1][Seq[j]] = BlockTime[k + 1][mpt1];
				//	//BlockTotalConsumption += BlockTime[k - 1][mpt1] * BlockConsumption[k - 1][mpt1];
				//}

				STime[k + 1][Seq[j]] = CTime[k][Seq[j]];
				MIdleTime[k + 1][mpt1] = STime[k + 1][Seq[j]];
				mpt1 = mpt;
			}
			STime[k][Seq[j]] = CTime[k][Seq[j]] + pTime[k][Seq[j]];
			MIdleTime[k][mpt] = STime[k][Seq[j]];
		}
	}

	int TF = -1;
	for (int i = 0; i < Len; i++)
	{
		if (STime[0][Seq[i]] > TF)
		{
			TF = STime[0][Seq[i]];
		}
	}
	return TF;
}


int GetEN_CON_After_Insert_Job_WYT(const vector<int>& Seq, int job, int pos)   //��Ŀ�꺯��ֵ For energy consumption
{
	for (auto& vec : MIdleTime)
	{
		fill(begin(vec), end(vec), 0);
	}


	int cur_job;
	int mpt1 = INT_MAX;
	for (int j = 0; j < Seq.size() + 1; ++j)
	{		
		if(j < pos)
		{
			cur_job = Seq[j];
		}
		else if(j == pos)
		{
			cur_job = job;
		}
		else
		{
			cur_job = Seq[j - 1];
		}

		for (int k = 0; k < pStage; ++k)
		{
			//Select a machine
			int mpt, minIdle = INT_MAX;

			//ѡ�����ȿ��л���
			for (int i = 0; i < pMachines[k]; i++)
			{
				if (MIdleTime[k][i] < minIdle)
				{
					minIdle = MIdleTime[k][i];
					mpt = i;
				}
			}

			if (k == 0)	//��1�׶λ�����ʼ��ʱ��
			{
				STime[k][cur_job] = minIdle; mpt1 = mpt;
			}
			else //�����׶�
			{
				STime[k][cur_job] = max(minIdle, CTime[k - 1][cur_job]);

				//if (minIdle >= CTime[k - 1][cur_job])
				//{
				//	BlockTime[k - 1][mpt1] = minIdle - CTime[k - 1][cur_job];
				//	RecordJobBlockTime[k - 1][cur_job] = BlockTime[k - 1][mpt1];
				//}

				CTime[k - 1][cur_job] = STime[k][cur_job];
				MIdleTime[k - 1][mpt1] = CTime[k - 1][cur_job];
				mpt1 = mpt;
			}
			CTime[k][cur_job] = STime[k][cur_job] + pTime[k][cur_job];
			MIdleTime[k][mpt] = CTime[k][cur_job];
		}
	}
	
	int TF = -1;
	for (int j = 0; j < Seq.size(); ++j)
	{
		if (CTime[pStage - 1][Seq[j]] > TF)
		{
			TF = CTime[pStage - 1][Seq[j]];
		}
	}
	if (CTime[pStage - 1][job] > TF)
	{
		TF = CTime[pStage - 1][job];
	}
	return TF;
}

//����
int GetEN_CON_After_Insert_Job_backward(const vector<int>& Seq, int job, int pos)   //�ع���ʱ����ÿһ�β��룬����span��ֻ��������õ�
{
	for (auto& vec : MIdleTime)
	{
		fill(begin(vec), end(vec), 0);
	}


	int cur_job;
	int mpt1 = INT_MAX;
	for (int j = Seq.size(); j >= 0; --j)
	{
		if (j > pos)
		{
			cur_job = Seq[j - 1];
		}
		else if (j == pos)
		{
			cur_job = job;
		}
		else
		{
			cur_job = Seq[j];
		}

		for (int k = pStage - 1; k >= 0; --k)
		{
			//Select a machine
			int mpt, minIdle = INT_MAX;

			//ѡ�����ȿ��л���
			for (int i = 0; i < pMachines[k]; i++)
			{
				if (MIdleTime[k][i] < minIdle)
				{
					minIdle = MIdleTime[k][i];
					mpt = i;
				}
			}

			if (k == pStage - 1)	//��1�׶λ�����ʼ��ʱ��
			{
				CTime[k][cur_job] = minIdle; mpt1 = mpt;
			}
			else //�����׶�
			{
				CTime[k][cur_job] = max(minIdle, STime[k + 1][cur_job]);

				//if (minIdle >= STime[k + 1][cur_job])
				//{
				//	BlockTime[k + 1][mpt1] = minIdle - STime[k + 1][cur_job];
				//	RecordJobBlockTime[k + 1][cur_job] = BlockTime[k + 1][mpt1];
				//}

				STime[k + 1][cur_job] = CTime[k][cur_job];
				MIdleTime[k + 1][mpt1] = STime[k + 1][cur_job];
				mpt1 = mpt;
			}
			STime[k][cur_job] = CTime[k][cur_job] + pTime[k][cur_job];
			MIdleTime[k][mpt] = STime[k][cur_job];
		}
	}

	int TF = -1;
	for (int j = 0; j < Seq.size(); ++j)
	{
		if (STime[0][Seq[j]] > TF)
		{
			TF = STime[0][Seq[j]];
		}
	}
	if (STime[0][job] > TF)
	{
		TF = STime[0][job];
	}
	return TF;
}


int GetEN_CON_After_Move_WYT(const vector<int>& Seq, int Pos1, int Pos2, int flag)   //��Ŀ�꺯��ֵ For energy consumption
{
	for (auto& vec : MIdleTime)
	{
		fill(begin(vec), end(vec), 0);
	}


	int cur_job;
	int mpt1 = INT_MAX;

	for (int j = 0; j < Seq.size(); ++j)
	{
		if( j >= Pos1 && j <= Pos2)
		{
			if(flag == 0 )
			{
				if(j == Pos1)
				{
					cur_job = Seq[Pos2];
				}
				else
				{
					cur_job = Seq[j-1];
				}
			}
			else
			{
				if (j == Pos2)
				{
					cur_job = Seq[Pos1];
				}
				else
				{
					cur_job = Seq[j + 1];
				}
			}
		}
		else
		{
			cur_job = Seq[j];
		}
		for (int k = 0; k < pStage; ++k)
		{
			//Select a machine
			int mpt, minIdle = INT_MAX;

			//ѡ�����ȿ��л���
			for (int i = 0; i < pMachines[k]; i++)
			{
				if (MIdleTime[k][i] < minIdle)
				{
					minIdle = MIdleTime[k][i];
					mpt = i;
				}
			}

			if (k == 0)	//��1�׶λ�����ʼ��ʱ��
			{
				STime[k][cur_job] = minIdle; mpt1 = mpt;
			}
			else //�����׶�
			{
				STime[k][cur_job] = max(minIdle, CTime[k - 1][cur_job]);

				//if (minIdle >= CTime[k - 1][cur_job])
				//{
				//	BlockTime[k - 1][mpt1] = minIdle - CTime[k - 1][cur_job];
				//	RecordJobBlockTime[k - 1][cur_job] = BlockTime[k - 1][mpt1];
				//}

				CTime[k - 1][cur_job] = STime[k][cur_job];
				MIdleTime[k - 1][mpt1] = CTime[k - 1][cur_job];
				mpt1 = mpt;
			}
			CTime[k][cur_job] = STime[k][cur_job] + pTime[k][cur_job];
			MIdleTime[k][mpt] = CTime[k][cur_job];
		}
	}

	int TF = -1;
	for (int i = 0; i < Seq.size(); i++)
	{
		if (CTime[pStage - 1][Seq[i]] > TF)
		{
			TF = CTime[pStage - 1][Seq[i]];
		}
	}
	
	return TF;

}

//����
int GetEN_CON_After_Move_backward(const vector<int>& Seq, int Pos1, int Pos2, int flag)   //��Ŀ�꺯��ֵ For energy consumption
{
	for (auto& vec : MIdleTime)
	{
		fill(begin(vec), end(vec), 0);
	}


	int cur_job;
	int mpt1 = INT_MAX;

	for (int j = pJob - 1; j >= 0; --j)
	{
		if (j >= Pos1 && j <= Pos2)
		{
			if (flag == 0)
			{
				if (j == Pos1)
				{
					cur_job = Seq[Pos2];
				}
				else
				{
					cur_job = Seq[j - 1];
				}
			}
			else
			{
				if (j == Pos2)
				{
					cur_job = Seq[Pos1];
				}
				else
				{
					cur_job = Seq[j + 1];
				}
			}
		}
		else
		{
			cur_job = Seq[j];
		}
		for (int k = pStage - 1; k >= 0; --k)
		{
			//Select a machine
			int mpt, minIdle = INT_MAX;

			//ѡ�����ȿ��л���
			for (int i = 0; i < pMachines[k]; i++)
			{
				if (MIdleTime[k][i] < minIdle)
				{
					minIdle = MIdleTime[k][i];
					mpt = i;
				}
			}

			if (k == pStage - 1)	//��1�׶λ�����ʼ��ʱ��
			{
				CTime[k][cur_job] = minIdle; mpt1 = mpt;
			}
			else //�����׶�
			{
				CTime[k][cur_job] = max(minIdle, STime[k + 1][cur_job]);

				/*if (minIdle >= STime[k + 1][cur_job])
				{
					BlockTime[k + 1][mpt1] = minIdle - STime[k + 1][cur_job];
					RecordJobBlockTime[k + 1][cur_job] = BlockTime[k + 1][mpt1];
				}*/

				STime[k + 1][cur_job] = CTime[k][cur_job];
				MIdleTime[k + 1][mpt1] = STime[k + 1][cur_job];
				mpt1 = mpt;
			}
			STime[k][cur_job] = CTime[k][cur_job] + pTime[k][cur_job];
			MIdleTime[k][mpt] = STime[k][cur_job];
		}
	}

	int TF = -1;
	for (int i = 0; i < Seq.size(); i++)
	{
		if (STime[0][Seq[i]] > TF)
		{
			TF = STime[0][Seq[i]];
		}
	}

	return TF;

}

int GetEN_CON_WYT(const vector<int>& Seq, int Len)   //��Ŀ�꺯��ֵ For energy consumption
{
	//vector<vector<int>> STime, CTime;
	//vector<vector<int>> mIdleTime;   //������ʼ���е�ʱ��

	//mIdleTime.resize(pStage);
	/*
	for (int k = 0; k < pStage; k++)
	{
		mIdleTime[k].resize(pMachines[k]);
	}


	//all machines are availale at 0  ��ʵ���Բ���Ҫ Ĭ�϶�Ϊ0
	for (int k = 0; k < pStage; k++)
	{
		for (int i = 0; i < pMachines[k]; i++)
		{
			mIdleTime[k][i] = 0;
		}
	}
	BlockTime = mIdleTime;
	STime.resize(pStage);
	
	for (int k = 0; k < pStage; k++)
	{
		STime[k].resize(pJob);
	}
	for (int i = 0; i < pStage; ++i)
	{
		for (int j = 0; j < pJob; ++j) {
			STime[i][j] = 0;

		}
	}

	CTime = STime;
	*/

	//all machines are availale at 0  ��ʵ���Բ���Ҫ Ĭ�϶�Ϊ0	
	for (auto& vec : MIdleTime)
	{
		fill(begin(vec), end(vec), 0);
	}	

	int mpt1 = INT_MAX;

	for (int j = 0; j < Len; ++j)
	{
		for (int k = 0; k < pStage; ++k)
		{
			//Select a machine
			int mpt, minIdle = INT_MAX;

			//ѡ�����ȿ��л���
			for (int i = 0; i < pMachines[k]; i++)
			{
				if (MIdleTime[k][i] < minIdle)
				{
					minIdle = MIdleTime[k][i];
					mpt = i;
				}
			}
			
			if (k == 0)	//��1�׶λ�����ʼ��ʱ��
			{
				STime[k][Seq[j]] = minIdle; mpt1 = mpt;
			}			
			else //�����׶�
			{
				STime[k][Seq[j]] = max(minIdle, CTime[k - 1][Seq[j]]);
				/*
				if (minIdle >= CTime[k - 1][Seq[j]])
				{
					BlockTime[k - 1][mpt1] = minIdle - CTime[k - 1][Seq[j]];
					RecordJobBlockTime[k - 1][Seq[j]] = BlockTime[k - 1][mpt1];
				}
				*/
				CTime[k - 1][Seq[j]] = STime[k][Seq[j]];
				MIdleTime[k - 1][mpt1] = CTime[k - 1][Seq[j]];
				mpt1 = mpt;
			}
			CTime[k][Seq[j]] = STime[k][Seq[j]] + pTime[k][Seq[j]];
			MIdleTime[k][mpt] = CTime[k][Seq[j]];
		}
	}

	int TF = -1;
	for (int i = 0; i < Len; i++)
	{
		if (CTime[pStage - 1][Seq[i]] > TF)
		{
			TF = CTime[pStage - 1][Seq[i]];
		}
	}

	return TF;
	
}

