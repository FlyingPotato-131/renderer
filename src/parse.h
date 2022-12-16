#pragma once
#include <vector>
#include "structs.h"
#include "funcs.h"
using namespace std;

int main()
{
	string znak;
	string vector;
	fstream F;
	F.open("jopa.obj");
	while (!F.eof())
	{
		F>>znak;
		if (znak == 'v')
		{
			get.line(F, vector)
			cout<< vector;
		}
	
	}
}








