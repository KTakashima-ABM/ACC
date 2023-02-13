#pragma once
#include<cstdio>
#include<vector>
#include<string>
#include<iostream>

#include"SimulatorContext.h"
#include"SimulatorConfig.h"

#include"AccSubject.h"

using namespace std;

extern SimulatorContext simCont;

class Account
{
private:
	AccSubject* km;
	//Assets = 0, Liabilities = 1, Equity = 2, Expenses = 3, Revenues = 4 
	vector< AccSubject*> Assets;			//Assets subject aggregation		0
	vector< AccSubject*> Liabilities;		//Liabilities subject aggregation	1
	vector< AccSubject*> Equity;			//Equity subject aggregation		2
	vector< AccSubject*> Expenses;			//Expenses subject aggregation		3
	vector< AccSubject*> Revenues;			//Revenues subject aggregation		4
	
	

	int holder_agent_id;			//ID of the agent holding the Account
	int holder_agent_type;			//Type of the agent holding the Account
	int journal_no;					//Transaction serial numbers, initialized at the beginning of the period

	FILE* GL;						//General ledger output fileÅiData is not output unless a file is passed during generationÅj
	FILE* BS_PL;					//Balance Sheet and Income Statement output fileÅiData is not output unless a file is passed during generationÅj


public:
	Account(int ag_id, int ag_type)	{
		holder_agent_id = ag_id;						//Agent ID is initialized and fixed at the time of generation
		holder_agent_type = ag_type;					//Agent Type is initialized and fixed at the time of generation
		journal_no = 0;									//Initializing Journal number
		settle_mem = new struct settlement_member;		//Generation of variable structure for settlement of account
		GL = NULL;										//Initializing GL
		BS_PL = NULL;									//Initializing BS_PL
	}
	~Account() {};

	void Set_file(FILE* BS_PL, FILE* GL)		//General ledger and balance sheet/income statement files received (agents not received do not output).
	{											
		this->GL = GL;
		this->BS_PL = BS_PL;
	};

	//Journal management functions: journal processing by sequentially calling a group of functions that make up the journal action.
	void Journalizing(int i_period, int codeDeb, int class_codeDeb, long long int amountDeb, const char* Debit, const char* Description, const char* Credit, long long int amountCre, int class_codeCre, int codeCre, const char* ag_counterparty_name, int agent_id, int agent_type);

	//Settlement function management functions: sequential calls to a set of settlement action composition functions to compose settlement actions.
	void settlement_control(double income_tax_rate, double corporation_tax_rate, double bonus_ratio, double executive_compensation_ratio, double dividend_ratio);

	//Accounting table output functions Control functions
	void print_account_control();


//data output system
	//id
	int getAgentId() { return holder_agent_id; };

	//Balance output
	//codeSubject Assets=0, Liabilities=1, Equity=2, Expenses=3, Revenue=4
	//Returns the current, end-of-period and beginning-of-period balances according to codeBalance. codeBalance:0 Current balance, :1 Opening balance, :2 Closing balance
	__int64 dataget_balance(const char* nameSubject, int codeSubject, int codeBalance)
	{
		int a = 0;
		int num = 99999;
		long long int amount = 0;
		num = checkSubject__num_return0(searchSubject_vector(codeSubject), nameSubject);
		if (num != 99999)	amount = searchSubject_vector(codeSubject)[num]->get_balance(codeBalance);	
		else	amount = 0;	
		return amount;
	}






