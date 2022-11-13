// Scripting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

// Lua is fundamentally a C system so we need to let the compiler know that.
extern "C"
{
#include "LUA/include/lua.h"
#include "LUA/include/lualib.h"
#include "LUA/include/lauxlib.h"

//load lib for example
#ifdef _WIN32
#pragma comment(lib, "LUA/liblua54.a")
#endif
}

//Checking VM state
bool CheckLua(lua_State* L, int r)
{
    if (r != LUA_OK)
    {
        std::string err = lua_tostring(L, -1);
        std::cout << err << std::endl;
        return false;
    }

    return true;
}

int main()
{
    std::string command = "a = 7 + 11 + math.sin(23.7)";

    //To do anything in lua, you need a lua state.
    //Lua uses a captial L to repressent a state.
    lua_State* state = luaL_newstate();

    //adds std libraries to machine state (so we can use math.sin for example)
    luaL_openlibs(state);

    //tell lua to execute command.
    if (CheckLua(state, luaL_dostring(state, command.c_str())))
    {
        //a exists in the lua global machine
        lua_getglobal(state, "a");

        //lua is reasonably typeless and so we need to validate A's type.
        //expected to be a number

        if (lua_isnumber(state, -1))
        {
            //convert whatever a is to a number
            float a_cpp = (float)lua_tonumber(state, -1);
            std::cout << a_cpp << std::endl;
        }
    }

    //using the same command means we cna sequence commands. 
    command = "a = a + 100";

    //tell lua to execute command.
    if (CheckLua(state, luaL_dostring(state, command.c_str())))
    {
        //a exists in the lua global machine
        lua_getglobal(state, "a");

        //lua is reasonably typeless and so we need to validate A's type.
        //expected to be a number

        if (lua_isnumber(state, -1))
        {
            //convert whatever a is to a number
            float a_cpp = (float)lua_tonumber(state, -1);
            std::cout << a_cpp << std::endl;
        }
    }


    //cleanup lua vm
    system("pause");
    lua_close(state);

    std::cout << "Hello World!\n";
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


//LUA IS A Stack based language
/*
we can use indexes to reference parts of the lua stack.

when we call lua_getglobal(vm, variable)
    we put a into the stack, lua pops a, and pushes the value defining 'variable'



stack is indexed starting from zero at the bottom which is always empty.
we can also index relative to our current position, so using index -1, -2 etc.. drops us down the stack.
-1 is always the top of the stack

*/