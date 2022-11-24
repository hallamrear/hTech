--create a coroutine with coroutine.create(CoroutineFunction)
test = nil

function CallCPPFunctionInLuaFunction(host, exampleIndex)
print("[LUA] CallCPPFunctionInLuaFunction called")

--using an underscore to show that it is a cpp function call.
_CPPFunction(host, exampleIndex);

end

function CreateCoroutine()
	test = coroutine.create(CoroutineFunction) 
	print("Created coroutine")
end


function CallCoroutine()
	print("CallCoroutine()")
	--a dead coroutine has exited its function.
	--CoroutineFunction cannot exist atm so cannot be dead
	if coroutine.status(test) ~= 'dead' then
		print("coroutine.resume")
		coroutine.resume(test)
	end
end

function CoroutineFunction()
	while true do
		print("1")
		--function will stop at the yield until it is signalled to run again
		coroutine.yield()
		print("2")
		coroutine.yield()
		print("3")
		coroutine.yield()
		print("4")
		coroutine.yield()
	end
end
