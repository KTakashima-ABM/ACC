#pragma once
#include<iostream>
#include"SimulatorContext.h"


using namespace std;

extern SimulatorContext simCont;

class SimulatorConfig
{
public:
	SimulatorConfig()
	{
		printf("SimulatorConfig_constructor test   period(simCont):");	
		printf("%d\n\n", simCont.period_count);

		FILE_set();
		
		SetParameter_Control();
		cout << "test2" << endl;
	}
	~SimulatorConfig()
	{

	}

	void FILE_set()
	{
		//system start-up confirmation file@
		fopen_s(&zzz_PrameterWriter, "..\\ZZZ_InputOutput\\output\\zzz_PrameterWriter.csv", "w");
		fopen_s(&zzz_account_function_writer, "..\\ZZZ_InputOutput\\output\\zzz_account_function_writer.csv", "w");


		//AccountDictionary family (AccountDictionary is actually used)
		fopen_s(&AccountDictionary, "..\\ZZZ_InputOutput\\Account\\dictionary.csv", "r");
		fopen_s(&AccountSentence, "..\\ZZZ_InputOutput\\Account\\dictionary_sentence.csv", "r");

		//Output file for accounting@
		fopen_s(&outfile_BS_PL_consumer, "..\\ZZZ_InputOutput\\output\\account_consumer_BS_PL.csv", "w");
		fopen_s(&outfile_soukanzyou_consumer, "..\\ZZZ_InputOutput\\output\\account_consumersoukanzyou.csv", "w");
		fopen_s(&outfile_BS_PL_retailer, "..\\ZZZ_InputOutput\\output\\account_retailer_BS_PL.csv", "w");
		fopen_s(&outfile_soukanzyou_retailer, "..\\ZZZ_InputOutput\\output\\account_retailersoukanzyou.csv", "w");
		fopen_s(&outfile_BS_PL_bank, "..\\ZZZ_InputOutput\\output\\account_bank_BS_PL.csv", "w");
		fopen_s(&outfile_soukanzyou_bank, "..\\ZZZ_InputOutput\\output\\account_banksoukanzyou.csv", "w");
		fopen_s(&outfile_BS_PL_government, "..\\ZZZ_InputOutput\\output\\account_government_BS_PL.csv", "w");
		fopen_s(&outfile_soukanzyou_government, "..\\ZZZ_InputOutput\\output\\account_governmentsoukanzyou.csv", "w");
		fopen_s(&zzz_account_dictionary, "..\\ZZZ_InputOutput\\output\\zzz_account_dictionary.csv", "w");

		
		//For economic statistics
		fopen_s(&Economic_01FreeWriter, "..\\ZZZ_InputOutput\\output\\Economic_01FreeWriter.csv", "w");
		fopen_s(&Economic_03InputOutput_Data, "..\\ZZZ_InputOutput\\output\\Economic_03InputOutput_Data.csv", "w");
		fopen_s(&Economic_04InputOutput_Table, "..\\ZZZ_InputOutput\\output\\Economic_04InputOutput_Table.csv", "w");
		fopen_s(&Economic_05MoneyAsset, "..\\ZZZ_InputOutput\\output\\Economic_05MoneyAsset.csv", "w");
		fopen_s(&Economic_05MoneyAsset_Increase, "..\\ZZZ_InputOutput\\output\\Economic_05MoneyAssetEx01_Increase.csv", "w");
		fopen_s(&Economic_05MoneyAsset_Decrease, "..\\ZZZ_InputOutput\\output\\Economic_05MoneyAssetEx02_Decrease.csv", "w");
		fopen_s(&Economic_05MoneyAsset_Flow, "..\\ZZZ_InputOutput\\output\\Economic_05MoneyAssetEx03_Flow.csv", "w");

		
		fopen_s(&Economic_10Consumer, "..\\ZZZ_InputOutput\\output\\Economic_10Consumer.csv", "w");
		fopen_s(&Economic_11Retailer, "..\\ZZZ_InputOutput\\output\\Economic_11Retailer.csv", "w");
		fopen_s(&Economic_14Bank, "..\\ZZZ_InputOutput\\output\\Economic_14Bank.csv", "w");
		fopen_s(&Economic_15Government, "..\\ZZZ_InputOutput\\output\\Economic_15Government.csv", "w");


	}



	struct initParameter_System
	{
		int max_period;				//Maximum number of simulation periods
		int household_size;			
		int firm_size;				

	}*iniP_Sys;



