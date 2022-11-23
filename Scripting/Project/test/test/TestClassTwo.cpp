#pragma once
#include <iostream>
#include "Scripting/Script.h"

//custom defines in script.h just to make it look more like an actual custom script
//and to hide the extern tbh
StartScript(TestClassTwo)
{
	CreateScript(TestClassTwo);

	publicFunction Update(float deltaTime)
	{
		std::cout << "testClass 2222222222222!" << std::endl;
	}
}
EndScript(TestClassTwo)