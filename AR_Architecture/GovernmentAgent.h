#pragma once
#include"Agent_AccountingFunction.h"


class GovernmentAgent :public Agent_AccountingFunction
{
public:
	GovernmentAgent(int agent_id) : Agent_AccountingFunction(agent_id, AGENT_TYPE_GOVERNMENT, AGENT_NAME_GOVERNMENT)
	{
		cout << "GovernmentAgent() called" << endl;
	}

	void begperAction()
	{
		Agent::begperAction();
		Agent_AccountingFunction::begperAction();
		Budgeting();
	}

	void Action()
	{
		Agent::Action();						
		Agent_AccountingFunction::Action();		
		Buying();
		Payment();
	}

	void aftperAction()
	{
		Agent::aftperAction();
		Agent_AccountingFunction::aftperAction();
	}

	void Budgeting()
	{
		long long int purchasing = 50000;
		long long int subsidy = 100000;
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("PurchasingBudgeting"), acme, 0, purchasing);
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("CorporateSubsidiesBudgeting"), acme, 0, subsidy);

		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("SuppliesPurchasingBudgeting_G"), acme, 0, purchasing);

	}
	
	void Payment()
	{
		long long int subsidy = 100000 / long long int(simCont.ts->mFirm.size());

		for (auto ite = simCont.ts->mFirm.begin(); ite != simCont.ts->mFirm.end(); ite++)
		{
			writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("CorporateSubsidiesPayment"), (*ite)->Get_accounting_message(), 0, subsidy);
			(*ite)->RecieveTradeMasseage("CorporateSubsidiesPayment", acme, 1, subsidy);
		}
	}

	//Market purchases (public investment)
	void Buying()
	{
		long long int price = dataget_balance("BudgetMoneyConsumables",0,0)/ long long int(simCont.ts->mFirm.size());
		long long int cost = price/10;
		vector<long long int > deal_amount;
		deal_amount.push_back(cost);
		deal_amount.push_back(price);

		for (auto ite = simCont.ts->mFirm.begin(); ite != simCont.ts->mFirm.end(); ite++)
		{
			RecieveTradeMasseage("Buying_GtoR_Supplies", (*ite)->Get_accounting_message(), 0, price);
			(*ite)->RecieveTradeMasseage("Buying_GtoR_Supplies", acme, 1, deal_amount);
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