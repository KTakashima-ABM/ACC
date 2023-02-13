#pragma once

#include"Agent.h"
#include"Account.h"


class Agent_AccountingFunction : public Agent
{
public:
	Agent_AccountingFunction(int agent_id, int agent_type, const char *agent_name) :
		Agent(agent_id, agent_type,agent_name) 
	{
		cout << "Agent_AccountingFunction() called" << endl;
		create_account(agent_id, agent_type);	//Generate agent accounts.
		set_message();							//Generation of account_message.
		set_init_account_condition();			//Initial status setting of accounts.

	}
	// ~Agent_AccountingFunction();

	accounting_message* acme;	//message box for the counterparty to an accounting transaction with another agent

	//Returns the current, end-of-period and beginning-of-period balances according to balance_code 
	// balance_code=0: current balance, 1: balance at the beginning of the period, 2: balance at the end of the period
	//subject_code assets=0, liabilities=1, equity=2, expenses=3, revenue=4
	__int64 dataget_balance(const char* kamoku_name, int subject_code, int balance_code) { return ac->dataget_balance(kamoku_name, subject_code, balance_code); }

	//Function to retrieve data that has become the previous period's data since the end of the financial year and carried forward.
	// subject_code: assets=0, liabilities=1, equity=2, expenses=3, revenue=4 || datacode: 0=increase , 1=decrease, 2=change, 3=balance
	__int64 dataget_subject_data(const char* kamoku_name, int subject_code, int data_code) { return ac->dataget_subject_data(kamoku_name, subject_code, data_code); }

	accounting_message* Get_accounting_message() { return acme; }

	//Print file for accounting purposes *Start printing when given this
	void setSettlementWritingFile(FILE* BS_PL, FILE* soukan) { ac->Set_file(BS_PL, soukan); }


	
	void begperAction()
	{

	}

	void Action()
	{

	}

	void aftperAction()
	{

	}


	void Settlement()
	{
		switch (agent_type)
		{
		case 0:		//governement
			ac->settlement_control(0, 0, 0, 0, 0);	//The government has zero tax rate and zero bonus rate.
			break;

		case 100:	//Houshold
			ac->settlement_control(simCont.SimConf->iniP_AgGov->IncoTaxRate, 0, 0, 0, 0);
			break;

		case 11000:	//Firm
			ac->settlement_control(0, simCont.SimConf->iniP_AgGov->CorpTaxRate, simCont.SimConf->iniP_AgEmper->BonusRate, simCont.SimConf->iniP_AgEmper->ExceCompRate, simCont.SimConf->iniP_AgEmper->DividendRate);
			break;


		case 21000:
			ac->settlement_control(0, 0, 0, 0, 0);
			break;

		default:
			simCont.ZZZF->ErrorCode("Agent_AccountingFunction","Settlement()",
				"A non-existent Agenttype is called and the accounts cannot be settled.");
			
			break;
		} 

	}