	//Data extraction functions per subject for statistical output.
	//codeSubject Assets=0, Liabilities=1, Equity=2, Expenses=3, Sales=4
	//datacode: 0=increase , 1=decrease , 2=change , 3=balance
	__int64 dataget_subject_data(const char* nameSubject, int codeSubject, int data_code)
	{
		int num = 99999;
		long long int amount = 0;

		try {
			num = checkSubject__num_return0(searchSubject_vector(codeSubject), nameSubject);		//Extract the subscripts of the relevant subject from the subject name.

			if (num != 99999)	//If there is a subject.
			{
				switch (data_code)
				{
				case 0:		//Incremental If the account code is 0 or 3, debit 1, 2 or 4, credit
					if (codeSubject == 0 || codeSubject == 3)	amount = searchSubject_vector(codeSubject)[num]->get_pre_termend_data(1);
					else if (codeSubject == 1 || codeSubject == 2 || codeSubject == 4)	amount = searchSubject_vector(codeSubject)[num]->get_pre_termend_data(2);
					else throw "error It contains subject codes that do not exist. case=0Å@";
					break;

				case 1:		//Reduction Credit if the account code is 0 or 3 Debit if the account code is 1, 2 or 4
					if (codeSubject == 0 || codeSubject == 3)	amount = searchSubject_vector(codeSubject)[num]->get_pre_termend_data(2);
					else if (codeSubject == 1 || codeSubject == 2 || codeSubject == 4)	amount = searchSubject_vector(codeSubject)[num]->get_pre_termend_data(1);
					else throw "error It contains subject codes that do not exist. case=1Å@";
					break;

				case 2:		//Flow If the account code is 0 or 3 (debit - credit - opening balance) If the account code is 1, 2 or 4 (credit - debit - opening balance)
					if (codeSubject == 0 || codeSubject == 3)	amount = searchSubject_vector(codeSubject)[num]->get_pre_termend_data(1) - searchSubject_vector(codeSubject)[num]->get_pre_termend_data(2) - searchSubject_vector(codeSubject)[num]->get_pre_termend_data(3);
					else if (codeSubject == 1 || codeSubject == 2 || codeSubject == 4)	amount = searchSubject_vector(codeSubject)[num]->get_pre_termend_data(2) - searchSubject_vector(codeSubject)[num]->get_pre_termend_data(1) - searchSubject_vector(codeSubject)[num]->get_pre_termend_data(3);
					else throw "error It contains subject codes that do not exist. case=2Å@";
					break;

				case 3:		//Balances Balances returned for all subjects in common
					amount = searchSubject_vector(codeSubject)[num]->get_pre_termend_data(0);
					break;

				default:
					throw"Incorrect data code entered.";
				}
			}
			else			amount = 0;


			return amount;
		}
		catch (char* str)
		{
			cout << str;
			throw "Account::dataget_subject_data\n";
		}
	}



private:

//component function
	vector< AccSubject*> searchSubject_vector(int codeSubject);												//Function that searches for a specified subject name and returns a pointer to the subject set.
	template<class T> bool check_Subject__existence(T& memSubject, const char* subject, int class_code);	//Function that returns whether the specified subject name exists or not with type bool.
	int checkSubject__num(const vector< AccSubject*> &memSubject, const char* subject);						//Function to return the subscript of the set if the specified subject name exists *This is the normal version and stops processing if the subject does not exist.
	int checkSubject__num_return0(const vector< AccSubject*> &memSubject, const char* subject);				//Function to return the subscript of the set if the specified subject name exists *This function is for data collection and returns 0 if the subject does not exist. 

//Å`Å`Å`Å`Å`Å`Å`Å`Group of functions that make up the settlement behaviour.Functions are executed in the following order Functions used only in settlement_controlÅ`Å`Å`Å`Å`Å`Å`Å`Å`Å`
	struct settlement_member {				//For storing various types of earnings at the end of the financial period (initialised at the beginning of each financial period).
		long long int operating_profit;		long long int nonoperating_profit;
		long long int performance_profit;	long long int ordinary_profit;
		long long int pretax_profit;		long long int net_profit;
		long long int net_earnings;
	}*settle_mem;

	void repeatSubject__settlement_control() {			//Functions that call the closing functions in the subject Vector for each subject Vector.
		repeatSubject__settlement(Assets);	repeatSubject__settlement(Liabilities);
		repeatSubject__settlement(Revenues);	repeatSubject__settlement(Expenses);
		repeatSubject__settlement(Equity);
	}
	void reset_settlement_member();							//Initialise the data set used in accounting, such as profit, to zero.
	__int64 settlement_settle__profitloss();				//Profit and loss calculation function
	__int64 settlement_settle__nonoperating_profit();		//Calculation of non-operating profit
	__int64 settlement_settle__performance_profit(long long int operating_profit, long long int nonoperating_profit);	//Calculation of performance profit (profit to determine bonus)
	void settlement_calc__bonus(long long int performance_profit, double bonus_ratio);									//Calculation of bonuses
	
