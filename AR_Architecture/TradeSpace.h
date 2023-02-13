#pragma once

#include<cstdio>
#include<vector>
#include<iostream>

#include"InAgent.h"

using namespace std;

extern SimulatorContext simCont;


class TradeSpace
{
	//Space for sending messages to each other via interfaces to prevent cross-referencing between agents.
	//An example is the Market, which is considered to exist as such a space.
	//"Model building and description using the agent-based computational economics framework for accounting", for example, 
	//let accounting messages and object transactions take place over the interfaces Seller (Firm) and Buyer (Household) via Item, 
	//in addition to this kind of space.
	
public:
	InAgent* go;
	InAgent* ba;
	vector<InAgent*>mHou;
	vector<InAgent*>mFirm;


	TradeSpace()
	{

	}
};


