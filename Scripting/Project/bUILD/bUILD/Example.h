#pragma once
#include <iostream>
#include "Scripting/Script.h"

//custom defines in script.h just to make it look more like an actual custom script
//and to hide the extern tbh
StartScript(Example)
{
	CreateScript(Example);

	publicFunction Update(float deltaTime)
	{
		std::cout << "test class 1" << std::endl;
	}
}
EndScript(Example)