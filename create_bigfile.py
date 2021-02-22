file = open("bigfile.txt", "w")

fileSize = 1024 * 1024

string = "Hello, world. this is string from big file\n"

for i in range(fileSize):
    file.write(string)

file.close()
