#pragma once

#ifndef HTECH_FUNCTION_EXPORT
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)
#else
#define HTECH_FUNCTION_EXPORT __declspec(dllimport)
#endif

#define EndScript(C) ;
#define CreateScript(C) public:	inline static __declspec(dllexport) Script* __cdecl Create(void) { return new C(); } 

#define StartScript(C) class C : public Script

#define publicFunction _publicFunction
#define _publicFunction public: inline void
#define privateFunction _privateFunction
#define _privateFunction private: inline void

class Script
{
public:
	virtual void Update(float deltaTime) = 0;

	virtual void Destroy()
	{
		delete this;
	}
};