	//Tax calculation management functions: corporate and income tax so have child functions
	void settlement_calc__tax_control(long long int pretax_profit, double income_tax_rate, double corporation_tax_rate);
	void settlement_calc__incometax(double income_tax_rate);											//Calculation of income tax.
	void settlement_calc__corporationtax(long long int pretax_profit, double corporation_tax_rate);		//Calculation of corporation tax
//Calculation of profit appropriation
	void settlement_calc__appropriation_control(long long int net_profit, double executive_compensation_ratio, double dividend_ratio);
	void settlement_calc__executive_compensation(long long int net_profit, double executive_compensation_ratio);						//Calculation of management remuneration
	void settlement_calc__corporate_dividend(long long int net_profit, double dividend_ratio);											//Calculation of shareholder dividends
//Carrying forward operations:	Recording of surplusand carry forward of various items
	void settlement_carryover_control();
	void settlement_carryover__profit();		//Surplus carry forward operations
	void settlement_carryover__Subject()		//Carry-over work for each subject area
	{				
		repeatSubject__carryover(Assets, simCont.period_count);
		repeatSubject__carryover(Liabilities, simCont.period_count);
		repeatSubject__carryover(Revenues, simCont.period_count);
		repeatSubject__carryover(Expenses, simCont.period_count);
		repeatSubject__carryover(Equity, simCont.period_count);
	}


//Code for repetition of the SUBJECT system
	//Repeated functions to call the accounts for each subject
	template<class T> void repeatSubject__settlement(T& memSubject) {					
		for (int i = 0; i<int(memSubject.size()); i++)	memSubject[i]->subject_settlement();
	}
	// Repeat functions to carry forward each subject
	template<class T> void repeatSubject__carryover(T& memSubject, int i_period) {	
		for (int i = 0; i<int(memSubject.size()); i++)	memSubject[i]->Carryover(i_period);
	}
	// Iterative function to sum the balances for each account during the period.
	template<class T> __int64 repeatSubject__sumBalance(T& memSubject) {			
		long long sum = 0;
		for (int i = 0; i<int(memSubject.size()); i++)
			sum += memSubject[i]->get_balance(2) - memSubject[i]->get_balance(1);		
		return sum;
	}
	//Total balance repetitive functions to create BS ,PL
	template<class T> __int64 repeatSubject__print_sumBalance(T& memSubject) {			
		long long sum = 0;
		for (int i = 0; i<int(memSubject.size()); i++)
			sum += memSubject[i]->get_balance(2);		
		return sum;
	}

	//Å`Å`Å`Å`Å`Å`Å` journal-action-constitutive-functionsÅ`Å`Å`Å`Å`Å`Å`Å`Å`
	void Create_Subject_control(int i_period, const char* subject, int code, int class_code);					
	template<class T> void createSubject(T& memSubject, const char* subject, int code, int class_code);			
	 void set_Journalizing__transaction(vector< AccSubject*> memSubject, const char* subject, int journal_no, const char* ag_counterparty_name, int agent_type, int agent_id, const char* Description, const char* contrastSubjects, long long int amount, int codeDebCre);


	 //Å`Å`Å`Å`Å`Å`Å` accounting-table-output-action-constitutive-functions Å`Å`Å`Å`Å`Å`
	void print_BS_PL(long long int sum_Assets, long long int sum_Liabilities, long long int sum_Equity, long long int sum_Revenues, long long int sum_Expenses);
	void print_GL()
	{	
		if (GL != NULL)		
		{
			fprintf(GL, "\n\n-----------------------%i period-------------------------\n\n", simCont.period_count);
			repeat_print_GL(Assets);		repeat_print_GL(Liabilities);
			repeat_print_GL(Revenues);		repeat_print_GL(Expenses);
			repeat_print_GL(Equity);
		}
	}
	template<class T> void repeat_print_GL(T& memSubject)	
	{
		for (int i = 0; i<int(memSubject.size()); i++)memSubject[i]->Print_ac_transaction(GL);
	}



};



