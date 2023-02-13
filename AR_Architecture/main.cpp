#include<cstdio>
#include<iostream>
#include"SimulatorApp.h"

SimulatorContext simCont;

int main()
{
	cout << "Start" << endl;
	SimulatorApp* App;
	App = new SimulatorApp();
	App->Set_up();

	cout << "End" << endl;
}


