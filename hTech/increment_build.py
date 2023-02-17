f = open("BUILD_NUMBER.h", "r+")
text = f.read()

split = text.rsplit(' ', 1) # split by last space

number_str = split[1][:-1] # without ; at the end

number = int(number_str) + 1 # INCREMENT!

final = split[0] + " " + str(number) + ";"

f.truncate(0) # clear
f.seek(0)
f.write(final)
f.close()

print("Incremented build number to: " + str(number))