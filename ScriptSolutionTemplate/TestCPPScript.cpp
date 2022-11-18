#pragma once
#include <iostream>
#include <Scripting/Script.h>

//custom defines in script.h just to make it look more like an actual custom script
//and to hide the extern tbh
StartScript(ExampleScript)
{
	publicFunction Update(float deltaTime)
	{
		Test();
	}

	privateFunction Test()
	{
		std::cout << "function really changed22444444444444442222222222!" << std::endl;
	}
}
EndScript(ExampleScript)