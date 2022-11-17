#pragma once

#define StartScript(C) class C : public Script
#define EndScript(C) ;extern "C" { __declspec(dllexport) Script* CreateScript()	{ return new C(); } }

class Script
{
public:
	virtual void Update(float deltaTime)
	{
		std::cout << ("original script") << std::endl;
	}
};