void Account::settlement_control(double income_tax_rate, double corporation_tax_rate, double bonus_ratio, double executive_compensation_ratio, double dividend_ratio)
{
	//The settlement process consists of calculating each of the profits required for the final profit settlement.
	//In the sample example, the same function is used for unnecessary profits such as non-operating profit and the calculation is omitted.

	reset_settlement_member();		//Initialisation of profit variable groups for accounts.

	//Settlement of accounts for each account.
	repeatSubject__settlement_control();
	//Determination of operating profit
	settle_mem->operating_profit = settlement_settle__profitloss();
	//Determination of non-operating profit
	settle_mem->nonoperating_profit = settlement_settle__nonoperating_profit();
	//Determination of performance profit (profit to determine bonus = operating profit + ordinary profit)
	settle_mem->performance_profit = settlement_settle__performance_profit(settle_mem->operating_profit, settle_mem->nonoperating_profit);
	//Calculation of bonuses
	settlement_calc__bonus(settle_mem->performance_profit, bonus_ratio);
	//Determination of ordinary profit
	settle_mem->ordinary_profit = settlement_settle__profitloss();
	//Determination of pre-tax profits
	settle_mem->pretax_profit = settlement_settle__profitloss();
	//Tax Calculation
	settlement_calc__tax_control(settle_mem->pretax_profit, income_tax_rate, corporation_tax_rate);
	//Determination of net profit
	settle_mem->net_profit = settlement_settle__profitloss();
	//Calculation of profit appropriation
	settlement_calc__appropriation_control(settle_mem->net_profit, executive_compensation_ratio, dividend_ratio);
	//Finalisation of final profit and loss
	settle_mem->net_earnings = settlement_settle__profitloss();

	//General ledger, PL and BS printouts
	print_account_control();
	//carry-over operations
	settlement_carryover_control();

}

__int64 Account::settlement_settle__profitloss()
{
	long long int profitloss = 0;
	long long int sum_Assets = 0, sum_Liabilities = 0, sum_Revenues = 0, sum_Expenses = 0, sum_Equity = 0;
	//Sum each account by asset, liability, expense and revenue (to establish profit)
	sum_Assets = repeatSubject__sumBalance(Assets);
	sum_Liabilities = repeatSubject__sumBalance(Liabilities);
	sum_Revenues = repeatSubject__sumBalance(Revenues);
	sum_Expenses = repeatSubject__sumBalance(Expenses);
	sum_Equity = repeatSubject__sumBalance(Equity);

	//Calculation of profit
	if (sum_Assets - (sum_Liabilities + sum_Equity) == sum_Revenues - sum_Expenses)//If the calculation does not match, an error code is thrown as problem handling.
	{
		profitloss = sum_Assets - (sum_Liabilities + sum_Equity);
	}
	else//If the calculation does not match, the error code is passed through as a problem process.
	{
		printf("error_There are errors in the profit and loss.\nAsset:%lld , liabilityÅF%lld , Capital%lld , RevenueÅF%lld , Expenses:%lld\n", sum_Assets, sum_Liabilities, sum_Equity, sum_Revenues, sum_Expenses);
		printf("Assets - liabilities - equityÅÅ%lld, Revenue - expenses=%lld\n", sum_Assets - sum_Liabilities - sum_Equity, sum_Revenues - sum_Expenses);
		scanf_s("%lld", &profitloss);
	}
	return profitloss;
}


void Account::reset_settlement_member()	//Initialise the data set used in the accounts, such as profit, to zero.
{
	settle_mem->operating_profit = 0;
	settle_mem->nonoperating_profit = 0;
	settle_mem->performance_profit = 0;
	settle_mem->ordinary_profit = 0;
	settle_mem->pretax_profit = 0;
	settle_mem->net_profit = 0;
	settle_mem->net_earnings = 0;
}


__int64 Account::settlement_settle__nonoperating_profit()		//Calculation of non-operating profit *Compulsory set to 0 as the structure currently does not generate non-operating profit.
{
	long long int nonoperating_profit = 0;

	return nonoperating_profit;
}
__int64 Account::settlement_settle__performance_profit(long long int operating_profit, long long int nonoperating_profit)		//Calculation of performance profit (profit to determine bonus)
{
	long long int performance_profit = 0;
	performance_profit = operating_profit + nonoperating_profit;
	return performance_profit;
}

