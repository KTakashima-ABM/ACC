#pragma once
#include<cstdio>
#include<vector>
#include<map>
#include<list>
#include<random>

//management class group
#include"SimulatorConfig.h"
#include"SimulatorContext.h"
#include"ZZZ_RandomGenerator.h"
#include"ZZZ_Func.h"

//Accounting-related
#include"AccountDictionary.h"

//‘ã—“XŒQ
#include"AgAggregator.h"
#include"Agent.h"
#include"Agent_AccountingFunction.h"
#include"HouseholdAgent.h"
#include"FirmAgent.h"
#include"GovernmentAgent.h"
#include"BankAgent.h"
#include"TradeSpace.h"

//statistical system
#include"Statistics_EconomicIndicator.h"

using namespace std;
extern SimulatorContext simCont;


class SimulatorApp
{
	ZZZ_RandomGenerator* Random;

public:
	SimulatorApp()
	{
		printf("SimulatorApp_constructor test\n");	
		simCont.agent_id_seq = 0;					
		simCont.period_count = 0;					

		rnd.seed(1);		//Fixed random number seed value.

		simCont.SimConf = new SimulatorConfig();
		simCont.ac_dictionary = new AccountDictionary();

	}
	~SimulatorApp()
	{

	}


	void Set_up()
	{
		//Various object creation where simCont has a pointer.
		simCont.agg = new AgAggregator();		
		simCont.SEI = new Statistics_EconomicIndicator();
		simCont.ts = new TradeSpace();



	//Generating agents
		simCont.agg->TsetGovernmentCreate();	
		simCont.agent_id_seq = 101;
		simCont.agg->TestHpouseholdCreate();	
		simCont.agent_id_seq = 11001;
		simCont.agg->TestFirmCreate();			
		simCont.agent_id_seq = 13001;
		simCont.agent_id_seq = 21001;
		simCont.agg->TestBankCreate();			

		simCont.agg->initAllocateWorker();	//Assignment of workers


	//Preparation of statistics.
		simCont.agg->initStatistics_AccountSend();		//Collect Account pointers for statistics from each agent.
		simCont.SEI->iniCreate_SD_Contorl();			//Generation of the required data structures
		simCont.SEI->iniMoneyAsset();					//Generation of column headings for asset data output systems.
		simCont.SEI->iniStatisticsAgents_control();		//Individual agent column heading generation.
		
		
	
	//Balance sheet and general ledger are handed over to the agent * When handed over, the agent to whom it is handed over makes a statement
		simCont.agg->mHouseholds[0]->setSettlementWritingFile(simCont.SimConf->outfile_BS_PL_consumer, simCont.SimConf->outfile_soukanzyou_consumer);
		simCont.agg->mFirms[0]->setSettlementWritingFile(simCont.SimConf->outfile_BS_PL_retailer, simCont.SimConf->outfile_soukanzyou_retailer);
		simCont.agg->go->setSettlementWritingFile(simCont.SimConf->outfile_BS_PL_government, simCont.SimConf->outfile_soukanzyou_government);
		simCont.agg->ba->setSettlementWritingFile(simCont.SimConf->outfile_BS_PL_bank, simCont.SimConf->outfile_soukanzyou_bank);
		
	//Settlement of accounts at the time of generation (period 0)
		simCont.agg->SettlementAction();


	//Simulation processing
		for (int i = 0; i < simCont.SimConf->iniP_Sys->max_period; i++)
		{
			simCont.period_count++;
			simCont.agg->begperAction();
			simCont.agg->Action();
			simCont.agg->aftperAction();
			simCont.agg->SettlementAction();
			simCont.SEI->CalcAndOutput_Control();
		}
		
	}
	









	


private:

};












