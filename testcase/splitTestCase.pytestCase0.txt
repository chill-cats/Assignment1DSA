#!python
import sys


def main(filename):
    with open(filename) as f:
        totalFileContent = f.read()
        for index, content in enumerate(totalFileContent.split("\n===========\n")):
            file = open(filename + "testCase" + str(index) + ".txt", "w")
            file.write(content)
            file.close()


if __name__ == '__main__':
    for arg in sys.argv:
        main(arg)