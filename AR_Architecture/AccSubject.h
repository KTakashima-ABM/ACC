#pragma once
#include<cstdio>
#include<vector>

#include"SimulatorContext.h"

extern SimulatorContext simCont;

using namespace std;

class AccSubject
{
public:
	AccSubject();
	AccSubject(const AccSubject& acs)
	{
	}
	~AccSubject();


	void Set_initial(int i_period, int code_x, int class_code_x, const char* kamoku_x, int jounral_no);		//Functions for setting initial values
	void Set_ac_transaction(int i_period, int journal_no, const char* ag_counterparty_name, int ag_counterparty_type, int ag_counterparty_id, const char* tekiyo, const char* contra_account, long long int amount, int codeDebCre);	//Transaction functions (one of the main functions) called when a transaction occurs
	void subject_settlement();															//Account closing functions, calculate total lending and borrowing and check balances and update ending balances if OK.
	void Carryover(int i_period);

	char* get_subject_name() { return subject_name; };									//Obtaining subject titles.

	__int64 get_balance(int datacode)		//Returns the current, ending and opening balances for the subject
	{										//0: Current transaction vector balance 1: Opening balance 2: Closing balance
		long long int amount = 0;
		switch (datacode)
		{
		case 0:	amount = mem_ac_transaction[mem_ac_transaction.size() - 1]->balance; break;
		case 1:	amount = Balance_beginning;	break;
		case 2:	amount = Balance_termend;	break;
		default:	printf("error_get_balance A non-existent datacode was entered.\n");	break;
		}
		return amount;
	}
	__int64 get_totalDebCre(int codeDebCre)		//Return the sum of debits and credits 0: debits, 1: credits
	{
		long long int amount = 0, cre_total = 0, deb_total = 0;
		for (int i = 0; i<int(mem_ac_transaction.size()); i++)	
		{
			if (mem_ac_transaction[i]->codeDebCre == 0) deb_total += mem_ac_transaction[i]->debit;
			else cre_total += mem_ac_transaction[i]->credit;
		}

		if (codeDebCre == 0)	amount = deb_total;
		else	amount = cre_total;
		return amount;
	}
	__int64 get_pre_termend_data(int datacode)		//Return data at the end of the period 0: balance, 1: debit 2: credit 3: opening balance
	{
		long long int amount = 0;
		switch (datacode)
		{
		case 0:		//balance
			amount = pre_termend_data->balance;			break;
		case 1:		//debt
			amount = pre_termend_data->debit_total;		break;
		case 2:		//credit
			amount = pre_termend_data->credit_total;	break;
		case 3:		//balance at the beginning of the period
			amount = pre_termend_data->Balance_beginning;		break;
		default:
			printf("error_get_termend_data A non-existent datacode was entered  data_code=%d\n", datacode);	break;
		}
		return amount;
	}
	bool Check_name(const char* kamoku) {				//Match search for subject names (used when new subjects are added in Account).
		bool checkker = false;
		if (strcmp(subject_name, kamoku) == 0)
			checkker = true;
		return checkker;
	};
	bool Check__class_code(int code) {		//Check that the subject code received as an argument matches your subject code.　
											//Returns false if it does not, true if it does.
		bool checker = false;
		if (code == class_code) checker = true;
		return checker;
	};



	void Print_ac_transaction(FILE* fi);			//Print transactions (general ledger printouts)
	void Print_subject(FILE* fi) { fprintf(fi, "%s , ", subject_name); };	//Printed name of the subject.



private:

	int code;	//Account codes 0: Assets , 1: Liabilities , 2: Capital , 3: Expenses , 4: Revenue
	int class_code;							
	char subject_name[50];
	long long int Balance_beginning;		//Opening balances (for data capture)
	long long int Balance_termend;			//Period-end balances (for data capture)

	struct ac_transaction_type				//Mixed general ledger and journal data.
	{
		int journal_no;						//Transaction No.
		int i_period;						//Period
		char ag_counterparty_name[50];		//Counterparty agent of the transaction
		int ag_counterparty_type;			//Agent type of counterparty
		int ag_counterparty_id;				//Agent ID of the counterparty
		char description[50];				//description
		char contra_account[50];			//counterpart subject name
		long long int debit;				//Value on the debit side (credit is Null if the value is entered here)
		long long int credit;				//Value on the credit side (debit is Null if the value is entered here)
		long long int balance;				//Balance (updated every time)
		int codeDebCre;						//0 for debit, 1 for credit.
	}*ac_transaction;

	struct previous_termend_data {			//Store figures as at the end of the period, irrespective of subject (up to one period earlier)
		long long int debit_total;
		long long int credit_total;
		long long int Balance_beginning;	//！！The value of the previous period's opening balance (when this value is generated it is still at the beginning of the period)
		long long int balance;				
	}*pre_termend_data;

	vector <struct ac_transaction_type*> mem_ac_transaction;		//Vector to store ac_transaction.
	vector <struct ac_transaction_type*> mem_pre_ac_transaction;	//Vectors to store transactions from the previous period (one period only, anything before two periods ago is discarded).


};

AccSubject::AccSubject()
{
}

