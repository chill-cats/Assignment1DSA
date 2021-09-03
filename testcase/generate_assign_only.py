import os
import re

scriptDir = os.getcwd()
reg = re.compile("^ASSIGN.*$")

def main():
    os.chdir(os.path.join(scriptDir, "tests"))

    currentDir = os.getcwd()
    files = [f for f in os.listdir(currentDir) if os.path.isfile(os.path.join(currentDir, f))]
    for file in files:
        f = open(file)
        fileContent = f.readlines();
        insertOnlyFileContent = list(filter(reg.match, fileContent))
        oldCWD = os.getcwd()
        os.chdir(os.path.join(scriptDir, "ASSIGN_ONLY_tests"))
        fileWrite = open("ASSIGN_ONLY_" + file, "w")
        fileWrite.writelines(insertOnlyFileContent)
        fileWrite.close()
        os.chdir(oldCWD)


if __name__ == '__main__':
    main()