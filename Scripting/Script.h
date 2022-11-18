#pragma once

#define StartScript(C) _StartScript(C)
#define _StartScript(C) class C : public Script
#define EndScript(C) _EndScript(C)
#define _EndScript(C) ;extern "C" { __declspec(dllexport) Script* CreateScript()	{ return new C(); } }
#define publicFunction _publicFunction
#define _publicFunction public: inline void
#define privateFunction _privateFunction
#define _privateFunction private: inline void

class Script
{
public:
	virtual void Update(float deltaTime) = 0;
};