AccSubject::~AccSubject()
{
}


void AccSubject::Carryover(int i_period)
{
	int i;
	long long balance;
	//Total debits, credits and complementary balances at the end of the period (including expenses and sales accounts)
	pre_termend_data->debit_total = get_totalDebCre(0);
	pre_termend_data->credit_total = get_totalDebCre(1);
	pre_termend_data->balance = Balance_termend;
	pre_termend_data->Balance_beginning = Balance_beginning;


	//Data carry forward operations: Balance sheet items carry forward balances, income statement items reset balances
	if (code == 0 || code == 1 || code == 2)
	{
		Balance_beginning = Balance_termend;
		Balance_termend = 0;
		balance = Balance_beginning;
	}
	else if (code == 3 || code == 4)
	{
		Balance_beginning = 0;
		Balance_termend = 0;
		balance = 0;
	}

	//Empty previous year's transaction data.
	i = int(mem_pre_ac_transaction.size()) - 1;				
	while (mem_pre_ac_transaction.size() > 0)				
	{
		ac_transaction = mem_pre_ac_transaction[i];			
		mem_pre_ac_transaction.pop_back();					
		delete ac_transaction;								
		i = i - 1;											
	}

	for (int x = 0; x<int(mem_ac_transaction.size()); x++)		//Transferring data from mem_ac_transaction to mem_pre_ac_transaction
	{
		mem_pre_ac_transaction.push_back(mem_ac_transaction[x]);
	}
	mem_ac_transaction.clear();								//Delete the address of the transaction data existing in mem_ac_transaction (address is in mem_preac_transaction)


	//Storing carry-over transaction data
	ac_transaction = new struct ac_transaction_type;											//Generate transaction data for carry-over.
	ac_transaction->i_period = i_period + 1;													//Enter the number of periods in the following period in the transaction data.
	strcpy_s(ac_transaction->description, _countof(ac_transaction->description), "CarryOver");	//The abstract name is 'CarryOver'.
	strcpy_s(ac_transaction->contra_account, _countof(ac_transaction->contra_account), "　");	//Blank because the abstract partner does not exist.
	strcpy_s(ac_transaction->ag_counterparty_name, _countof(ac_transaction->ag_counterparty_name), "Self");
	ac_transaction->ag_counterparty_id = -99999;
	ac_transaction->ag_counterparty_type = -99999;
	ac_transaction->journal_no = -1;											//Transaction numbers are standardised at '-1' for carryovers.
	if (code == 0 || code == 3) {												//Data on assets and expenses are created in debit.
		ac_transaction->codeDebCre = 0;
		ac_transaction->balance = long long(balance);
		ac_transaction->debit = long long(balance);
	}
	else if (code == 1 || code == 2 || code == 4) {								//Liabilities, sales and equity create data for creditors.
		ac_transaction->codeDebCre = 1;
		ac_transaction->balance = long long(balance);
		ac_transaction->credit = long long(balance);
	}
	mem_ac_transaction.push_back(ac_transaction);
}


void AccSubject::subject_settlement()
{
	//Confirmation of period-end balances of total debits and total credits.
	int i;
	long long deb_total = 0, cre_total = 0;
	deb_total = get_totalDebCre(0);		//Calculate the debit sum of the transaction vectors.
	cre_total = get_totalDebCre(1);		//Calculate the credit sum of the transaction vector.

	if (code == 0 || code == 3) {
		if (deb_total - cre_total == mem_ac_transaction[mem_ac_transaction.size() - 1]->balance)
		{
			Balance_termend = mem_ac_transaction[mem_ac_transaction.size() - 1]->balance;
		}
		else
		{
			printf("erro Incorrect calculation of account balances at the end of the year.");
			scanf_s("%d", &i);
		}
	}
	if (code == 1 || code == 2 || code == 4) {
		if (cre_total - deb_total == mem_ac_transaction[mem_ac_transaction.size() - 1]->balance)
		{
			Balance_termend = mem_ac_transaction[mem_ac_transaction.size() - 1]->balance;
		}
		else
		{
			printf("erro Incorrect calculation of account balances at the end of the year.");
			scanf_s("%d", &i);
		}
	}
}


