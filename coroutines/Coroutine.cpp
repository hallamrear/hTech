#include "Coroutine.h"

std::list<Coroutine*> Coroutine::mRoutines;

//int l = __LINE__; //Tracks Line number.

Coroutine::Coroutine(CoroutineFunc function)
{
    mFunction = function;
}

Coroutine::~Coroutine()
{

}

Coroutine Coroutine::Create(CoroutineFunc function)
{
    mRoutines.push_back(new Coroutine(function));
    return *mRoutines.back();
}

COROUTINE_ERROR Coroutine::Resume(Coroutine coroutine)
{
    //Coroutines are paused when constructed, or when they call yield.
    COROUTINE_ERROR error = COROUTINE_ERROR::ALL_OK;

    coroutine.State = COROUTINE_STATE::SUSPENDED;

    return error;
}

void Coroutine::Yield(Coroutine* coroutine)
{
    //Coroutines are paused when constructed, or when they call yield.
    //coroutine.State = COROUTINE_STATE::SUSPENDED;
}

COROUTINE_STATE Coroutine::Status(Coroutine coroutine)
{
    return coroutine.State;
}

//void* Coroutine::operator new(size_t size)
//{
//    return nullptr;
//}
//
//void Coroutine::operator delete(void* addr)
//{
// 
//}