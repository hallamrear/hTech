a = 7 + 11 + math.sin(23.7)
a = a + 50;
a = a + 50;
a = a + 50;
a = a + 50;
playername = "test player name";

--2 ways to define a variable.
--id pairs e.g. [string] = variable
--player = {}
--player["title"] = "squire"
--player["name"] = "keith"
--player["family"] = "brent"
--player["level"] = 40

--OR class initialising

players = {}
players[0] = { title = "alt", name = "marv", family = "look", level = 12}
players[1] = { title = "1", name = "1", family = "1", level = 1}

function AddStuff( a, b )
	--just a visual thing that is not necessary,
	--..a.. concatenates a to the string.
	--print runs to the win32 console for the program.
	print("[LUA] AddStuff("..a..", "..b..") called \n");
	return a+b;
end

function getplayer(n)
	return players[n];
end


function DoCPPFunction(a, b)
	print("[LUA] DoCPPFunction("..a..", "..b..") called \n")
	c = HostFunction(a + 1, b)
	return c
end