void Account::settlement_calc__bonus(long long int performance_profit, double bonus_ratio)						//Calculation of bonuses
{
	//The rules for calculating bonuses are prepared as an example.
	int num;
	char error_code[50] = "settlement_calc__bonus";
	long long int bonus = 0;
	try {
		if (performance_profit > 0 && dataget_balance("Cash",0,0) > dataget_balance("ShorttermDebt", 1, 0))		//Bonus payments if there is a profit and you have more cash than short-term borrowing
			bonus = long long int(double(performance_profit) * double(bonus_ratio));
		else if(performance_profit > 0)																			//Reduce the bonus ratio when there is a profit but a high proportion of short-term bonds
			bonus = long long int(double(performance_profit) * double(bonus_ratio)*0.5);

		
		if (bonus != 0)
		{
			Journalizing(simCont.period_count, 3, 0, bonus, "Bonus", "AccruedFixedBonus_accounted", "AccruedBonuses", bonus, 0, 1, "Self", holder_agent_id, holder_agent_type);
			num = checkSubject__num(Expenses, "Bonus");
			Expenses[num]->subject_settlement();
			num = checkSubject__num(Liabilities, "AccruedBonuses");
			Liabilities[num]->subject_settlement();
		}
	}
	catch (char* str) { cout << str; throw"Account::settlement_calc__bonusÅ@Å®Å@\n"; }
}

void Account::settlement_calc__tax_control(long long int pretax_profit, double income_tax_rate, double corporation_tax_rate)					//ê≈ã‡ÇÃåvéZä«óùä÷êîÅAñ@êlÅAèäìæÅAè¡îÔê≈Ç™Ç†ÇÈÇÃÇ≈éqä÷êîÇéùÇ¬
{
	settlement_calc__incometax(income_tax_rate);
	settlement_calc__corporationtax(pretax_profit, corporation_tax_rate);

}
void Account::settlement_calc__incometax(double income_tax_rate)				//Income tax calculations
{
	int num;
	char error_code[50] = "settlement_calc__incometax";
	long long int pay_tax = 0;
	long long int income = 0;

	try {
		income = dataget_balance("ReceivedFixedSalary", 4, 0)
			+ dataget_balance("ReceivedBonuses", 4, 0);

		if (income > 0)	pay_tax = long long int(double(income) * double(income_tax_rate));

		if (pay_tax != 0)
		{
			Journalizing(simCont.period_count, 3, 0, pay_tax, "IncomeTax", "AccruedIncomeTax_accounted", "AccruedIncomeTax", pay_tax, 0, 1, "Self", holder_agent_id, holder_agent_type);
			num = checkSubject__num(Expenses, "IncomeTax");
			Expenses[num]->subject_settlement();
			num = checkSubject__num(Liabilities, "AccruedIncomeTax");
			Liabilities[num]->subject_settlement();
		}
	}
	catch (char* str) { cout << str; throw" Account::settlement_calc__incometaxÅ@Å®Å@\n"; }
}
void Account::settlement_calc__corporationtax(long long int pretax_profit, double corporation_tax_rate)			//Calculation of corporate income tax
{
	int num;
	char error_code[50] = "settlement_calc__corporationtax";
	long long int pay_tax = 0;

	try {
		if (pretax_profit > 0)	pay_tax = long long int(double(pretax_profit) * double(corporation_tax_rate));

		if (pay_tax != 0)
		{
			Journalizing(simCont.period_count, 3, 0, pay_tax, "CorporationsTax", "AccruedCorporationsTax_accounted", "AccruedCorporationsTax", pay_tax, 0, 1, "Self", holder_agent_id, holder_agent_type);
			num = checkSubject__num(Expenses, "CorporationsTax");
			Expenses[num]->subject_settlement();
			num = checkSubject__num(Liabilities, "AccruedCorporationsTax");
			Liabilities[num]->subject_settlement();
		}
	}
	catch (char* str) { cout << str; throw"Account::settlement_calc__corporationtaxÅ@Å®Å@\n"; }
}


