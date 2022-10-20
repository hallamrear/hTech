// coroutines.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stack>
#include <iostream>
#include "Coroutine.h"

void Print(Coroutine* c)
{
    int* t = new int();
    //Coroutine c runs
    printf("Hello ");
    //coroutine yields control back to main
    Coroutine::Yield(c);
    printf(" World!\n");
}

int main()
{
    //Creates coroutine and does not start it.
    auto c = Coroutine::Create(Print);
    Print(&c); 
   
    while (1)
    {
        float dt = 0.00016;
        //Coroutine::Update(dt);

        if(Coroutine::Status(c) != COROUTINE_STATE::DEAD || Coroutine::Status(c) != COROUTINE_STATE::SUSPENDED)
        {
            //Starts coroutine c.
            Coroutine::Resume(c);
            //After yield in coroutine, this line runs
            printf(" uhhhhhhh ");
            //Coroutine is resumed from previous yield.
            Coroutine::Resume(c);
        }
    }

    std::cout << "\nHello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file