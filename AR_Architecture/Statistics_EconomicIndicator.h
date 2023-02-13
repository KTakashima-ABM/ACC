#pragma once
#include<cstdio>
#include<vector>
#include"Account.h"

using namespace std;
extern SimulatorContext simCont;

// Å`Å`Å`Å`Å`Å` Usage: Å`Å`Å`Å`Å`Å`Å`Å`
// Preparation stage.
// 1. Constructor - creates mem_EC
// 2. Call iniSetAccount() from AgAggregator to collect the account
// 3. Call iniCreate_SD_Contorl() from SimulatorApp to create SD
// 4. Call iniMoneyAsset() from SimulatorApp to prepare for writing

// Actual use phase.
// Call CalcAndOutput_Control() from SimulatorApp, and the process is automatic from then on.
//* The Input-Output table needs to be modified as the number of subjects and agents increases.


class Statistics_EconomicIndicator
{
	
	struct input_output			//Dataset for Input-output table
	{
		long long int input_r;
		long long int input_w;
		long long int input_e;
		long long int input_b;
		long long int input_T;				//Vertical total of intermediate inputs
		long long int out_expenses_T;		//Vertical total of non-household consumption expenditure
		long long int private_consume_T;	//Vertical total of private consumption demand
		long long int public_consume_T;		//Vertical total of government consumption demand
		long long int fixed_capital_T;		//Vertical total of fixed capital formation
		long long int stock_T;				//Vertical total of stock
		long long int final_demand_T;		//GDPÅiVertical sum of final_demandÅj


		long long int pay_out_expenses_T;	//Total non-operating expenditure
		long long int pay_worker_T;			//Total employment income
		long long int profit_T;				//Total operating surplus
		long long int depreciation_T;		//Total depreciation
		long long int indirect_tax_T;		//Total indirect taxes
		long long int subsidy_T;			//Total subsidies
		long long int GDP_cor;				//Total gross value added


	} *in_out;

	struct subject_data		//Data per subject stored in mem_sd inside economiccount
	{
		int kamoku_code;
		char kamoku_name[50];
		long long int increase;		//Increase in subject accounts (total debits or total credits)
		long long int decrease;		//Decrease in subject accounts (total credit or total debit)
		long long int flow;			//Changes in subject accounts (debits - credits - beginning of year or credits - debits - beginning of year)
		long long int zandaka;		//Balance


		long long int incre_max;	//Maximum increase
		long long int decre_max;	//Maximum decrease
		long long int zandaka_max;	//Maximum balance

		long long int incre_min;	//Minimum increase
		long long int decre_min;	//Minimum decrease
		long long int zandaka_min;	//Minimum balance

		double incre_ave;			//Incremental average
		double decre_ave;			//Decremental Average
		double zandaka_ave;			//Balance average
	}*sd;


	struct economiccount	//Structure to store the total accounting settlement for each agent type.
	{
		int aggregate_type_id;					//Type of agent set
		char aggregate_name[50];				//agent set name

		vector <struct subject_data*> mem_sd;	//Set of subjects
		vector<Account*> mAcc;					//Account pointer set for the agent type concerned.

		long long int mid_R;			//Product sales_R_Balance of raw materials
		long long int mid_W;			//Product sales_W_Balance of raw materials	
		long long int mid_E;			//Product sales_E_Balance of raw materials
		long long int mid_B;			//Product sales_B_Balance of raw materials

		long long int mid_demand;		//intermediate demand
		long long int out_expenses;		//Out-of-home consumption expenditure
		long long int private_consume;	//Private consumption needs
		long long int public_consume;	//Government expenditure
		long long int fixed_capital;	//Fixed capital formation
		long long int stocks;			//Product inventory
		long long int final_demand;		//Final demand


		long long int pay_out_expenses;	//Out-of-home consumption expenditure
		long long int pay_worker;		//Wage
		long long int profit;			//Profit
		long long int depreciation;		//Depreciation
		long long int indirect_tax;		//Indirect taxation
		long long int subsidy;			//Subsidy
		long long int arahuka;			//Gross added value

	}*EC;

	vector <struct economiccount*> mem_EC;	//Set to store ECs for each agent type.

	


	


public:
	Statistics_EconomicIndicator()
	{
		//Structure generation for input-output tables.
		in_out = new input_output;



		//Generate tables for aggregate data for each agent set.
		for (int i = 0; i < 6; i++)
		{
			EC = new economiccount;
			mem_EC.push_back(EC);
		}

		strcpy_s(mem_EC[0]->aggregate_name, _countof(mem_EC[0]->aggregate_name), "consumer");
		mem_EC[0]->aggregate_type_id = 100;

		strcpy_s(mem_EC[1]->aggregate_name, _countof(mem_EC[1]->aggregate_name), "retailer");
		mem_EC[1]->aggregate_type_id = 11000;

		strcpy_s(mem_EC[2]->aggregate_name, _countof(mem_EC[2]->aggregate_name), "wholesaler");
		mem_EC[2]->aggregate_type_id = 12000;

		strcpy_s(mem_EC[3]->aggregate_name, _countof(mem_EC[3]->aggregate_name), "equipment_maker");
		mem_EC[3]->aggregate_type_id = 13000;

		strcpy_s(mem_EC[4]->aggregate_name, _countof(mem_EC[4]->aggregate_name), "bank");
		mem_EC[4]->aggregate_type_id = 21000;

		strcpy_s(mem_EC[5]->aggregate_name, _countof(mem_EC[5]->aggregate_name), "government");
		mem_EC[5]->aggregate_type_id = 0;

		
		fprintf(simCont.SimConf->Economic_03InputOutput_Data, "period,GDPÅifinal demandÅj,GDP(gross value added),");
		fprintf(simCont.SimConf->Economic_03InputOutput_Data,
			"R_intermediate R,R_intermediate W,R_intermediate E,R_intermediate B,R_intermediate need,R_extra domestic consumption expenditure,R_private consumption,R_public consumption,R_fixed capital formation,R_inventory,R_final need,");
		fprintf(simCont.SimConf->Economic_03InputOutput_Data,
			"W_intermediate R,W_intermediate W,W_intermediate E,W_intermediate B,W_intermediate need,W_extra domestic consumption expenditure,W_non-public consumption,W_public consumption,W_fixed capital formation,W_inventory,W_final need,");
		fprintf(simCont.SimConf->Economic_03InputOutput_Data,
			"E_intermediate R,E_intermediate W,E_intermediate E,E_intermediate B,E_intermediate need,E_extra domestic consumption expenditure,E_non-public consumption,E_public consumption,E_fixed capital formation,E_inventory,E_final need,");
		fprintf(simCont.SimConf->Economic_03InputOutput_Data,
			"B_intermediate R,B_intermediate W,B_intermediate E,B_intermediate B,B_intermediate need,B_extra domestic consumption expenditure,B_private consumption,B_public consumption,B_fixed capital formation,B_inventory,B_final need,");
		fprintf(simCont.SimConf->Economic_03InputOutput_Data, "\n");


	}

