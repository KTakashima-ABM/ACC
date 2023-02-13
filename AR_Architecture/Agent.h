#pragma once
#include<cstdio>
#include<math.h>
#include<random>
#include<iostream>
#include"InAgent.h"
#include"TradeSpace.h"
//#include"Market.h"

/*#include"SimulatorConfig.h"
#include"PrintingFunction.h"
#include"ZZZ_Func.h"
*/
extern SimulatorContext simCont;
using namespace std;

//Agent Classification
const int AGENT_TYPE_GOVERNMENT = 0;					//Government agent type 0`99
const int AGENT_TYPE_HOUSEHOLD = 100;					//Household agent type 100`999
const int AGENT_TYPE_RETAILER = 11000;					//Firm agent type 11000`11999
const int AGENT_TYPE_WHOLESALER = 12000;				//Material maker agent type 12000`12999 *Not used.
const int AGENT_TYPE_EQUIPMENTMAKER = 13000;			//Equipment maker agent type 13000`13999 *Not used.
const int AGENT_TYPE_BANK = 21000;						//Bank agent type21000`21999
const int AGENT_TYPE_ENTERPRISEMANAGER = -1;			//*Not used.

const char* AGENT_NAME_GOVERNMENT = "government";				
const char* AGENT_NAME_HOUSEHOLD = "household";					
const char* AGENT_NAME_RETAILER = "firm";						
const char* AGENT_NAME_WHOLESALER = "material_maker";			
const char* AGENT_NAME_EQUIPMENTMAKER = "equipment_maker";		
const char* AGENT_NAME_BANK = "bank";							
const char* AGENT_NAME_ENTERPRISEMANAGER = "ENTERPRISEMANAGER";



class Agent:public InAgent		//Basic class of all agents
{


public:
	int agent_id;
	int agent_type;
	char agent_name[20];
		
	Agent(int agent_id, int agent_type, const char* agent_name);
	~Agent();

	int getAgentId() { return agent_id; }
	int getAgentType() { return agent_type; }



	//Each action in the term.
	//If there are common behaviours in superclasses such as Agentand Agent_AccountingFunction, describe them.
	//`````action at the beginning of a period`````````
	void begperAction()
	{

	}

	//`````period (main) actions.`````````
	void Action()
	{

	}

	//`````end-of-period action`````````
	void aftperAction()
	{

	}






	virtual void RecieveTradeMasseage(const char* transaction_name, accounting_message* pal_acme, int sendrece_code, long long int deal_amount) = 0;


private:

};


inline Agent::Agent(int agent_id, int agent_type, const char* agent_name)
{
	printf("Agent_constructor test   period(simCont):");
	printf("%d\n\n", simCont.period_count);


	this->agent_type = agent_type;
	this->agent_id = agent_id;
	strcpy_s(this->agent_name, _countof(this->agent_name), agent_name);

}



Agent::~Agent()
{
}
