#pragma once
#include <iostream>
#include "Script.h"

//custom defines in script.h just to make it look more like an actual custom script
//and to hide the extern tbh
StartScript(ExampleScript)
{
public:
	inline void Update(float deltaTime) override
	{
		std::cout << "test" << std::endl;
	}
}
EndScript(ExampleScript)