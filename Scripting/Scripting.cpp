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
    struct player
    {
        std::string title;
        std::string name;
        std::string family;
        int level;
    } mPlayer;


    //To do anything in lua, you need a lua state.
    //Lua uses a captial L to repressent a state.
    lua_State* state = luaL_newstate();

    //adds std libraries to machine state (so we can use math.sin for example)
    luaL_openlibs(state);

    //tell lua to execute command.
    //do file to open a file.
    if (CheckLua(state, luaL_dofile(state, "example.lua")))
    {
        //a exists in the lua global machine
        //we are now looking for a global object called player
        lua_getglobal(state, "player");

        //lua is reasonably typeless and so we need to validate playername's type.
        //player is no longer a simple type and so we need to define it as a table of variables.

        //-1 indicates its at the top of the stack.
        if (lua_istable(state, -1))
        {
            {
            //this pushes the string to the top of the stack
            lua_pushstring(state, "name");
            //making the index for the table -2 as it is no longer at the top.
            lua_gettable(state, -2);

            //looks at top parameter, finds key value, pops name off stack and replaces with value we're looking for.
            //top of stack now contains the name we are searching for.
            mPlayer.name = lua_tostring(state, -1);
            //fixes stack arrangement
            lua_pop(state, 1);
            }

            //this is just multiple version of what is happening above
            lua_pushstring(state, "title");
            lua_gettable(state, -2);
            mPlayer.title = lua_tostring(state, -1);
            lua_pop(state, 1);
            lua_pushstring(state, "family");
            lua_gettable(state, -2);
            mPlayer.family = lua_tostring(state, -1);
            lua_pop(state, 1);
            lua_pushstring(state, "level");
            lua_gettable(state, -2);
            mPlayer.level = lua_tonumber(state, -1);
            lua_pop(state, 1);

            std::cout << "family: " << mPlayer.family << std::endl;
            std::cout << "name: " << mPlayer.name << std::endl;
            std::cout << "title: " << mPlayer.title << std::endl;
            std::cout << "level: " << mPlayer.level << std::endl;
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