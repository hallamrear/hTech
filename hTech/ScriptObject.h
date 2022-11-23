#pragma once

#ifndef HTECH_FUNCTION_EXPORT
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)
#else
#define HTECH_FUNCTION_EXPORT __declspec(dllimport)
#endif

#define EndScript(C) _EndScript(C)
#define StartScript(C) _StartScript(C)
#define _EndScript(C) ; extern "C" {  inline HTECH_FUNCTION_EXPORT Script* CREATE_FUNCTION(C) { return new C(); } }
#define _StartScript(C) class C : public Script

#define publicFunction _publicFunction
#define _publicFunction public: inline void
#define privateFunction _privateFunction
#define _privateFunction private: inline void

#define Create_
#define MAKE_FN_NAME(CLASS) Create_##CLASS(void)
#define CREATE_FUNCTION(CLASS) MAKE_FN_NAME(CLASS)

class ScriptObject
{
public:
	virtual void Update(float deltaTime) = 0;

	virtual void Destroy()
	{
		delete this;
	}
};