	~Statistics_EconomicIndicator()
	{

	}

	//Give a pointer to the Account to each set of ECs.Å@
	void iniSetAccount(int agent_type_id, Account* ac)
	{
		//Change the EC you put in depending on the AgentTypeId.
		switch (agent_type_id)
		{
		case 100:	mem_EC[0]->mAcc.push_back(ac);	break;
		case 11000:	mem_EC[1]->mAcc.push_back(ac);	break;
		case 12000:	mem_EC[2]->mAcc.push_back(ac);	break;
		case 13000:	mem_EC[3]->mAcc.push_back(ac);	break;
		case 21000:	mem_EC[4]->mAcc.push_back(ac);	break;
		case 0:		mem_EC[5]->mAcc.push_back(ac);	break;

		default:
			simCont.ZZZF->ErrorCode("Statistics_EconomicIndicatyor", "iniSetAccount()",
				"A non-existent Agenttype was sent in.");
			break;
		}
	}



	//Generation of SD
	void iniCreate_SD_Contorl()
	{
		//Asset
		iniCreate_SD("BonusBudget", 0);
		iniCreate_SD("BudgetMoneyConsumables", 0);
		iniCreate_SD("Cash", 0);
		iniCreate_SD("CorporateSubsidiesBudget", 0);
		iniCreate_SD("Deposits", 0);
		iniCreate_SD("FixedSalaryBudget", 0);
		iniCreate_SD("InterestExpenseBudget", 0);
		iniCreate_SD("LoanRepaymentBudget", 0);
		iniCreate_SD("LongtermLoanReceivables", 0);
		iniCreate_SD("Products", 0);
		iniCreate_SD("PurchasingBudget", 0);
		iniCreate_SD("ShorttermLoanReceivables", 0);
		iniCreate_SD("TaxBudget", 0);


		//Liability
		iniCreate_SD("AccruedBonuses", 1);
		iniCreate_SD("AccruedCorporationsTax", 1);
		iniCreate_SD("AccruedFixedSalaries", 1);
		iniCreate_SD("AccruedIncomeTax", 1);
		iniCreate_SD("DepositsUnderCustody", 1);
		iniCreate_SD("LongtermDebt", 1);
		iniCreate_SD("ShorttermDebt", 1);


		//Capital
		iniCreate_SD("RetainedEarnings", 2);
		iniCreate_SD("Capital", 2);


		//Exepenses
		iniCreate_SD("CorporateSubsidies", 3);
		iniCreate_SD("CostSales", 3);
		iniCreate_SD("FixedSalaries", 3);
		iniCreate_SD("InterestExpense", 3);
		iniCreate_SD("InterestExpense(Interest)", 3);
		iniCreate_SD("ProductionInputCost", 3);
		iniCreate_SD("Supplies_R", 3);
		iniCreate_SD("CorporationsTax",3);
		iniCreate_SD("IncomeTax", 3);
		iniCreate_SD("Bonus", 3);


		//Revenue
		iniCreate_SD("ProductSalesSupplies_C", 4);
		iniCreate_SD("ProductSalesSupplies_G", 4);
		iniCreate_SD("ReceivedBonuses", 4);
		iniCreate_SD("ReceivedCorporationsTax", 4);
		iniCreate_SD("ReceivedDepositInterest", 4);
		iniCreate_SD("ReceivedFixedSalary", 4);
		iniCreate_SD("ReceivedIncomeTax", 4);
		iniCreate_SD("ReceivedLoanInterest", 4);
		iniCreate_SD("ReceivedSubsidies", 4);

		
		iniCreate_SD("NotCurrentlyUsed", 0);
		iniCreate_SD("NotCurrentlyUsed", 1);
		iniCreate_SD("NotCurrentlyUsed", 2);
		iniCreate_SD("NotCurrentlyUsed", 3);
		iniCreate_SD("NotCurrentlyUsed", 4);
		

	}


	//SD generating functions
	void iniCreate_SD(const char* kamoku, int kamoku_code)
	{
		for (auto ite = mem_EC.begin(); ite != mem_EC.end(); ite++)
		{
			sd = new subject_data;
			sd->kamoku_code = kamoku_code;
			strcpy_s(sd->kamoku_name, _countof(sd->kamoku_name), kamoku);
			sd->increase = 0;
			sd->decrease = 0;
			sd->flow = 0;
			sd->zandaka = 0;

			(*ite)->mem_sd.push_back(sd);
		}

	}





	void CalcAndOutput_Control()
	{
	//Initialisation of data and collection of information from each account.
		//Initialisation of in_out and mem_EC
		reset_economiccount();

		//Aggregation of data
		TakeAccountData();
		

	//Calculation and output process thereafter.
		//Compilation of data required for the preparation of an input-output table
		Create_InputOutputData();

		//Generation of an input-output table.
		Create_InputOutputTable();


		//Generation and output of economic statistics
		writeMoneyAsset();
		writeMoneyAsset_Increase();
		writeMoneyAsset_Decrease();
		writeMoneyAsset_Flow();

		writeStatisticsAgents(mem_EC[0], simCont.SimConf->Economic_10Consumer);
		writeStatisticsAgents(mem_EC[1], simCont.SimConf->Economic_11Retailer);
		writeStatisticsAgents(mem_EC[4], simCont.SimConf->Economic_14Bank);
		writeStatisticsAgents(mem_EC[5], simCont.SimConf->Economic_15Government);


	}


	//Reset various data.
	void reset_economiccount() {

		//Initialisation of variables for aggregation.
		in_out->input_r = 0;				in_out->input_w = 0;
		in_out->input_e = 0;				in_out->input_b = 0;
		in_out->input_T = 0;

		in_out->fixed_capital_T = 0;		in_out->final_demand_T = 0;
		in_out->out_expenses_T = 0;			in_out->private_consume_T = 0;
		in_out->public_consume_T = 0;		in_out->stock_T = 0;

		in_out->pay_out_expenses_T = 0;		in_out->pay_worker_T = 0;
		in_out->profit_T = 0;				in_out->depreciation_T = 0;
		in_out->indirect_tax_T = 0;			in_out->subsidy_T = 0;
		in_out->GDP_cor = 0;

		//Initialisation of mem_EC
		for (auto ite = mem_EC.begin(); ite != mem_EC.end(); ite++)
		{
			(*ite)->mid_R = 0;	(*ite)->mid_W = 0;	(*ite)->mid_E = 0;	(*ite)->mid_B = 0;

			(*ite)->mid_demand = 0;		(*ite)->out_expenses = 0;	(*ite)->private_consume = 0;
			(*ite)->public_consume = 0;	(*ite)->fixed_capital = 0;	(*ite)->stocks = 0;
			(*ite)->final_demand = 0;

			(*ite)->pay_out_expenses = 0;	(*ite)->pay_worker = 0;		(*ite)->profit = 0;
			(*ite)->depreciation = 0;		(*ite)->indirect_tax = 0;	(*ite)->subsidy = 0;
			(*ite)->arahuka = 0;


			for (auto iteEC = (*ite)->mem_sd.begin(); iteEC != (*ite)->mem_sd.end(); iteEC++)
			{
				(*iteEC)->decrease = 0;
				(*iteEC)->increase = 0;
				(*iteEC)->flow = 0;
				(*iteEC)->zandaka = 0;

				(*iteEC)->decre_ave = 0; (*iteEC)->decre_max = 0; (*iteEC)->decre_min = 0;
				(*iteEC)->incre_ave = 0; (*iteEC)->incre_max = 0; (*iteEC)->incre_min = 0;
				(*iteEC)->zandaka_ave = 0; (*iteEC)->zandaka_max = 0; (*iteEC)->zandaka_min = 0;
			}
		}
	}