void Account::settlement_calc__appropriation_control(long long int net_profit, double executive_compensation_ratio, double dividend_ratio)		//óòâvèàï™ÇÃåvéZ
{
	settlement_calc__executive_compensation(net_profit, executive_compensation_ratio);
	settlement_calc__corporate_dividend(net_profit, dividend_ratio);
}
void Account::settlement_calc__executive_compensation(long long int net_profit, double executive_compensation_ratio)	//Calculation of management remuneration *Not currently used
{
	int num;
	char error_code[50] = "settlement_calc__executive_compensation";
	long long int compensation = 0;

	try {
		if (net_profit > 0)	compensation = long long int(double(net_profit) * double(executive_compensation_ratio));

		if (compensation != 0)
		{
			Journalizing(simCont.period_count, 3, 0, compensation, "ExecutiveCompensation", "AccruedExecutiveCompensation_accounted", "AccruedExecutiveCompensation", compensation, 0, 1, "self", holder_agent_id, holder_agent_type);
			num = checkSubject__num(Expenses, "ExecutiveCompensation");
			Expenses[num]->subject_settlement();
			num = checkSubject__num(Liabilities, "AccruedExecutiveCompensation");
			Liabilities[num]->subject_settlement();
		}
	}
	catch (char* str) { cout << str; throw"Account::settlement_calc__executive_compensation"; }
}
void Account::settlement_calc__corporate_dividend(long long int net_profit, double dividend_ratio)		//Calculation of shareholder dividends *Not currently used
{
	long long int dividend = 0;


}


//carry-over processing
void Account::settlement_carryover_control()
{
	settlement_carryover__profit();
	settlement_carryover__Subject();
}
void Account::settlement_carryover__profit()
{
	//Profit and loss for the current period is transferred to the fund retained earnings account.
	try {
		if (settle_mem->net_earnings > 0)
		{
			char error_code[50] = "settlement_carryover__profit";
			Create_Subject_control(simCont.period_count, "RetainedEarnings", 2, 0);
			int num = 99999;
			num = checkSubject__num(Equity, "RetainedEarnings");
			Equity[num]->Set_ac_transaction(simCont.period_count, journal_no, "Self", holder_agent_type, holder_agent_id, "RetainedEarningsWrite", "RetainedEarnings", settle_mem->net_earnings, 1);
			Equity[num]->subject_settlement();
		}
		else 
		{
			char error_code[50] = "settlement_carryover__profit";
			Create_Subject_control(simCont.period_count, "RetainedEarnings", 2, 0);
			int num = 99999;
			num = checkSubject__num(Equity, "RetainedEarnings");
			Equity[num]->Set_ac_transaction(simCont.period_count, journal_no, "Self", holder_agent_type, holder_agent_id, "RetainedEarningsWrite", "RetainedEarnings", -settle_mem->net_earnings, 0);
			Equity[num]->subject_settlement();
		}
	}
	catch (char* str) { cout << str; throw"Account::settlement_carryover__profit"; }
}




//~~~~~~~~~~~~~~~~~Journalistic function area~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//template <class T>void Account::set_Journalizing__transaction(T& memSubject, const char* subject, int journal_no, const char* ag_counterparty_name, int agent_type, int agent_id, const char* Description, const char* contrastSubjects, long long int amount, int codeDebCre)
void Account::set_Journalizing__transaction(vector< AccSubject*> memSubject, const char* subject, int journal_no, const char* ag_counterparty_name, int agent_type, int agent_id, const char* Description, const char* contrastSubjects, long long int amount, int codeDebCre)
{
	int num = 99999;
	char error_code[50] = "set_Journalizing__transaction";
	try {
		num = checkSubject__num(memSubject, subject);		
		memSubject[num]->Set_ac_transaction(simCont.period_count, journal_no, ag_counterparty_name, agent_type, agent_id, Description, contrastSubjects, amount, codeDebCre);
	}
	catch (char* str) { cout << str; throw"Account::set_Journalizing__transactionÅ@Å®Å@\n"; }
}

