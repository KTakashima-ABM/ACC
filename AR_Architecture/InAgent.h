#pragma once
#include"Agent.h"

class InAgent //Interface of Agent 
{
public:
	virtual int getAgentId() = 0;
	virtual int getAgentType() = 0;
	virtual void RecieveTradeMasseage(const char* transaction_name, accounting_message* pal_acme, int sendrece_code, long long int deal_amount) = 0;
	virtual void RecieveTradeMasseage(const char* transaction_name, accounting_message* pal_acme, int sendrece_code, vector<long long int> deal_amount) = 0;
	virtual accounting_message* Get_accounting_message() = 0;

};
