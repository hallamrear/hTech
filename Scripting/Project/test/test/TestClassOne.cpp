#pragma once
#include <iostream>
#include "Scripting/Script.h"

//custom defines in script.h just to make it look more like an actual custom script
//and to hide the extern tbh
StartScript(TestClassOne)
{
	//CreateScript(TestClassOne);

	publicFunction Update(float deltaTime)
	{
		std::cout << "testClass 111111111111!" << std::endl;
	}
}
EndScript(TestClassOne)