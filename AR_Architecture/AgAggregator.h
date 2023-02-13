#pragma once
#include<iostream>
#include<vector>

#include"Agent.h"
#include"Agent_AccountingFunction.h"
#include"FirmAgent.h"
#include"HouseholdAgent.h"
#include"GovernmentAgent.h"
#include"BankAgent.h"

#include"Statistics_EconomicIndicator.h"
#include"TradeSpace.h"


using namespace std;
extern SimulatorContext simCont;



class AgAggregator
{
public:
	GovernmentAgent* go;

	BankAgent* ba;

	HouseholdAgent* ho;
	vector<HouseholdAgent*> mHouseholds;

	FirmAgent* fi;
	vector<FirmAgent*> mFirms;



	AgAggregator()
	{

	}
	~AgAggregator()
	{

	}

	//action at the beginning of a period of time
	void begperAction()
	{
		for (auto ite = mHouseholds.begin(); ite != mHouseholds.end(); ite++)
				(*ite)->begperAction();

		for (auto ite = mFirms.begin(); ite != mFirms.end(); ite++)
			(*ite)->begperAction();

		go->begperAction();

		ba->begperAction();
	}

	//Main actions during the period.
	void Action()
	{
		for (auto ite = mFirms.begin(); ite != mFirms.end(); ite++)
			(*ite)->Action();

		for (auto ite = mHouseholds.begin(); ite != mHouseholds.end(); ite++)
			(*ite)->Action();

		go->Action();

		ba->Action();
	}

	//Settlement action
	void SettlementAction()
	{
		for (auto ite = mFirms.begin(); ite != mFirms.end(); ite++)
			(*ite)->Settlement();
		
		for (auto ite = mHouseholds.begin(); ite != mHouseholds.end(); ite++)
			(*ite)->Settlement();
		
		go->Settlement();

		ba->Settlement();
	}

	//end-of-period action
	void aftperAction()
	{
		for (auto ite = mFirms.begin(); ite != mFirms.end(); ite++)
			(*ite)->aftperAction();
		
		for (auto ite = mHouseholds.begin(); ite != mHouseholds.end(); ite++)
			(*ite)->aftperAction();

		go->aftperAction();

		ba->aftperAction();
	}

	//Government creation
	void TsetGovernmentCreate()
	{
		go = new GovernmentAgent(0);
		simCont.ts->go = go;
	}

	//Bank creation
	void TestBankCreate()
	{
		ba = new BankAgent(simCont.agent_id_seq++);
		simCont.ts->ba = ba;
	}

	//Household creation
	void TestHpouseholdCreate()
	{
		for (auto i = 0; i < simCont.SimConf->iniP_Sys->household_size; i++)
		{
			ho = new HouseholdAgent(simCont.agent_id_seq++);
			mHouseholds.push_back(ho);
			simCont.ts->mHou.push_back(ho);
		}
	}

	//Firm creation
	void TestFirmCreate()
	{
		for (auto i = 0; i < simCont.SimConf->iniP_Sys->firm_size; i++)
		{
			fi = new FirmAgent(simCont.agent_id_seq++);
			mFirms.push_back(fi);
			simCont.ts->mFirm.push_back(fi);
		}
	}

	//Worker and employer quotas
	void initAllocateWorker()
	{
		int HNum = int(mHouseholds.size()) / int(mFirms.size());
		int FNum_l = int(mHouseholds.size()) % int(mFirms.size());

		auto Hite = mHouseholds.begin();
		auto Fite = mFirms.begin();

		for (Fite = mFirms.begin(); Fite != mFirms.end(); Fite++)
		{
			for (int i = 0; i < HNum; i++)
			{
				(*Fite)->SetWorker(*Hite);
				Hite++;
			}
			if (FNum_l>0)
			{
				(*Fite)->SetWorker(*Hite);
				Hite++;
				FNum_l = FNum_l - 1;
			}
		}

		Fite = mFirms.begin();
		for (Hite = mHouseholds.begin(); Hite != mHouseholds.end(); Hite++)
		{
			(*Hite)->SetFirm(*Fite);
			Fite++;
			if (Fite == mFirms.end())	Fite = mFirms.begin();
		}

	}



	//Storing pointers to Account for the collection of economic statistics information.
	void initStatistics_AccountSend()
	{
		for (auto ite = mHouseholds.begin(); ite != mHouseholds.end(); ite++)
			simCont.SEI->iniSetAccount((*ite)->agent_type, (*ite)->SendAccount());

		for (auto ite = mFirms.begin(); ite != mFirms.end(); ite++)
			simCont.SEI->iniSetAccount((*ite)->agent_type, (*ite)->SendAccount());

		simCont.SEI->iniSetAccount(go->agent_type, go->SendAccount());

		simCont.SEI->iniSetAccount(ba->agent_type, ba->SendAccount());

	}




	
};

