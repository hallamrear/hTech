
function CallCPPFunctionInLuaFunction(host, exampleIndex)
print("[LUA] CallCPPFunctionInLuaFunction called")

--using an underscore to show that it is a cpp function call.
_CPPFunction(host, exampleIndex);

end