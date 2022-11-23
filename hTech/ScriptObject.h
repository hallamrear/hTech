#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#define EndScript(C) _EndScript(C)
#define StartScript(C) _StartScript(C)
#define _EndScript(C) ; extern "C" {  inline HTECH_FUNCTION_EXPORT ScriptObject* CREATE_FUNCTION(C) { return new C(); } }
#define _StartScript(C) class HTECH_FUNCTION_EXPORT C : public ScriptObject

#define publicFunction _publicFunction
#define _publicFunction public: inline void
#define privateFunction _privateFunction
#define _privateFunction private: inline void

#define Create_
#define MAKE_FN_NAME(CLASS) Create_##CLASS(void)
#define CREATE_FUNCTION(CLASS) MAKE_FN_NAME(CLASS)

class HTECH_FUNCTION_EXPORT ScriptObject
{
public:
	virtual void Update(float deltaTime) = 0;

	virtual void Destroy()
	{
		delete this;
	}
};