	void TakeAccountData()
	{
		//Store the data of the required subjects in the SD data of mem_EC respectively.
		//What we do: give take_subject_data "agent set", "EC", "subject name" and "subject code" and run it.

		//Asset
		take_subject_data("BonusBudget", 0);
		take_subject_data("BudgetMoneyConsumables", 0);
		take_subject_data("Cash", 0);
		take_subject_data("CorporateSubsidiesBudget", 0);
		take_subject_data("Deposits", 0);
		take_subject_data("FixedSalaryBudget", 0);
		take_subject_data("InterestExpenseBudget", 0);
		take_subject_data("LoanRepaymentBudget", 0);
		take_subject_data("LongtermLoanReceivables", 0);
		take_subject_data("Products", 0);
		take_subject_data("PurchasingBudget", 0);
		take_subject_data("ShorttermLoanReceivables", 0);
		take_subject_data("TaxBudget", 0);


			

		//Liability
		take_subject_data("AccruedBonuses", 1);
		take_subject_data("AccruedCorporationsTax", 1);
		take_subject_data("AccruedFixedSalaries", 1);
		take_subject_data("AccruedIncomeTax", 1);
		take_subject_data("DepositsUnderCustody", 1);
		take_subject_data("LongtermDebt", 1);
		take_subject_data("ShorttermDebt", 1);



		//Capital
		take_subject_data("RetainedEarnings", 2);
		take_subject_data("Capital", 2);


		//Exepense
		take_subject_data("CorporateSubsidies", 3);
		take_subject_data("CostSales", 3);
		take_subject_data("FixedSalaries", 3);
		take_subject_data("InterestExpense", 3);
		take_subject_data("InterestExpense(Interest)", 3);
		take_subject_data("ProductionInputCost", 3);
		take_subject_data("Supplies_R", 3);
		take_subject_data("IncomeTax", 3);
		take_subject_data("CorporationsTax", 3);
		take_subject_data("Bonus", 3);


		//Revenue
		take_subject_data("ProductSalesSupplies_C", 4);
		take_subject_data("ProductSalesSupplies_G", 4);
		take_subject_data("ReceivedBonuses", 4);
		take_subject_data("ReceivedCorporationsTax", 4);
		take_subject_data("ReceivedDepositInterest", 4);
		take_subject_data("ReceivedFixedSalary", 4);
		take_subject_data("ReceivedIncomeTax", 4);
		take_subject_data("ReceivedLoanInterest", 4);
		take_subject_data("ReceivedSubsidies", 4);

		


		for (auto ite_EC = mem_EC.begin(); ite_EC != mem_EC.end(); ite_EC++)
		{
			fprintf(simCont.SimConf->Economic_01FreeWriter, "%s: , ", (*ite_EC)->aggregate_name);

			for (auto ite_sd = (*ite_EC)->mem_sd.begin(); ite_sd != (*ite_EC)->mem_sd.end(); ite_sd++)
			{
				fprintf(simCont.SimConf->Economic_01FreeWriter, "%s , %lld , %lld , %lld , %lld , ", (*ite_sd)->kamoku_name, (*ite_sd)->zandaka, (*ite_sd)->increase, (*ite_sd)->decrease, (*ite_sd)->flow);
			}
			fprintf(simCont.SimConf->Economic_01FreeWriter, "\n");
		}
	}

	//Aggregate subject data from each agent's Account and store in SD
	void take_subject_data(const char* kamoku, int kamoku_code)
	{
		long long int max = 0;
		long long int min = 0;
		long long int average = 0;
		
		for (auto ite = mem_EC.begin(); ite != mem_EC.end(); ite++)
		{
			//Get the SD of the relevant subject name.
			auto ite_sd = (*ite)->mem_sd.begin();
			for (auto ite_sd2 = (*ite)->mem_sd.begin(); ite_sd2 != (*ite)->mem_sd.end(); ite_sd2++)
			{
				if (strcmp((*ite_sd2)->kamoku_name, kamoku) == 0)
					ite_sd = ite_sd2;
			}


			//Initialise various data in SD to find the maximum and minimum 
			if ((*ite)->mAcc.begin() != (*ite)->mAcc.end())
			{
				(*ite_sd)->incre_max = (*(*ite)->mAcc.begin())->dataget_subject_data(kamoku, kamoku_code, 0);
				(*ite_sd)->incre_min = (*(*ite)->mAcc.begin())->dataget_subject_data(kamoku, kamoku_code, 0);

				(*ite_sd)->decre_max = (*(*ite)->mAcc.begin())->dataget_subject_data(kamoku, kamoku_code, 1);
				(*ite_sd)->decre_min = (*(*ite)->mAcc.begin())->dataget_subject_data(kamoku, kamoku_code, 1);

				(*ite_sd)->zandaka_max = (*(*ite)->mAcc.begin())->dataget_subject_data(kamoku, kamoku_code, 3);
				(*ite_sd)->zandaka_min = (*(*ite)->mAcc.begin())->dataget_subject_data(kamoku, kamoku_code, 3);
			}

								

			//Extract the subject data corresponding to the acquired SD from the set of Accounts.
			long long int increase = 0, decrease = 0, flow = 0, zandaka = 0;	

			for (auto ite_ac = (*ite)->mAcc.begin(); ite_ac != (*ite)->mAcc.end(); ite_ac++)
			{
				//data extraction
				increase = (*ite_ac)->dataget_subject_data(kamoku, kamoku_code, 0);
				decrease = (*ite_ac)->dataget_subject_data(kamoku, kamoku_code, 1);
				flow = (*ite_ac)->dataget_subject_data(kamoku, kamoku_code, 2);
				zandaka = (*ite_ac)->dataget_subject_data(kamoku, kamoku_code, 3);

				//Additions to total
				(*ite_sd)->increase += increase;
				(*ite_sd)->decrease += decrease;
				(*ite_sd)->flow += flow;
				(*ite_sd)->zandaka += zandaka;

				//Extraction of maximum and minimum values
				if ((*ite_sd)->incre_max < increase)
					(*ite_sd)->incre_max = increase;

				if ((*ite_sd)->incre_min > increase)
					(*ite_sd)->incre_min = increase;

				if ((*ite_sd)->decre_max < decrease)
					(*ite_sd)->decre_max = decrease;

				if ((*ite_sd)->decre_min > decrease)
					(*ite_sd)->decre_min = decrease;

				if ((*ite_sd)->zandaka_max < zandaka)
					(*ite_sd)->zandaka_max = zandaka;

				if ((*ite_sd)->zandaka_min > zandaka)
					(*ite_sd)->zandaka_min = zandaka;

			}
			

			//Average calculation
			(*ite_sd)->incre_ave = double((*ite_sd)->increase) / double((*ite)->mAcc.size());
			(*ite_sd)->decre_ave = double((*ite_sd)->decrease) / double((*ite)->mAcc.size());
			(*ite_sd)->zandaka_ave = double((*ite_sd)->zandaka) / double((*ite)->mAcc.size());
			
		}
		
	}