	struct initParameter_AgentEmployer
	{
		double BonusRate;			//Bonus rate on profit
		double ExceCompRate;		//Ratio of executive remuneration to profit *Not used
		double DividendRate;		//Ratio of dividends to profits *Not used
		double BonusRate_Equip;		//Bonus rates for equipment manufacturing *Not used

	}*iniP_AgEmper;

	struct initParameter_AgentHousehold
	{
		int iniCash_ranMin;		//Lower limit of initial cash holdings
		int iniCash_ranMax;		//Upper limit on initial cash holdings
		
	}*iniP_AgHou;

	struct initParameter_AgentFirm
	{
		int iniCash_ranMin;		//Lower limit of initial cash holdings
		int iniCash_ranMax;		//Upper limit on initial cash holdings

	}*iniP_AgFir;


	struct initParameter_AgentGovernment
	{
		double CorpTaxRate;		
		double IncoTaxRate;		
		int iniCash;			
		
	}*iniP_AgGov;


	struct initParameter_AgentBank
	{
		int iniCash;				
		
	}*iniP_AgBan;




	//FILE for reading
	FILE* Parameter_FILE_1_System;
	FILE* Parameter_FILE_5_Employer;
	FILE* Parameter_FILE_6_Household;
	FILE* Parameter_FILE_7_Firm;
	FILE* Parameter_FILE_8_Government;
	FILE* Parameter_FILE_9_Bank;

	FILE* AccountSentence;			//Journal entries for accounting
	FILE* AccountDictionary;		//Account relation

	//File group for accounting accounts
	FILE* outfile_BS_PL_retailer;
	FILE* outfile_soukanzyou_retailer;
	FILE* outfile_BS_PL_consumer;
	FILE* outfile_soukanzyou_consumer;
	FILE* outfile_BS_PL_bank;
	FILE* outfile_soukanzyou_bank;
	FILE* outfile_BS_PL_government;
	FILE* outfile_soukanzyou_government;
	FILE* zzz_account_dictionary;





	//Files for checking system start-up
	FILE* zzz_PrameterWriter;			//Recording of initial parameters
	FILE* zzz_account_function_writer;	//Description for checking around Account.


	//For economic statistics
	FILE* Economic_01FreeWriter;			//For free economic relations (*File to be used when checking operation)
	FILE* Economic_03InputOutput_Data;		//Vertical time-series file of each piece of data used in the input-output table.
	FILE* Economic_04InputOutput_Table;		//input-output table
	FILE* Economic_05MoneyAsset;			//List of data on fund _Aggregate balances
	FILE* Economic_05MoneyAsset_Increase;	//List of data on fund _Aggregate increase
	FILE* Economic_05MoneyAsset_Decrease;	//List of data on fund _Aggregate decrease
	FILE* Economic_05MoneyAsset_Flow;		//List of data on fund _Aggregate flow
	
	FILE* Economic_10Consumer;			//Household subject aggregation
	FILE* Economic_11Retailer;			//Firm subject aggregation
	FILE* Economic_14Bank;				//Bank subject aggregation
	FILE* Economic_15Government;		//Government subject aggregation



	void SetParameter_Control()
	{

		setPara_System();
		setPara_AgentEmployer();
		setPara_AgentHousehold();
		setPara_AgentFirm(); 
		setPara_AgentGovernment();
		setPara_AgentBank();

		fclose(zzz_PrameterWriter);
	}

	void setPara_System()		
	{
		fopen_s(&Parameter_FILE_1_System, "..\\ZZZ_InputOutput\\parameter\\p01System.csv", "r");

		iniP_Sys = new initParameter_System;

		fscanf_s(Parameter_FILE_1_System,
			"%d,					%d,							%d",
			&iniP_Sys->max_period,	&iniP_Sys->household_size,	&iniP_Sys->firm_size);

		fclose(Parameter_FILE_1_System);


		fprintf(zzz_PrameterWriter, "setPara_System \n");
		fprintf_s(zzz_PrameterWriter,
			"max_period(d),			household_size(d),			firm_size(d)\n" );
		fprintf_s(zzz_PrameterWriter,
			"%d,					%d,							%d\n",
			iniP_Sys->max_period,	iniP_Sys->household_size,	iniP_Sys->firm_size);
		fprintf_s(zzz_PrameterWriter, "\n\n");
		
	}





