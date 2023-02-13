#pragma once
#include<cstdio>
#include<iostream>
#include<vector>

class ZZZ_Func		//Small functions used in programming assumptions, such as pause mechanisms, outputs, etc.
{
public:
	ZZZ_Func();
	~ZZZ_Func();

	void stop_function(const char* str)
	{
		int dummy;
		printf("\n\nÅ´Å´Å´Message below stop branch occurrenceÅ´Å´Å´\n\n\n%s\n", str);
		scanf_s("%d", &dummy);
	}

	int comparison_int(int x, int y)
	{
		int z;
		if (x > y) z = x;
		else z = y;
		return z;
	}

	


	void ErrorCode(const char* classname,const char* methodname, const char* detail)
	{
		printf_s("\n\n\n");
		printf_s("Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°\n");
		printf_s("Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°\n");
		printf_s("Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°\n");
		printf_s("\n\n\n");

		printf_s("%s  :  %s     called!!!\n", classname, methodname);
		printf_s("%s\n",detail);

		printf_s("\n\n\n");
		printf_s("Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°\n");
		printf_s("Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°\n");
		printf_s("Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°\n");
		printf_s("\n\n\n");
		exit(1);
	}



private:

};

inline ZZZ_Func::ZZZ_Func()
{

}

ZZZ_Func::~ZZZ_Func()
{
}