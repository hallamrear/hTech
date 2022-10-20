#pragma once

#include <functional>
#include <list>
#include <stack>

enum class COROUTINE_STATE
{
	RUNNING, //Running normally
	SUSPENDED, //Paused or not launched yet
	NORMAL, //Active, no faults and not running
	DEAD	//Coroutine has finished.
};

enum class COROUTINE_ERROR
{
	ALL_OK,
	UNRESOLVED,
};

class Coroutine
{
#define CoroutineFunc std::function<void(Coroutine*)>

private:
	static std::stack<CoroutineFunc> mCoroutineSubstack;
	static std::list<Coroutine*> mRoutines;

	CoroutineFunc mFunction;
	COROUTINE_STATE State;

	Coroutine(CoroutineFunc function);

	static Coroutine* PopCoroutineStack();
	static void PushCoroutineStack(Coroutine* routine);

public:

	/*
	* MAKE
	* DESTROY
	* RESUME
	* YIELD
	* WAIT
	*/
	~Coroutine();

	/// <summary>
	/// Creates a new coroutine.
	/// </summary>
	/// <param name="function">The function for the coroutine to run.</param>
	/// <returns>New coroutine instance.</returns>
	static Coroutine Create(CoroutineFunc function);

	/// <summary>
	/// Resumes a paused coroutine.
	/// </summary>
	/// <param name="coroutine">Coroutine to resume</param>
	/// <returns>Returns appropriate error code. See COROUTINE_ERROR</returns>
	static COROUTINE_ERROR Resume(Coroutine coroutine);

	/// <summary>
	/// Pauses coroutine for a number of seconds.
	/// </summary>
	/// <param name="coroutine">Coroutine to pause.</param>
	/// <param name="seconds">Number of seconds to pause for.</param>
	/// <returns>Returns appropriate error code. See COROUTINE_ERROR</returns>
	static COROUTINE_ERROR Wait(Coroutine coroutine, float seconds);

	/// <summary>
	/// Pauses the currently runnning coroutine.
	/// </summary>
	static void Yield(Coroutine* func);


	static COROUTINE_STATE Status(Coroutine coroutine);

	//static void Update(float DeltaTime);

};