	void setPara_AgentEmployer()
	{
		fopen_s(&Parameter_FILE_5_Employer, "..\\ZZZ_InputOutput\\parameter\\p05Employer.csv", "r");

		iniP_AgEmper = new initParameter_AgentEmployer;

		fscanf_s(Parameter_FILE_5_Employer,
			"%lf,						%lf,						%lf,						%lf",
			&iniP_AgEmper->BonusRate,	&iniP_AgEmper->ExceCompRate,&iniP_AgEmper->DividendRate,&iniP_AgEmper->BonusRate_Equip);

		fclose(Parameter_FILE_5_Employer);


		fprintf(zzz_PrameterWriter, "setPara_AgentEmployer \n");
		fprintf_s(zzz_PrameterWriter,
			"BonusRate(lf),				ExcectiveCompensationRate(lf),	DividendRate(lf),			BonusRate_Equip(lf)\n");
		fprintf_s(zzz_PrameterWriter,
			"%lf,						%lf,							%lf,						%lf\n",
			iniP_AgEmper->BonusRate,	iniP_AgEmper->ExceCompRate,		iniP_AgEmper->DividendRate,	iniP_AgEmper->BonusRate_Equip);
		fprintf_s(zzz_PrameterWriter, "\n\n");
	}

	void setPara_AgentHousehold()
	{
		fopen_s(&Parameter_FILE_6_Household, "..\\ZZZ_InputOutput\\parameter\\p06Household.csv", "r");

		iniP_AgHou = new initParameter_AgentHousehold;

		fscanf_s(Parameter_FILE_6_Household,
			"%d,						%d",
			&iniP_AgHou->iniCash_ranMin,&iniP_AgHou->iniCash_ranMax);

		fclose(Parameter_FILE_6_Household);


		fprintf(zzz_PrameterWriter, "setPara_AgentHousehold \n");
		fprintf_s(zzz_PrameterWriter,
			"iniCash_ranMin(d),			iniCash_ranMax(d)\n");
		fprintf_s(zzz_PrameterWriter,
			"%d,						%d\n",
			iniP_AgHou->iniCash_ranMin,	iniP_AgHou->iniCash_ranMax);
		fprintf_s(zzz_PrameterWriter, "\n\n");
	}

	void setPara_AgentFirm()
	{
		fopen_s(&Parameter_FILE_7_Firm, "..\\ZZZ_InputOutput\\parameter\\p07Firm.csv", "r");

		iniP_AgFir = new initParameter_AgentFirm;

		fscanf_s(Parameter_FILE_7_Firm,
			"%d,						%d",
			&iniP_AgFir->iniCash_ranMin,&iniP_AgFir->iniCash_ranMax	);

		fclose(Parameter_FILE_7_Firm);


		fprintf(zzz_PrameterWriter, "setPara_AgentFirm \n");
		fprintf_s(zzz_PrameterWriter,
			"iniCash_ranMin(d),			iniCash_ranMax(d)\n");
		fprintf_s(zzz_PrameterWriter,
			"%d,						%d\n",
			iniP_AgFir->iniCash_ranMin,	iniP_AgFir->iniCash_ranMax);
		fprintf_s(zzz_PrameterWriter, "\n\n");
	}

	void setPara_AgentGovernment()
	{
		fopen_s(&Parameter_FILE_8_Government, "..\\ZZZ_InputOutput\\parameter\\p08Government.csv", "r");

		iniP_AgGov = new initParameter_AgentGovernment;

		fscanf_s(Parameter_FILE_8_Government,
			"%lf,						%lf,						%d",
			&iniP_AgGov->IncoTaxRate,	&iniP_AgGov->CorpTaxRate,	&iniP_AgGov->iniCash);

		fclose(Parameter_FILE_8_Government);


		fprintf(zzz_PrameterWriter, "setPara_AgentGovernment \n");
		fprintf_s(zzz_PrameterWriter,
			"IncomeTaxRate(lf),			CorporationTaxRate(lf),		iniCash(d)\n");
		fprintf_s(zzz_PrameterWriter,
			"%lf,						%lf,						%d\n",
			iniP_AgGov->IncoTaxRate,	iniP_AgGov->CorpTaxRate,	iniP_AgGov->iniCash);
		fprintf_s(zzz_PrameterWriter, "\n\n");

	}

	void setPara_AgentBank()
	{
		fopen_s(&Parameter_FILE_9_Bank, "..\\ZZZ_InputOutput\\parameter\\p09Bank.csv", "r");

		iniP_AgBan = new initParameter_AgentBank;

		fscanf_s(Parameter_FILE_9_Bank,
			"%d",
			&iniP_AgBan->iniCash);

		fclose(Parameter_FILE_9_Bank);


		fprintf(zzz_PrameterWriter, "setPara_AgentBank \n");
		fprintf_s(zzz_PrameterWriter,
			"iniCash(d)\n");
		fprintf_s(zzz_PrameterWriter,
			"%d\n",
			iniP_AgBan->iniCash);
		fprintf_s(zzz_PrameterWriter, "\n\n");
	}

	


private:







};