void AccSubject::Print_ac_transaction(FILE* fi)
{
	//Print out the data in the mem_ac_transaction (which is for each subject).
	//Equivalent to data for each subject in the general ledger
	int i;
	long long deb_total = 0, cre_total = 0;
	fprintf(fi, "\n\n------------------------\nsubject name：%s\n------subject code：%d-----------\n", subject_name, code);
	fprintf(fi, "period , journal No , description , counterpart subject , debt , credit , balance, , Counterparty, Counterparty type, Counterparty number\n");
	for (i = 0; i<int(mem_ac_transaction.size()); i++)
	{
		if (mem_ac_transaction[i]->codeDebCre == 0)
		{
			fprintf(fi, "%d , %d , %s , %s, %lld ,  , %lld, , %s, %d, %d\n"
				, mem_ac_transaction[i]->i_period
				, mem_ac_transaction[i]->journal_no
				, mem_ac_transaction[i]->description
				, mem_ac_transaction[i]->contra_account
				, mem_ac_transaction[i]->debit
				, mem_ac_transaction[i]->balance
				, mem_ac_transaction[i]->ag_counterparty_name
				, mem_ac_transaction[i]->ag_counterparty_type
				, mem_ac_transaction[i]->ag_counterparty_id
			);
			deb_total += mem_ac_transaction[i]->debit;
		}
		else
		{
			fprintf(fi, "%d , %d , %s , %s,  , %lld , %lld, , %s, %d, %d\n"
				, mem_ac_transaction[i]->i_period
				, mem_ac_transaction[i]->journal_no
				, mem_ac_transaction[i]->description
				, mem_ac_transaction[i]->contra_account
				, mem_ac_transaction[i]->credit
				, mem_ac_transaction[i]->balance
				, mem_ac_transaction[i]->ag_counterparty_name
				, mem_ac_transaction[i]->ag_counterparty_type
				, mem_ac_transaction[i]->ag_counterparty_id
			);
			cre_total += mem_ac_transaction[i]->credit;
		}
	}

	//	printf("test  kamoku:%s, balance:%d\n",subject_name,Balance_termend); 

	fprintf(fi, "==============================================================================\n");
	fprintf(fi, "  , ,  ,  , %lld , %lld , %lld\n", deb_total, cre_total, Balance_termend);
	fprintf(fi, "------------------------------------------------------------------------------\n\n\n");
}
void AccSubject::Set_ac_transaction(int i_period, int journal_no, const char* ag_counterparty_name, int ag_counterparty_type, int ag_counterparty_id, const char* tekiyo, const char* contra_account, long long int amount, int codeDebCre)
{
	//Inserting transaction data
	ac_transaction = new struct ac_transaction_type;
	ac_transaction->i_period = i_period;
	ac_transaction->journal_no = journal_no;
	strcpy_s(ac_transaction->ag_counterparty_name, _countof(ac_transaction->ag_counterparty_name), ag_counterparty_name);
	strcpy_s(ac_transaction->description, _countof(ac_transaction->description), tekiyo);
	strcpy_s(ac_transaction->contra_account, _countof(ac_transaction->contra_account), contra_account);
	ac_transaction->ag_counterparty_type = ag_counterparty_type;
	ac_transaction->ag_counterparty_id = ag_counterparty_id;
	ac_transaction->codeDebCre = codeDebCre;

	if (code == 0 || code == 3) {		
		if (codeDebCre == 0)
		{
			ac_transaction->debit = amount;
			ac_transaction->credit = 'Null';
			ac_transaction->balance = mem_ac_transaction[mem_ac_transaction.size() - 1]->balance + amount;
		}
		else
		{
			ac_transaction->debit = 'Null';
			ac_transaction->credit = amount;
			ac_transaction->balance = mem_ac_transaction[mem_ac_transaction.size() - 1]->balance - amount;
		}
	}
	else if (code == 1 || code == 2 || code == 4) {		
		if (codeDebCre == 0)
		{
			ac_transaction->debit = amount;
			ac_transaction->credit = 'Null';
			ac_transaction->balance = mem_ac_transaction[mem_ac_transaction.size() - 1]->balance - amount;
		}
		else
		{
			ac_transaction->debit = 'Null';
			ac_transaction->credit = amount;
			ac_transaction->balance = mem_ac_transaction[mem_ac_transaction.size() - 1]->balance + amount;
		}
	}
	mem_ac_transaction.push_back(ac_transaction);
}
void AccSubject::Set_initial(int i_period, int code_x, int class_code_x, const char* kamoku_x, int journal_no)
{
	//When generating subjects, set the subject class.
	code = code_x;
	class_code = class_code_x;
	strcpy_s(subject_name, _countof(subject_name), kamoku_x);
	Balance_termend = 0;
	Balance_beginning = 0;


	//Initialisation of end-of-term data pre_termend_data
	pre_termend_data = new previous_termend_data;
	pre_termend_data->debit_total = 0;
	pre_termend_data->credit_total = 0;
	pre_termend_data->balance = 0;

	//Transactions for setting initial values when generating accounts
	int size = int(mem_ac_transaction.size());

	ac_transaction = new struct ac_transaction_type;
	mem_ac_transaction.push_back(ac_transaction);

	mem_ac_transaction[size]->i_period = i_period;
	mem_ac_transaction[size]->journal_no = journal_no;
	strcpy_s(mem_ac_transaction[size]->description, _countof(mem_ac_transaction[size]->description), "carry-over from the previous period");
	strcpy_s(mem_ac_transaction[size]->contra_account, _countof(mem_ac_transaction[size]->contra_account), " ");
	mem_ac_transaction[size]->debit = 0;
	mem_ac_transaction[size]->balance = 0;
	mem_ac_transaction[size]->codeDebCre = 0;
	mem_ac_transaction[size]->ag_counterparty_type = 0;
	mem_ac_transaction[size]->ag_counterparty_id = 0;
	strcpy_s(mem_ac_transaction[size]->ag_counterparty_name, _countof(mem_ac_transaction[size]->ag_counterparty_name), " ");
}
