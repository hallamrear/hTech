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

//creating a fucntion callable by lua it needs a specific definition
//using lua_ as a prefix to show its callable from there.
//takes a single state argument
//returns teh number of arguements passed back to lua.
//e.g. 1, C
int lua_HostFunction(lua_State* L)
{
    //not indexing -1, -2 
    //as if its a fresh stack that already has the values pushed onto it.
    float a = (float)lua_tonumber(L, 1);
    float b = (float)lua_tonumber(L, 2);

    std::cout << "C++ function has been called" << std::endl;
    float result = a + b;
    //push back onto the stack.
    lua_pushnumber(L, result);
    return 1;
}

struct CallableStruct
{
    //the wrapper function lua will be able to see.
    //we need to register this function with lua however.
    static int wrap_ToBeCalledFromLUA(lua_State* L)
    {
        //checking we have been passed the correct number of arguments
        if (lua_gettop(L) != 2) return -1;

        //the first object on the stack is now the 'host' argument i.e a pointer to one of these objects
        //therefore
        CallableStruct* object = static_cast<CallableStruct*>(lua_touserdata(L, 1));
        int exampleIndex = lua_tointeger(L, 2);
        object->ToBeCalledFromLUA();

        //we are not returning anything to lua.
        return 0;
    }

    void ToBeCalledFromLUA()
    {
        printf("CPP - void ToBeCalledFromLUA()");
    }

    void RunLua(lua_State* L)
    {
        //call to register the functions.
        if (CheckLua(L, luaL_dofile(L, "Example2.lua")))
        {
            //getting funciton and putting at top of stack.
            lua_getglobal(L , "CallCPPFunctionInLuaFunction");

            //validating
            if (lua_isfunction(L, -1))
            {
                //this is effectively pushing a pointer for this object.
                lua_pushlightuserdata(L, this);
                //push second parameter for function
                lua_pushnumber(L, 123);

                /// <summary>
                /// 2 arguments,
                /// 1 return,
                /// 0 is for error flags
                /// </summary>
                /// <param name="L"></param>
                /// CALLS THE FUNCTION
                if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
                {
                    std::cout << "[CPP] has called the member function of CallableStruct" << std::endl;
                }
            }
        }
    }
};

int main()
{
    struct player
    {
        std::string title;
        std::string name;
        std::string family;
        int level = 0;

        void print()
        {
            std::cout << "family: " << family << std::endl;
            std::cout << "name: " << name << std::endl;
            std::cout << "title: " << title << std::endl;
            std::cout << "level: " << level << std::endl;
        }
    } mPlayer;


    //To do anything in lua, you need a lua state.
    //Lua uses a captial L to repressent a state.
    lua_State* state = luaL_newstate();
    float result = 0.0f;

    //adds std libraries to machine state (so we can use math.sin for example)
    luaL_openlibs(state);

    /// <summary>
    /// string name that lua uses to call the function
    /// function ptr to the function
    /// </summary>
    /// <returns></returns>
    lua_register(state, "HostFunction", lua_HostFunction);

    //tell lua to execute command.
    //do file to open a file.
    if (CheckLua(state, luaL_dofile(state, "example.lua")))
    {
        //a exists in the lua global machine
        //we are now looking for a global object called player
        lua_getglobal(state, "player");

        //getting a function from the file.
        lua_getglobal(state, "DoCPPFunction");
        //checking its real
        if (lua_isfunction(state, -1))
        {
            //pushing 2 numbers to the stack to use in the function call.
            lua_pushnumber(state, 4);
            lua_pushnumber(state, 5);

            /// <summary>
            /// 2 == how many arguments expected
            /// 1 == how many arguments returned
            /// 0 == defines how error is handled in lua script.
            /// </summary>
            /// <returns></returns>
            if (CheckLua(state, lua_pcall(state, 2, 1, 0)))
            {
                //we told it to expect 2 so it would have popped them.
                //it will have pushed the 1 result to the top of the stack.
                result = (float)lua_tonumber(state, -1);
                std::cout << "function calls result = " << result << std::endl;
            }
        }

        //Getting a player from the file.
        lua_getglobal(state, "getplayer");
        if (lua_isfunction(state, -1))
        {
            lua_pushnumber(state, 0);

            if (CheckLua(state, lua_pcall(state, 1, 1, 0)))
            {
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

                    mPlayer.print();
                }
            }
        }



        //registering the function to be called from lua later.
        lua_register(state, "_CPPFunction", CallableStruct::wrap_ToBeCalledFromLUA);

        //calling a cpp member function from lua.
        CallableStruct object;
        object.RunLua(state);

















       
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