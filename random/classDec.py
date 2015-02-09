className = raw_input("> Class Name: ")
tabCnt = int(raw_input("> # Spaces per Tab: "))

output = []

def newLine(text, indent = 0):
    global output, tabCnt
    spaces = " " * indent * tabCnt
    output.append(spaces + text)
    
newLine("#ifndef _" + className + "_h_")
newLine("#define _" + className + "_h_")
newLine("")
newLine("class " + className)
newLine("{")
newLine("public:", 1)
newLine(className + "();", 2)
newLine("virtual ~" + className + "();", 2)
newLine("};")
newLine("")
newLine("#endif // _" + className + "_h_")

outFile = open(className + ".h", "w")
outFile.writelines('\n'.join(output))
outFile.close()
