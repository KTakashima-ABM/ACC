#pragma once

//Simulation management system
class SimulatorConfig;
class ZZZ_Func;

//Simulation accounting system
class AccountDictionary;

//Agent aggregator
class AgAggregator;
class TradeSpace;

//Statistics
class Statistics_EconomicIndicator;


struct accounting_message			//Only the existence is described in the Context in order to handle it as a whole, while the actual content is encapsulated in the Agent_AccountingFunction.
{
	int agent_id;
	int agent_type;
	char agent_name[20];
};


struct SimulatorContext {		

	int agent_id_seq;	
	int period_count;	

	SimulatorConfig* SimConf;
	ZZZ_Func* ZZZF;

	AccountDictionary* ac_dictionary;

	AgAggregator* agg;
	TradeSpace* ts;

	Statistics_EconomicIndicator* SEI;
};

