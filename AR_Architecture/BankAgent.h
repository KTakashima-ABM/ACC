#pragma once
#include"Agent_AccountingFunction.h"

class BankAgent : public Agent_AccountingFunction
{
public:
	BankAgent(int agent_id) :	Agent_AccountingFunction(agent_id, AGENT_TYPE_BANK, AGENT_NAME_BANK)
	{
		cout << "BankAgent() called" << endl;
	}


	void begperAction()
	{
		Agent::begperAction();
		Agent_AccountingFunction::begperAction();
	}

	void Action()
	{
		Agent::Action();
		Agent_AccountingFunction::Action();
		Payment();
	}

	void aftperAction()
	{
		Agent::aftperAction();
		Agent_AccountingFunction::aftperAction();
	}

	void Payment()
	{
		long long int inte = 100;
		for (auto ite = simCont.ts->mHou.begin(); ite != simCont.ts->mHou.end(); ite++)
		{
			RecieveTradeMasseage("DepositInterestPayment", (*ite)->Get_accounting_message(), 0, inte);
			(*ite)->RecieveTradeMasseage("DepositInterestPayment", acme, 1, inte);
		}
	}



	//Functions for receiving journalisation instructions: send journalisation messages to Account
	void RecieveTradeMasseage(const char* transaction_name, accounting_message* pal_acme, int sendrece_code, long long int deal_amount)
	{
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code(transaction_name), pal_acme, sendrece_code, deal_amount);
		//		cout << "recieve messeage from Item to Seller" << endl;
	}
	void RecieveTradeMasseage(const char* transaction_name, accounting_message* pal_acme, int sendrece_code, vector<long long int> deal_amount)
	{
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code(transaction_name), pal_acme, sendrece_code, deal_amount);
	}

	//Return ofacme(accounting_message)
	accounting_message* Get_accounting_message() { return Agent_AccountingFunction::Get_accounting_message(); }

};


