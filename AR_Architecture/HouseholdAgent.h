#pragma once

#include"Agent_AccountingFunction.h"


class HouseholdAgent : public Agent_AccountingFunction
{
public:

	InAgent* firm;


	HouseholdAgent(int agent_id) : Agent_AccountingFunction(agent_id, AGENT_TYPE_HOUSEHOLD, AGENT_NAME_HOUSEHOLD)
	{
		cout << "HouseholdAgent() called" << endl;
	}

	void SetFirm(InAgent *fir)	//Product purchase partner (Although it is not normally appropriate to fix the counterparty, for the sake of simplified sample code, the purchasing counterparty is fixed by default).
	{	firm = fir;	}


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
		Transfer();
	}

	void Budgeting() //Examples of separating cash and other variables into different variables (in this example, the values of Cash and Deposit are transferred to the TaxBudget and SuppliesPurchasingBudget subject for purchasing supplies).
	{
		long long int withdraw= 0,consume=0;
		
		//Cash withdrawal to take out budget for income tax payments (transfer from Deposit balance to Cash balance).
		long long int tax = 0;
		tax = dataget_balance("AccruedIncomeTax", 1, 0);
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("DepositWithdrawal"), acme, 0, tax);
		simCont.ts->ba->RecieveTradeMasseage("DepositWithdrawal", acme, 1, tax);

		//Transfer cash to 'TaxBudget', a cash subject temporarily retired to pay tax.
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("TaxingBudgeting"), acme, 0, tax);


		//Transfer to budget for consumable purchases
		withdraw = long long int(rnd(0, 1) * double(dataget_balance("Deposits", 0, 0)));
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("DepositWithdrawal"), acme, 0, withdraw);
		simCont.ts->ba->RecieveTradeMasseage("DepositWithdrawal", acme, 1, withdraw);

		consume = dataget_balance("Cash", 0, 0);
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("SuppliesPurchasingBudgeting"), acme, 0, consume);
		
	}

	void Payment()
	{
		simCont.ts->go->RecieveTradeMasseage("AccruedIncomeTaxPayments", acme, 1, dataget_balance("AccruedIncomeTax", 1, 0));
		RecieveTradeMasseage("AccruedIncomeTaxPayments", simCont.ts->go->Get_accounting_message(), 0, dataget_balance("AccruedIncomeTax", 1, 0));
	}
		
	void Transfer()//ŠúAfter the end of the 'PurchaseBudget', the surplus in SuppliesPurchasingBudget is transferred to Cash (because if decisions are made with the Cash balance, etc., data in SuppliesPurchasingBudget will upset the calculations).
	{
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("SuppliesPurchasingBudget_Transfer"), acme, 0, dataget_balance("SuppliesPurchasingBudget", 0, 0));


		long long int depo = long long int(double(dataget_balance("Cash", 0, 0)) * rnd(double(0.5), double(1)));

		simCont.ts->ba->RecieveTradeMasseage("DepositCash", acme, 1, depo);
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("DepositCash"), acme, 0, depo);
		
	}
	

	void Buying()
	{
		long long int price = dataget_balance("BudgetMoneyConsumables",0,0);
		long long int cost = price/10;
		vector<long long int > deal_amount;
		deal_amount.push_back(cost);
		deal_amount.push_back(price);

		RecieveTradeMasseage("Buying_CtoR_Supplies", firm->Get_accounting_message(), 0, price);
		firm->RecieveTradeMasseage("Buying_CtoR_Supplies", acme, 1, deal_amount);
		
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