	//Single journal entries
	void writing_Journalizing(int acc_dictionary_id_code, accounting_message* pal_acme, int sendrece_code, long long int deal_amount)
	{
		int repeat;
		if (sendrece_code == 0)//sendrece_code=0 is the sender (originator) of this process
		{
			repeat = simCont.ac_dictionary->Get_Journalizing_number(acc_dictionary_id_code, sendrece_code);

			for (int i = 0; i < repeat; i++)
			{
				ac->Journalizing(
					simCont.period_count
					, simCont.ac_dictionary->Get_S_codeDeb(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_S_class_codeDeb(acc_dictionary_id_code, i)
					, deal_amount
					, simCont.ac_dictionary->Get_S_Debit(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_S_Description(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_S_Credit(acc_dictionary_id_code, i)
					, deal_amount
					, simCont.ac_dictionary->Get_S_class_codeCre(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_S_codeCre(acc_dictionary_id_code, i)
					, pal_acme->agent_name
					, pal_acme->agent_type
					, pal_acme->agent_id
				);
			}
		}
		else if (sendrece_code == 1)//sendrece_code=1 is the recipient of this process (done by receiving a message from the sender)
		{
			repeat = simCont.ac_dictionary->Get_Journalizing_number(acc_dictionary_id_code, sendrece_code);

			for (int i = 0; i < repeat; i++)
			{
				ac->Journalizing(
					simCont.period_count
					, simCont.ac_dictionary->Get_R_codeDeb(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_R_class_codeDeb(acc_dictionary_id_code, i)
					, deal_amount
					, simCont.ac_dictionary->Get_R_Debit(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_R_Description(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_R_Credit(acc_dictionary_id_code, i)
					, deal_amount
					, simCont.ac_dictionary->Get_R_class_codeCre(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_R_codeCre(acc_dictionary_id_code, i)
					, pal_acme->agent_name
					, pal_acme->agent_type
					, pal_acme->agent_id
				);
			}
		}
	}

	//Journal entries when multiple entries are required at the time of journalisation
	void writing_Journalizing(int acc_dictionary_id_code, accounting_message* pal_acme, int sendrece_code, vector<long long int> deal_amount)
	{
		int repeat;
		if (sendrece_code == 0)
		{
			repeat = simCont.ac_dictionary->Get_Journalizing_number(acc_dictionary_id_code, sendrece_code);

			for (int i = 0; i < repeat; i++)
			{
				ac->Journalizing(
					simCont.period_count
					, simCont.ac_dictionary->Get_S_codeDeb(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_S_class_codeDeb(acc_dictionary_id_code, i)
					, deal_amount[i]
					, simCont.ac_dictionary->Get_S_Debit(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_S_Description(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_S_Credit(acc_dictionary_id_code, i)
					, deal_amount[i]
					, simCont.ac_dictionary->Get_S_class_codeCre(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_S_codeCre(acc_dictionary_id_code, i)
					, pal_acme->agent_name
					, pal_acme->agent_type
					, pal_acme->agent_id
				);
			}
		}
		else if (sendrece_code == 1)
		{
			repeat = simCont.ac_dictionary->Get_Journalizing_number(acc_dictionary_id_code, sendrece_code);

			for (int i = 0; i < repeat; i++)
			{
				ac->Journalizing(
					simCont.period_count
					, simCont.ac_dictionary->Get_R_codeDeb(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_R_class_codeDeb(acc_dictionary_id_code, i)
					, deal_amount[i]
					, simCont.ac_dictionary->Get_R_Debit(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_R_Description(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_R_Credit(acc_dictionary_id_code, i)
					, deal_amount[i]
					, simCont.ac_dictionary->Get_R_class_codeCre(acc_dictionary_id_code, i)
					, simCont.ac_dictionary->Get_R_codeCre(acc_dictionary_id_code, i)
					, pal_acme->agent_name
					, pal_acme->agent_type
					, pal_acme->agent_id
				);
			}
		}
	}

	Account* SendAccount() { return ac; };


private:
	Account* ac;
	
	
	void create_account(int agent_id, int agent_type) {		ac = new Account(agent_id, agent_type);	}

	//Generation of account_message
	void set_message()
	{
		acme = new struct accounting_message;
		acme->agent_id = agent_id;
		acme->agent_type = agent_type;
		strcpy_s(acme->agent_name, _countof(acme->agent_name), agent_name);
		fprintf(simCont.SimConf->zzz_account_function_writer, "%s type:%d id:Generate %d set_massage\nid:%d , type:%d , name:%s\n\n", agent_name, agent_type, agent_id, acme->agent_id, acme->agent_type, acme->agent_name);
	}


	//Initial status setting of accounts.
	void set_init_account_condition()
	{
		int iniAsset = 0;

		switch (this->agent_type)
		{
		case 0:			//When agents are in government.	
			iniAsset = simCont.SimConf->iniP_AgGov->iniCash;
			writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("InitialAssetDeployment"), acme, 0, iniAsset);
			break;

		case 100:		//When agents are in household.
			iniAsset = rnd(simCont.SimConf->iniP_AgHou->iniCash_ranMin, simCont.SimConf->iniP_AgHou->iniCash_ranMax);
			writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("InitialAssetDeployment"), acme, 0, iniAsset);
			break;

		case 11000:		//When agents are in firm.
			iniAsset = rnd(simCont.SimConf->iniP_AgFir->iniCash_ranMin, simCont.SimConf->iniP_AgFir->iniCash_ranMax);
			writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("InitialAssetDeployment"), acme, 0, iniAsset);
			break;


		case 21000:		//When agents are in bank.
			writing_Journalizing(simCont.ac_dictionary->Get_dictionary_id_code("InitialAssetDeployment"), acme, 0, simCont.SimConf->iniP_AgBan->iniCash);// RandomGenerator::random_int(app_context.config->BI_parameter->init_cash_min, app_context.config->BI_parameter->init_cash_max));

			break;

		default:
			simCont.ZZZF->stop_function("Agent_AccountingFunction::set_init_account_condition\nNon-existent agent type entered.\n");
			break;
		}
	}

};