void Account::Journalizing(int i_period, int codeDeb, int class_codeDeb, long long int amountDeb, const char* Debit, const char* Description, const char* Credit, long long int amountCre, int class_codeCre, int codeCre, const char* ag_counterparty_name, int agent_id, int agent_type)
{
	const char* error_code = "error_Journalizing:A non-existent subject code was entered.\n";
	Create_Subject_control(i_period, Debit, codeDeb, class_codeDeb);		
	Create_Subject_control(i_period, Credit, codeCre, class_codeCre);		
	

	set_Journalizing__transaction(searchSubject_vector(codeDeb), Debit, journal_no, ag_counterparty_name, agent_type, agent_id, Description, Credit, amountDeb, 0);
	set_Journalizing__transaction(searchSubject_vector(codeCre), Credit, journal_no, ag_counterparty_name, agent_type, agent_id, Description, Debit, amountCre, 1);

	journal_no++;
}
void Account::Create_Subject_control(int i_period, const char* subject, int code, int class_code)	//Check if there is a relevant subject and if not, create one.
{
	switch (code)	
	{
	case 0:		//For asset items
		createSubject(Assets, subject, code, class_code);		break;
	case 1:		//For liability items
		createSubject(Liabilities, subject, code, class_code);		break;
	case 2:		//For capital items
		createSubject(Equity, subject, code, class_code);		break;
	case 3:		//For expenses items
		createSubject(Expenses, subject, code, class_code);		break;
	case 4:		//For revenue items
		createSubject(Revenues, subject, code, class_code);		break;
	}
	
}
template<class T>void  Account::createSubject(T& memSubject, const char* subject, int code, int class_code)
{
	//Generate subjects and insert them into the set of subjects (assets, liabilities, etc.) brought in as arguments.
	if (check_Subject__existence(memSubject, subject, class_code) == false)
	{
		km = new AccSubject;
		km->Set_initial(simCont.period_count, code, class_code, subject, -1);		//Journal_no fixed at -1 when newly generated
		memSubject.push_back(km);
	}
}

//~~~~~~~~~~~~~~~~~Journalistic function area Ends~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//=========================================================================================================================

//~~~~~~~~~~~~~~~~~~~~~operand function area~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<class T> bool Account::check_Subject__existence(T& memSubject, const char* subject, int class_code)	//Check if there is a relevant subject name
{
	int i;
	int dummy;
	bool checkker = false;
	for (i = 0; i<int(memSubject.size()); i++)
	{
		if (memSubject[i]->Check_name(subject) == true)
			if (memSubject[i]->Check__class_code(class_code) == true)	checkker = true;
			else
			{
				printf("error_check_Subject__existence There is a difference between the class codes of the accounting entries and the class codes of the journal entries.\nperiod:%d , journal_no:%d ,  subject:%s\n", simCont.period_count, journal_no, subject);
				scanf_s("%d", &dummy);
			}
	}
	return checkker;
}
int Account::checkSubject__num(const vector< AccSubject*> &memSubject, const char* subject)
{
	//Function to retrieve subscripts for a subject Stop processing if an attempt is made to retrieve a subject that does not exist.
	//Used for internal processing, e.g. for end-of-term processing of Accounts, after the end of the existing subject determination at the time of sorting, etc.
	int i;
	int checkker = 99999;
	for (i = 0; i<int(memSubject.size()); i++)
	{
		if (memSubject[i]->Check_name(subject) == true)
			checkker = i;
	}

	if (checkker == 99999)		//Processing when wrong subject names are entered.
	{
		throw"Account::A non-existent subject name was entered.Å@checkSubject__num";
	}

	return checkker;
}
int Account::checkSubject__num_return0(const vector< AccSubject*> &memSubject, const char* subject)
{
	//Function to retrieve subscripts of a subjectÅ@No error when trying to retrieve non-existent subjects
	//Used for external output, such as statistical processing, where "if a subject does not exist, it is set to 0".

	int checkker = 99999;
	for(auto ite=memSubject.begin();ite!=memSubject.end();ite++)
	{
		if ((*ite)->Check_name(subject)==true)	
			checkker = int(distance(memSubject.begin(), ite));
	}
	return checkker;
}