	//Function to retrieve the required data for mem_sd.
	//0:increase, 1:decrease, 2:flow, 3:zandaka
	long long int getSubjectData(economiccount* EC, const char* subject, int datacode)
	{
		long long int ret_amount = 0;
		bool checker = false;
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end();ite++)
		{
			if (strcmp((*ite)->kamoku_name, subject) == 0)
			{
				checker = true;
				switch (datacode)
				{
				case 0:
					ret_amount = (*ite)->increase;
					break;
				case 1:
					ret_amount = (*ite)->decrease;
					break;
				case 2:
					ret_amount = (*ite)->flow;
					break;
				case 3:
					ret_amount = (*ite)->zandaka;
					break;

				default:
					simCont.ZZZF->ErrorCode("Statistics_EconomicIndicatyor", "getSubjectData()",
						"A non-existent DataCode was sent in as an argument.");
					break;
				}
			}
		}

		if (checker == false)
		{
			printf("%s\n", subject);
			simCont.ZZZF->ErrorCode("Statistics_EconomicIndicatyor", "getSubjectData()",
				"A non-existent subject name is used. There is an error in generation or invocation.");
		}

		return ret_amount;
	}


	void Create_InputOutputData()
	{
		//Preparation of the data required for the input-output table.
		//1.Calculate the data structures that will be needed by taking the respective data from mem_SD in mem_EC.
		//2.Store calculated data in each variable of mem_EC.
		//3.Total value of each agent set (e.g. GDP) is stored in the in_out structure

		for(auto ite=mem_EC.begin();ite!=mem_EC.end();ite++)
		{
			if (strcmp((*ite)->aggregate_name, "retailer") == 0 ||
				strcmp((*ite)->aggregate_name, "wholesaler") == 0 ||
				strcmp((*ite)->aggregate_name, "equipment_maker") == 0 ||
				strcmp((*ite)->aggregate_name, "bank") == 0)
			{
				//Calculation of intermediate demand for each industry sector *This sample code has not been used Product sales between business sectors_Raw materials
				(*ite)->mid_R = getSubjectData(*ite, "NotCurrentlyUsed", 3);
				(*ite)->mid_W = getSubjectData(*ite, "NotCurrentlyUsed", 3);
				(*ite)->mid_E = getSubjectData(*ite, "NotCurrentlyUsed", 3);
				(*ite)->mid_B = getSubjectData(*ite, "NotCurrentlyUsed", 3);
				//Calculation of intermediate demand
				(*ite)->mid_demand = (*ite)->mid_R + (*ite)->mid_W + (*ite)->mid_E + (*ite)->mid_B;

				//Calculation the demand side
					//Creation of non-household consumption expenditure *This sample code has not been used Product sales between business units_Consumables
				(*ite)->out_expenses = getSubjectData(*ite, "NotCurrentlyUsed", 3);
				
				//Calculation of private consumption demand
				//Note: Only banks deduct interest payments from sales, so special treatment.
				if (strcmp((*ite)->aggregate_name, "bank") == 0)
				{
					(*ite)->private_consume = getSubjectData(*ite, "ProductSalesSupplies_C", 3)
						- getSubjectData(*ite, "InterestExpense(Interest)", 3);
				}
				else
				{
					(*ite)->private_consume = getSubjectData(*ite, "ProductSalesSupplies_C", 3) + getSubjectData(*ite, "ReceivedDepositInterest", 3);
				}

				//Calculation of government consumption demand
				(*ite)->public_consume = getSubjectData(*ite, "ProductSalesSupplies_G", 3);

				//Calculation of fixed capital formation *This sample code has not been used Product sales_capital
				(*ite)->fixed_capital = getSubjectData(*ite, "NotCurrentlyUsed", 3);

				//Calculation of product inventories
				(*ite)->stocks = getSubjectData(*ite, "Products",2);

				//Calculation of final demand
				(*ite)->final_demand = (*ite)->out_expenses
					+ (*ite)->private_consume
					+ (*ite)->public_consume
					+ (*ite)->fixed_capital
					+ (*ite)->stocks;


				//Calculation of gross added value
					//Calculation of non-household consumption expenditure
				(*ite)->pay_out_expenses = getSubjectData(*ite, "Supplies_R", 3);

				//Calculation of employer income
				(*ite)->pay_worker = getSubjectData(*ite, "FixedSalaries",0)
					+ getSubjectData(*ite, "Bonus",3);

				//Calculation of operating surpluses
				(*ite)->profit = getSubjectData(*ite, "RetainedEarnings",2)
					+ getSubjectData(*ite, "CorporationsTax",3);

				//Calculation of depreciation
				(*ite)->depreciation = getSubjectData(*ite, "NotCurrentlyUsed", 0);

				//Calculation of indirect taxes
				(*ite)->indirect_tax = 0;

				//Calculation of subsidies
				(*ite)->subsidy = -getSubjectData(*ite, "ReceivedSubsidies",0);

				//Calculation of gross added value
				(*ite)->arahuka = (*ite)->pay_out_expenses
					+ (*ite)->pay_worker
					+ (*ite)->profit
					+ (*ite)->depreciation
					+ (*ite)->indirect_tax
					+ (*ite)->subsidy;

				//Calculation of totals
				in_out->input_r += (*ite)->mid_R;
				in_out->input_w += (*ite)->mid_W;
				in_out->input_e += (*ite)->mid_E;
				in_out->input_b += (*ite)->mid_B;
				in_out->input_T += (*ite)->mid_demand;
				in_out->out_expenses_T += (*ite)->out_expenses;
				in_out->private_consume_T += (*ite)->private_consume;
				in_out->public_consume_T += (*ite)->public_consume;
				in_out->fixed_capital_T += (*ite)->fixed_capital;
				in_out->stock_T += (*ite)->stocks;
				in_out->final_demand_T += (*ite)->final_demand;

				in_out->pay_out_expenses_T += (*ite)->pay_out_expenses;
				in_out->pay_worker_T += (*ite)->pay_worker;
				in_out->profit_T += (*ite)->profit;
				in_out->depreciation_T += (*ite)->depreciation;
				in_out->indirect_tax_T += (*ite)->indirect_tax;
				in_out->subsidy_T += (*ite)->subsidy;
				in_out->GDP_cor += (*ite)->arahuka;
			}
		}
	}

	void Create_InputOutputTable()
	{
		//Outputs of the input-output table
		//What we do: output the required data from mem_EC and in_out in the form of an input-output table.
		//1.Output intermediate input part and row side with create_input_output_table__category
		//2.Output column section

		fprintf(simCont.SimConf->Economic_04InputOutput_Table, "\n\n%d period", simCont.period_count);
		fprintf(simCont.SimConf->Economic_04InputOutput_Table, " , Retailer , Material_maker , Equipment_maker , Bank , Intermediate demand , Expenditure , Private consumption , Government consumption , Capital formation , Stock , Final demand  \n");
		
		//Describes the intermediate input part of the Aggregator and the line side
		fprintf(simCont.SimConf->Economic_03InputOutput_Data, "%d,%lld,%lld", simCont.period_count, in_out->final_demand_T, in_out->GDP_cor);
		Create_InputOutputTableCategory("retailer");
		Create_InputOutputTableCategory("wholesaler");
		Create_InputOutputTableCategory("equipment_maker");
		Create_InputOutputTableCategory("bank");
		fprintf(simCont.SimConf->Economic_03InputOutput_Data, "\n");

		//Describes the total value
		fprintf(simCont.SimConf->Economic_04InputOutput_Table, "Intermediate input , ");
		fprintf(simCont.SimConf->Economic_04InputOutput_Table, "%lld , %lld , %lld , %lld , %lld , %lld , %lld , %lld , %lld , %lld , %lld\n"
			, in_out->input_r, in_out->input_w, in_out->input_e, in_out->input_b, in_out->input_T
			, in_out->out_expenses_T, in_out->private_consume_T, in_out->public_consume_T, in_out->fixed_capital_T, in_out->stock_T, in_out->final_demand_T);

		//Describes the column side of the Aggregator
		fprintf(simCont.SimConf->Economic_04InputOutput_Table, "Expenditure, %lld , %lld , %lld , %lld, %lld\n", mem_EC[1]->pay_out_expenses, mem_EC[2]->pay_out_expenses, mem_EC[3]->pay_out_expenses, mem_EC[4]->pay_out_expenses, in_out->pay_out_expenses_T);

		fprintf(simCont.SimConf->Economic_04InputOutput_Table, "Income of employers, %lld , %lld , %lld , %lld, %lld\n", mem_EC[1]->pay_worker, mem_EC[2]->pay_worker, mem_EC[3]->pay_worker, mem_EC[4]->pay_worker, in_out->pay_worker_T);

		fprintf(simCont.SimConf->Economic_04InputOutput_Table, "Operating surplus, %lld , %lld , %lld , %lld, %lld\n", mem_EC[1]->profit, mem_EC[2]->profit, mem_EC[3]->profit, mem_EC[4]->profit, in_out->profit_T);

		fprintf(simCont.SimConf->Economic_04InputOutput_Table, "Depreciation, %lld , %lld , %lld , %lld, %lld\n", mem_EC[1]->depreciation, mem_EC[2]->depreciation, mem_EC[3]->depreciation, mem_EC[4]->depreciation, in_out->depreciation_T);

		fprintf(simCont.SimConf->Economic_04InputOutput_Table, "Indirect tax, %lld , %lld , %lld , %lld, %lld\n", mem_EC[1]->indirect_tax, mem_EC[2]->indirect_tax, mem_EC[3]->indirect_tax, mem_EC[4]->indirect_tax, in_out->indirect_tax_T);

		fprintf(simCont.SimConf->Economic_04InputOutput_Table, "Subsidy, %lld , %lld , %lld , %lld, %lld\n", mem_EC[1]->subsidy, mem_EC[2]->subsidy, mem_EC[3]->subsidy, mem_EC[4]->subsidy, in_out->subsidy_T);

		fprintf(simCont.SimConf->Economic_04InputOutput_Table, "Gross added value, %lld , %lld , %lld , %lld, %lld\n", mem_EC[1]->arahuka, mem_EC[2]->arahuka, mem_EC[3]->arahuka, mem_EC[4]->arahuka, in_out->GDP_cor);
	}

	
	//Describes the row section of the input-output table for each industry sector
	void Create_InputOutputTableCategory(const char* agg_name)
	{
		//Output column parts of the input-output table.
		int no = 99999;
		for (int i = 0; i<int(mem_EC.size()); i++)
		{
			if (strcmp(mem_EC[i]->aggregate_name, agg_name) == 0)	no = i;
		}

		if (no != 99999)
		{
			//Creation of regular input-output table
			//Describes the name of the Aggregator
			fprintf(simCont.SimConf->Economic_04InputOutput_Table, "%s , ", mem_EC[no]->aggregate_name);
			//Describes intermediate demand
			fprintf(simCont.SimConf->Economic_04InputOutput_Table, "%lld , %lld , %lld , %lld , %lld , ", mem_EC[no]->mid_R, mem_EC[no]->mid_W, mem_EC[no]->mid_E, mem_EC[no]->mid_B, mem_EC[no]->mid_demand);
			//Describes final demand
			fprintf(simCont.SimConf->Economic_04InputOutput_Table, "%lld , %lld , %lld , %lld , %lld , %lld\n", mem_EC[no]->out_expenses, mem_EC[no]->private_consume, mem_EC[no]->public_consume, mem_EC[no]->fixed_capital, mem_EC[no]->stocks, mem_EC[no]->final_demand);

			//Linear representation of data for input-output tables.
			fprintf(simCont.SimConf->Economic_03InputOutput_Data, " , %lld , %lld , %lld , %lld , %lld , ", mem_EC[no]->mid_R, mem_EC[no]->mid_W, mem_EC[no]->mid_E, mem_EC[no]->mid_B, mem_EC[no]->mid_demand);
			fprintf(simCont.SimConf->Economic_03InputOutput_Data, "%lld , %lld , %lld , %lld , %lld , %lld", mem_EC[no]->out_expenses, mem_EC[no]->private_consume, mem_EC[no]->public_consume, mem_EC[no]->fixed_capital, mem_EC[no]->stocks, mem_EC[no]->final_demand);
		}
		else
		{
			printf("Error Fill in the wrong agent string.Å@create_input_output_table__category\n");
			scanf_s("%d", &no);
		}
	}
//Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`
//Description of the input-output table End of entry









//Agent Type Statistics
	void iniStatisticsAgents_control()
	{
		iniStatisticsAgents(mem_EC[0], simCont.SimConf->Economic_10Consumer);
		iniStatisticsAgents(mem_EC[1], simCont.SimConf->Economic_11Retailer);
		iniStatisticsAgents(mem_EC[4], simCont.SimConf->Economic_14Bank);
		iniStatisticsAgents(mem_EC[5], simCont.SimConf->Economic_15Government);

	}
	void iniStatisticsAgents(economiccount* EC, FILE* fi)
	{
		fprintf_s(fi, "period,");
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%s_balance,", (*ite)->kamoku_name);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%s_balance average,", (*ite)->kamoku_name);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%s_balance minimum,", (*ite)->kamoku_name);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%s_balance maximum,", (*ite)->kamoku_name);

		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%s_increase,", (*ite)->kamoku_name);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%s_increase average,", (*ite)->kamoku_name);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%s_increase minimum,", (*ite)->kamoku_name);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%s_increase maximum,", (*ite)->kamoku_name);

		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%s_decrease,", (*ite)->kamoku_name);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%s_decrease average,", (*ite)->kamoku_name);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%s_decrease minimum,", (*ite)->kamoku_name);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%s_decrease maximum,", (*ite)->kamoku_name);

		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%s_flow,", (*ite)->kamoku_name);

		fprintf_s(fi, "\n");
	}
	void writeStatisticsAgents(economiccount *EC, FILE* fi)
	{
		fprintf_s(fi,"%d,",simCont.period_count);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%lld,", (*ite)->zandaka);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%lf,", (*ite)->zandaka_ave);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%lld,", (*ite)->zandaka_min);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%lld,", (*ite)->zandaka_max);

		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%lld,", (*ite)->increase);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%lf,", (*ite)->incre_ave);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%lld,", (*ite)->incre_min);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%lld,", (*ite)->incre_max);

		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%lld,", (*ite)->decrease);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%lf,", (*ite)->decre_ave);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%lld,", (*ite)->decre_min);
		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%lld,", (*ite)->decre_max);

		for (auto ite = EC->mem_sd.begin(); ite != EC->mem_sd.end(); ite++)
			fprintf_s(fi, "%lld,", (*ite)->flow);

		fprintf_s(fi, "\n");
	}






	//Fund-related data statistical output locations.Åi05MoneyAssetÅj
	void iniMoneyAsset()
	{
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset,
			"Periods,Total funds,Money supply,Credit creation, ,Household cash,Corporate cash,Bank cash,Government cash, ,Household deposits,Corporate deposits,Government deposits,Bank deposits,Deposits_deposits_differences, ,Corporate short-term borrowing,Corporate long-term borrowing,Total corporate borrowing,Short-term loan receivables,Long-term loan receivables,Bank loans,Loans_differences, ,Private household cash and deposits Cash and deposits of public households, cash and deposits of lithe, cash and deposits of halls, cash and deposits of equipment, cash and deposits of banks, cash and deposits of the government\n");

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Increase,
			"Periods,Total funds,Money supply,Credit creation, ,Household cash,Corporate cash,Bank cash,Government cash, ,Household deposits,Corporate deposits,Government deposits,Bank deposits,Deposits_deposits_differences, ,Corporate short-term borrowing,Corporate long-term borrowing,Total corporate borrowing,Short-term loan receivables,Long-term loan receivables,Bank loans,Loans_differences, ,Private household cash and deposits Cash and deposits of public households, cash and deposits of lithe, cash and deposits of halls, cash and deposits of equipment, cash and deposits of banks, cash and deposits of the government\n");

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Decrease,
			"Periods,Total funds,Money supply,Credit creation, ,Household cash,Corporate cash,Bank cash,Government cash, ,Household deposits,Corporate deposits,Government deposits,Bank deposits,Deposits_deposits_differences, ,Corporate short-term borrowing,Corporate long-term borrowing,Total corporate borrowing,Short-term loan receivables,Long-term loan receivables,Bank loans,Loans_differences, ,Private household cash and deposits Cash and deposits of public households, cash and deposits of lithe, cash and deposits of halls, cash and deposits of equipment, cash and deposits of banks, cash and deposits of the government\n");

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Flow,
			"Periods,Total funds,Money supply,Credit creation, ,Household cash,Corporate cash,Bank cash,Government cash, ,Household deposits,Corporate deposits,Government deposits,Bank deposits,Deposits_deposits_differences, ,Corporate short-term borrowing,Corporate long-term borrowing,Total corporate borrowing,Short-term loan receivables,Long-term loan receivables,Bank loans,Loans_differences, ,Private household cash and deposits Cash and deposits of public households, cash and deposits of lithe, cash and deposits of halls, cash and deposits of equipment, cash and deposits of banks, cash and deposits of the government\n");


	}

	//Output of fund-related economic data.
	void writeMoneyAsset()
	{
		long long int TotalCapital = 0;
		long long int MoneySupply = 0;
		long long int CreditCreation = 0;

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset, "%d,", simCont.period_count);

		//Total funds || Total funds, money supply, 
		for (auto ite = mem_EC.begin(); ite != mem_EC.end(); ite++)
		{
			TotalCapital += getSubjectData(*ite, "Cash", 3) + getSubjectData(*ite, "Deposits", 3) -  getSubjectData(*ite, "DepositsUnderCustody", 3);
			MoneySupply += getSubjectData(*ite, "Cash", 3) + getSubjectData(*ite, "Deposits", 3);		//Narrow money supply (M1)
		}
		
		CreditCreation = MoneySupply - TotalCapital;		
		
		//Total funding output
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset, "%lld,%lld,%lld, ,", TotalCapital, MoneySupply,CreditCreation);


		//Gross cash section || Household cash, enterprise cash, bank cash, government cash, 
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset,
			"%lld,%lld,%lld,%lld, ,",
			getSubjectData(mem_EC[0], "Cash", 3),
			getSubjectData(mem_EC[1], "Cash", 3) + getSubjectData(mem_EC[2], "Cash", 3) + getSubjectData(mem_EC[3], "Cash", 3),
			getSubjectData(mem_EC[4], "Cash", 3), getSubjectData(mem_EC[5], "Cash", 3));

		//Total deposits||Household deposits, business deposits, government deposits, bank deposits, deposits_deposits_deposits_differences,
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset,
			"%lld,%lld,%lld,%lld,",
			getSubjectData(mem_EC[0], "Deposits", 3),
			getSubjectData(mem_EC[1], "Deposits", 3) + getSubjectData(mem_EC[2], "Deposits", 3) + getSubjectData(mem_EC[3], "Deposits", 3),
			getSubjectData(mem_EC[5], "Deposits", 3), getSubjectData(mem_EC[4], "DepositsUnderCustody", 3));

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset, "%lld, ,",
			getSubjectData(mem_EC[0], "Deposits", 3) + getSubjectData(mem_EC[1], "Deposits", 3) + getSubjectData(mem_EC[2], "Deposits", 3) +
			getSubjectData(mem_EC[3], "Deposits", 3) + getSubjectData(mem_EC[5], "Deposits", 3) - getSubjectData(mem_EC[4], "DepositsUnderCustody", 3));


		//Total loans||enterprises short-term loans,enterprises long-term loans,enterprises total loans,short-term loans receivable,long-term loans receivable,bank loans,borrowing_lending_differences
		long long int Short = getSubjectData(mem_EC[1], "ShorttermDebt", 3) + getSubjectData(mem_EC[2], "ShorttermDebt", 3) + getSubjectData(mem_EC[3], "ShorttermDebt", 3);
		long long int Long = getSubjectData(mem_EC[1], "LongtermDebt", 3) + getSubjectData(mem_EC[2], "LongtermDebt", 3) + getSubjectData(mem_EC[3], "LongtermDebt", 3);
		long long int Lend = getSubjectData(mem_EC[4], "ShorttermLoanReceivables", 3) + getSubjectData(mem_EC[4], "LongtermLoanReceivables", 3);

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset, "%lld,%lld,%lld,", Short, Long, Short + Long);
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset, "%lld,%lld,%lld,%lld, ,",
			getSubjectData(mem_EC[4], "ShorttermLoanReceivables", 3), getSubjectData(mem_EC[4], "LongtermLoanReceivables", 3),
			Lend, Short + Long - Lend);


		//Confirmation of funding bias by agent type.
		//Created only a mould for now, as the households of civil servants are currently in a situation where they do not exist.

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset, "%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",
			getSubjectData(mem_EC[0], "Deposits", 3) + getSubjectData(mem_EC[0], "Cash", 3),
			long long int(0),
			getSubjectData(mem_EC[1], "Deposits", 3) + getSubjectData(mem_EC[1], "Cash", 3),
			getSubjectData(mem_EC[2], "Deposits", 3) + getSubjectData(mem_EC[2], "Cash", 3),
			getSubjectData(mem_EC[3], "Deposits", 3) + getSubjectData(mem_EC[3], "Cash", 3),
			getSubjectData(mem_EC[4], "Deposits", 3),
			getSubjectData(mem_EC[5], "Deposits", 3) + getSubjectData(mem_EC[5], "Cash", 3));
	}



	//Output of fund-related economic data Increase
	void writeMoneyAsset_Increase()
	{
		long long int TotalCapital = 0;
		long long int MoneySupply = 0;
		long long int CreditCreation = 0;

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Increase, "%d,", simCont.period_count);

		for (auto ite = mem_EC.begin(); ite != mem_EC.end(); ite++)
		{
			TotalCapital += getSubjectData(*ite, "Cash", 0) + getSubjectData(*ite, "Deposits", 0) - getSubjectData(*ite, "ShorttermLoanReceivables", 0) - getSubjectData(*ite, "LongtermLoanReceivables", 0);
			MoneySupply += getSubjectData(*ite, "Cash", 0) + getSubjectData(*ite, "Deposits", 0);	
		}

		CreditCreation = MoneySupply - TotalCapital;		
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Increase, "%lld,%lld,%lld, ,", TotalCapital, MoneySupply, CreditCreation);



		
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Increase,
			"%lld,%lld,%lld,%lld, ,",
			getSubjectData(mem_EC[0], "Cash", 0),
			getSubjectData(mem_EC[1], "Cash", 0) + getSubjectData(mem_EC[2], "Cash", 0) + getSubjectData(mem_EC[3], "Cash", 0),
			getSubjectData(mem_EC[4], "Cash", 0), getSubjectData(mem_EC[5], "Cash", 0));

	
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Increase,
			"%lld,%lld,%lld,%lld,",
			getSubjectData(mem_EC[0], "Deposits", 0),
			getSubjectData(mem_EC[1], "Deposits", 0) + getSubjectData(mem_EC[2], "Deposits", 0) + getSubjectData(mem_EC[3], "Deposits", 0),
			getSubjectData(mem_EC[5], "Deposits", 0), getSubjectData(mem_EC[4], "DepositsUnderCustody", 0));

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Increase, "%lld, ,",
			getSubjectData(mem_EC[0], "Deposits", 0) + getSubjectData(mem_EC[1], "Deposits", 0) + getSubjectData(mem_EC[2], "Deposits", 0) +
			getSubjectData(mem_EC[3], "Deposits", 0) + getSubjectData(mem_EC[5], "Deposits", 0) - getSubjectData(mem_EC[4], "DepositsUnderCustody", 0));


		
		long long int Short = getSubjectData(mem_EC[1], "ShorttermDebt", 0) + getSubjectData(mem_EC[2], "ShorttermDebt", 0) + getSubjectData(mem_EC[3], "ShorttermDebt", 0);
		long long int Long = getSubjectData(mem_EC[1], "LongtermDebt", 0) + getSubjectData(mem_EC[2], "LongtermDebt", 0) + getSubjectData(mem_EC[3], "LongtermDebt", 0);
		long long int Lend = getSubjectData(mem_EC[4], "ShorttermLoanReceivables", 0) + getSubjectData(mem_EC[4], "LongtermLoanReceivables", 0);

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Increase, "%lld,%lld,%lld,", Short, Long, Short + Long);
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Increase, "%lld,%lld,%lld,%lld, ,",
			getSubjectData(mem_EC[4], "ShorttermLoanReceivables", 0), getSubjectData(mem_EC[4], "LongtermLoanReceivables", 0),
			Lend, Short + Long - Lend);


		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Increase, "%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",
			getSubjectData(mem_EC[0], "Deposits", 0) + getSubjectData(mem_EC[0], "Cash", 0),
			long long int(0),
			getSubjectData(mem_EC[1], "Deposits", 0) + getSubjectData(mem_EC[1], "Cash", 0),
			getSubjectData(mem_EC[2], "Deposits", 0) + getSubjectData(mem_EC[2], "Cash", 0),
			getSubjectData(mem_EC[3], "Deposits", 0) + getSubjectData(mem_EC[3], "Cash", 0),
			getSubjectData(mem_EC[4], "Deposits", 0),
			getSubjectData(mem_EC[5], "Deposits", 0) + getSubjectData(mem_EC[5], "Cash", 0));
	}



	
	void writeMoneyAsset_Decrease()
	{
		long long int TotalCapital = 0;
		long long int MoneySupply = 0;
		long long int CreditCreation = 0;

		
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Decrease, "%d,", simCont.period_count);


		for (auto ite = mem_EC.begin(); ite != mem_EC.end(); ite++)
		{
			TotalCapital += getSubjectData(*ite, "Cash", 1) + getSubjectData(*ite, "Deposits", 1) - getSubjectData(*ite, "ShorttermLoanReceivables", 1) - getSubjectData(*ite, "LongtermLoanReceivables", 1);
			MoneySupply += getSubjectData(*ite, "Cash", 1) + getSubjectData(*ite, "Deposits", 1);	
		}
		CreditCreation = MoneySupply - TotalCapital;	
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Decrease, "%lld,%lld,%lld, ,", TotalCapital, MoneySupply, CreditCreation);



		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Decrease,
			"%lld,%lld,%lld,%lld, ,",
			getSubjectData(mem_EC[0], "Cash", 1),
			getSubjectData(mem_EC[1], "Cash", 1) + getSubjectData(mem_EC[2], "Cash", 1) + getSubjectData(mem_EC[3], "Cash", 1),
			getSubjectData(mem_EC[4], "Cash", 1), getSubjectData(mem_EC[5], "Cash", 1));

	
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Decrease,
			"%lld,%lld,%lld,%lld,",
			getSubjectData(mem_EC[0], "Deposits", 1),
			getSubjectData(mem_EC[1], "Deposits", 1) + getSubjectData(mem_EC[2], "Deposits", 1) + getSubjectData(mem_EC[3], "Deposits", 1),
			getSubjectData(mem_EC[5], "Deposits", 1), getSubjectData(mem_EC[4], "DepositsUnderCustody", 1));

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Decrease, "%lld, ,",
			getSubjectData(mem_EC[0], "Deposits", 1) + getSubjectData(mem_EC[1], "Deposits", 1) + getSubjectData(mem_EC[2], "Deposits", 1) +
			getSubjectData(mem_EC[3], "Deposits", 1) + getSubjectData(mem_EC[5], "Deposits", 1) - getSubjectData(mem_EC[4], "DepositsUnderCustody", 1));


		
		long long int Short = getSubjectData(mem_EC[1], "ShorttermDebt", 1) + getSubjectData(mem_EC[2], "ShorttermDebt", 1) + getSubjectData(mem_EC[3], "ShorttermDebt", 1);
		long long int Long = getSubjectData(mem_EC[1], "LongtermDebt", 1) + getSubjectData(mem_EC[2], "LongtermDebt", 1) + getSubjectData(mem_EC[3], "LongtermDebt", 1);
		long long int Lend = getSubjectData(mem_EC[4], "ShorttermLoanReceivables", 1) + getSubjectData(mem_EC[4], "LongtermLoanReceivables", 1);

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Decrease, "%lld,%lld,%lld,", Short, Long, Short + Long);
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Decrease, "%lld,%lld,%lld,%lld, ,",
			getSubjectData(mem_EC[4], "ShorttermLoanReceivables", 1), getSubjectData(mem_EC[4], "LongtermLoanReceivables", 1),
			Lend, Short + Long - Lend);



		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Decrease, "%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",
			getSubjectData(mem_EC[0], "Deposits", 1) + getSubjectData(mem_EC[0], "Cash", 1),
			long long int(0),
			getSubjectData(mem_EC[1], "Deposits", 1) + getSubjectData(mem_EC[1], "Cash", 1),
			getSubjectData(mem_EC[2], "Deposits", 1) + getSubjectData(mem_EC[2], "Cash", 1),
			getSubjectData(mem_EC[3], "Deposits", 1) + getSubjectData(mem_EC[3], "Cash", 1),
			getSubjectData(mem_EC[4], "Deposits", 1),
			getSubjectData(mem_EC[5], "Deposits", 1) + getSubjectData(mem_EC[5], "Cash", 1));
	}


	void writeMoneyAsset_Flow()
	{
		long long int TotalCapital = 0;
		long long int MoneySupply = 0;
		long long int CreditCreation = 0;

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Flow, "%d,", simCont.period_count);

		for (auto ite = mem_EC.begin(); ite != mem_EC.end(); ite++)
		{
			TotalCapital += getSubjectData(*ite, "Cash", 2) + getSubjectData(*ite, "Deposits", 2) - getSubjectData(*ite, "ShorttermLoanReceivables", 2) - getSubjectData(*ite, "LongtermLoanReceivables", 2);
			MoneySupply += getSubjectData(*ite, "Cash", 2) + getSubjectData(*ite, "Deposits", 2);		
		}
		CreditCreation = MoneySupply - TotalCapital;		
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Flow, "%lld,%lld,%lld, ,", TotalCapital, MoneySupply, CreditCreation);


 
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Flow,
			"%lld,%lld,%lld,%lld, ,",
			getSubjectData(mem_EC[0], "Cash", 2),
			getSubjectData(mem_EC[1], "Cash", 2) + getSubjectData(mem_EC[2], "Cash", 2) + getSubjectData(mem_EC[3], "Cash", 2),
			getSubjectData(mem_EC[4], "Cash", 2), getSubjectData(mem_EC[5], "Cash", 2));

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Flow,
			"%lld,%lld,%lld,%lld,",
			getSubjectData(mem_EC[0], "Deposits", 2),
			getSubjectData(mem_EC[1], "Deposits", 2) + getSubjectData(mem_EC[2], "Deposits", 2) + getSubjectData(mem_EC[3], "Deposits", 2),
			getSubjectData(mem_EC[5], "Deposits", 2), getSubjectData(mem_EC[4], "DepositsUnderCustody", 2));

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Flow, "%lld, ,",
			getSubjectData(mem_EC[0], "Deposits", 2) + getSubjectData(mem_EC[1], "Deposits", 2) + getSubjectData(mem_EC[2], "Deposits", 2) +
			getSubjectData(mem_EC[3], "Deposits", 2) + getSubjectData(mem_EC[5], "Deposits", 2) - getSubjectData(mem_EC[4], "DepositsUnderCustody", 2));


		long long int Short = getSubjectData(mem_EC[1], "ShorttermDebt", 2) + getSubjectData(mem_EC[2], "ShorttermDebt", 2) + getSubjectData(mem_EC[3], "ShorttermDebt", 2);
		long long int Long = getSubjectData(mem_EC[1], "LongtermDebt", 2) + getSubjectData(mem_EC[2], "LongtermDebt", 2) + getSubjectData(mem_EC[3], "LongtermDebt", 2);
		long long int Lend = getSubjectData(mem_EC[4], "ShorttermLoanReceivables", 2) + getSubjectData(mem_EC[4], "LongtermLoanReceivables", 2);

		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Flow, "%lld,%lld,%lld,", Short, Long, Short + Long);
		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Flow, "%lld,%lld,%lld,%lld, ,",
			getSubjectData(mem_EC[4], "ShorttermLoanReceivables", 2), getSubjectData(mem_EC[4], "LongtermLoanReceivables", 2),
			Lend, Short + Long - Lend);


		fprintf_s(simCont.SimConf->Economic_05MoneyAsset_Flow, "%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",
			getSubjectData(mem_EC[0], "Deposits", 2) + getSubjectData(mem_EC[0], "Cash", 2),
			long long int(0),
			getSubjectData(mem_EC[1], "Deposits", 2) + getSubjectData(mem_EC[1], "Cash", 2),
			getSubjectData(mem_EC[2], "Deposits", 2) + getSubjectData(mem_EC[2], "Cash", 2),
			getSubjectData(mem_EC[3], "Deposits", 2) + getSubjectData(mem_EC[3], "Cash", 2),
			getSubjectData(mem_EC[4], "Deposits", 2),
			getSubjectData(mem_EC[5], "Deposits", 2) + getSubjectData(mem_EC[5], "Cash", 2));
	}



	

};