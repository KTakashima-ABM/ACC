#pragma once
#include"Agent_AccountingFunction.h"


class FirmAgent : public Agent_AccountingFunction
{
	InAgent* ho;
	vector<InAgent*> mHou;

public:
	FirmAgent(int agent_id) : Agent_AccountingFunction(agent_id, AGENT_TYPE_RETAILER, AGENT_NAME_RETAILER)
	{
		cout << "FirmAgent() called" << endl;
	}

	void SetWorker(InAgent *hou)
	{
		ho = hou;
		mHou.push_back(ho);
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
		Production();
		Payment();
	}

	void aftperAction()
	{
		Agent::aftperAction();
		Agent_AccountingFunction::aftperAction();
		Loan();
		LoanPayment();
	}

	void Budgeting()
	{
		//As a sample, each budget money amount is given a fixed value.
		//In practice, they are determined by the respective decision-making rules.
		//By temporarily storing these values in a separate variable from cash as a budget, fixed costs, etc. that will be required 
		//by the end of the period are separated from the cash variable, making calculations easier.
		//For example, it makes it easier to calculate funds for uses that depend on market prices and stock levels, such as the purchase of raw materials.
		//(This eliminates the need to make exceptions for insufficient cash when fixed costs are required after the purchase of raw materials).

		long long int fixed_salary = 2500 * long long int(mHou.size());
		long long int bonus = dataget_balance("AccruedBonuses", 1, 0);
		long long int loanpayment = 5000;
		long long int interestpayment = 1000;

		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("FixedSalaryBudgeting"), acme, 0, fixed_salary);
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("BonusBudgeting"), acme, 0, bonus);
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("LoanRepaymentBudgeting"), acme, 0, loanpayment);
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("InterestExpenseBudgeting"), acme, 0, interestpayment);


		long long int tax = 0;
		tax = dataget_balance("AccruedCorporationsTax", 1, 0);
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("TaxingBudgeting"), acme, 0, tax);

	}
	
	void Production()
	{
		long long int fixed_salary = 2500 * long long int(mHou.size());

		//Fixed salaries are accounted for as an expense.
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("AccruedFixedSalaries_accounted"), acme, 0, fixed_salary);

		//Fixed wages are transferred as production input costs (raw material costs, etc., which are not included in the example, can also be added up by inserting a journal entry to transfer them).
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("ProductionInputCost_Transfer"), acme, 0, dataget_balance("FixedSalaries", 3, 0));

		//Produce products from production input costs (essentially objectifying the products produced and storing production costs separately)
		writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("ProductProduction"), acme, 0, dataget_balance("ProductionInputCost", 3, 0));


	}

	void Payment()//Payroll behaviour *Simplest example of a financial transaction between agents
	{
		long long int fixed_salary = 2500;
		long long int bonus = dataget_balance("AccruedBonuses",1,0)/long long int(mHou.size());
		long long int bonus_re = dataget_balance("AccruedBonuses", 1, 0) % long long int(mHou.size());

		for (auto ite = mHou.begin(); ite != mHou.end(); ite++)
		{
			writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("FixedSalaryPayment"), (*ite)->Get_accounting_message(), 0, fixed_salary);
			(*ite)->RecieveTradeMasseage("FixedSalaryPayment", acme, 1, fixed_salary);

			if (ite == mHou.begin())
			{
				writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("BonusPayment"), (*ite)->Get_accounting_message(), 0, bonus + bonus_re);
				(*ite)->RecieveTradeMasseage("BonusPayment", acme, 1, bonus + bonus_re);
			}
			else
			{
				writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("BonusPayment"), (*ite)->Get_accounting_message(), 0, bonus);
				(*ite)->RecieveTradeMasseage("BonusPayment", acme, 1, bonus);
			}
			
		}

		simCont.ts->go->RecieveTradeMasseage("AccruedCorporationTaxPayments", acme, 1, dataget_balance("AccruedCorporationsTax", 1, 0));
		RecieveTradeMasseage("AccruedCorporationTaxPayments", simCont.ts->go->Get_accounting_message(), 0, dataget_balance("AccruedCorporationsTax", 1, 0));
		
	}


	void Loan()
	{
		long long int lloan = 10000;
		long long int sloan = 1000;
		
		RecieveTradeMasseage("LongtermLoans", simCont.ts->ba->Get_accounting_message(), 0, lloan);
		simCont.ts->ba->RecieveTradeMasseage("LongtermLoans", acme, 1, lloan);

		RecieveTradeMasseage("ShorttermLoans", simCont.ts->ba->Get_accounting_message(), 0, sloan);
		simCont.ts->ba->RecieveTradeMasseage("ShorttermLoans", acme, 1, sloan);
	}

	void LoanPayment()
	{
		long long int lpay = 4000;
		long long int spay = dataget_balance("ShorttermDebt", 1, 0);
		long long int inte = 1000;

		RecieveTradeMasseage("LongtermLoanRepayments", simCont.ts->ba->Get_accounting_message(), 0, lpay);
		simCont.ts->ba->RecieveTradeMasseage("LongtermLoanRepayments", acme, 1, lpay);

		RecieveTradeMasseage("ShorttermLoanRepayments", simCont.ts->ba->Get_accounting_message(), 0, spay);
		simCont.ts->ba->RecieveTradeMasseage("ShorttermLoanRepayments", acme, 1, spay);

		RecieveTradeMasseage("InterestPayment", simCont.ts->ba->Get_accounting_message(), 0, inte);
		simCont.ts->ba->RecieveTradeMasseage("InterestPayment", acme, 1, inte);
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