vector< AccSubject*> Account::searchSubject_vector(int codeSubject)
{//Return the subject Vector corresponding to the subject code (search for subject Vector).
	vector< AccSubject*> U;
	switch (codeSubject)
	{
	case 0:	U = Assets;			break;	
	case 1:	U = Liabilities;	break;	
	case 2:	U = Equity;			break;	
	case 3:	U = Expenses;		break;	
	case 4:	U = Revenues;		break;	

	default:

		throw "Account::searchSubject_vector: codeSubject error\n ";
	}
	return U;
}

//~~~~~~~~~~~~~~~~~~~~~Operational function area End~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//=========================================================================================================================

//~~~~~~~~~~~~~~~~print function area~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Account::print_account_control()
{
	long long int sum_Assets = 0, sum_Liabilities = 0, sum_Revenues = 0, sum_Expenses = 0, sum_Equity = 0;
	sum_Assets = repeatSubject__print_sumBalance(Assets);
	sum_Liabilities = repeatSubject__print_sumBalance(Liabilities);
	sum_Revenues = repeatSubject__print_sumBalance(Revenues);
	sum_Expenses = repeatSubject__print_sumBalance(Expenses);
	sum_Equity = repeatSubject__print_sumBalance(Equity);

	//Actual print part Not executed unless file is set
	print_GL();
	print_BS_PL(sum_Assets, sum_Liabilities, sum_Equity, sum_Revenues, sum_Expenses);

}
void Account::print_BS_PL(long long int sum_Assets, long long int sum_Liabilities, long long int sum_Equity, long long int sum_Revenues, long long int sum_Expenses)
{
	if (BS_PL != NULL)		//Print out only if you have received a BS_PL
	{
		int i;
		long long int ProfitAndLoss = settle_mem->net_earnings;

		//BS printout.
		fprintf(BS_PL, "\n\nbalance sheet\n\n-----------------------%i period-------------------------\n\n", simCont.period_count);
		fprintf(BS_PL, "Subjects , Debits , Credits\n");
		fprintf(BS_PL, "-----------------------------------------------------\n");

		//Subject descriptions in the balance sheet.
		for (i = 0; i<int(Assets.size()); i++) {
			Assets[i]->Print_subject(BS_PL);
			fprintf(BS_PL, " %lld  , \n", Assets[i]->get_balance(2));
		}
		for (i = 0; i<int(Liabilities.size()); i++) {
			Liabilities[i]->Print_subject(BS_PL);
			fprintf(BS_PL, " ,  %lld \n", Liabilities[i]->get_balance(2));
		}
		for (i = 0; i<int(Equity.size()); i++) {
			Equity[i]->Print_subject(BS_PL);
			fprintf(BS_PL, " ,  %lld \n", Equity[i]->get_balance(2));
		}

		fprintf(BS_PL, "profit and loss ,  , %lld\n", ProfitAndLoss);
		fprintf(BS_PL, "----------------------------------------------\n");
		fprintf(BS_PL, " , %lld , %lld , %lld\n ", sum_Assets, sum_Liabilities + sum_Equity + ProfitAndLoss, sum_Assets - (sum_Liabilities + sum_Equity + ProfitAndLoss));


		//Profit and loss account presentation
		fprintf(BS_PL, "\n\nprofit and loss statement\n\n-----------------------%i period-------------------------\n\n", simCont.period_count);
		fprintf(BS_PL, "Subjects , Debits , Credits\n");
		fprintf(BS_PL, "-----------------------------------------------------\n");

		//Description of the subject section of the income statement.
		for (i = 0; i<int(Revenues.size()); i++)
		{
			Revenues[i]->Print_subject(BS_PL);
			fprintf(BS_PL, "  , %lld \n", Revenues[i]->get_balance(2));
		}
		for (i = 0; i<int(Expenses.size()); i++)
		{
			Expenses[i]->Print_subject(BS_PL);
			fprintf(BS_PL, "%lld ,   \n", Expenses[i]->get_balance(2));
		}

		fprintf(BS_PL, "profit and loss  , %lld\n", ProfitAndLoss);
		fprintf(BS_PL, "----------------------------------------------\n");
		fprintf(BS_PL, " , %lld , %lld , %lld\n ", sum_Revenues, sum_Expenses + ProfitAndLoss, sum_Revenues - sum_Expenses - ProfitAndLoss);
	}
}

