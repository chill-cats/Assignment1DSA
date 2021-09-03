import os
import subprocess
import sys

programPath = sys.argv[1]
testPath = sys.argv[2]

files = [os.path.join(testPath, f) for f in os.listdir(testPath) if os.path.isfile(os.path.join(testPath, f))]
for file in files:
    print("Running file %s" % file)
    hasError = subprocess.run(
        [programPath, file], stdout=subprocess.DEVNULL, env=dict(os.environ, ASAN_OPTIONS="detect_leaks=1",
                                                                 ASAN_SYMBOLIZER_PATH="/usr/local/opt/llvm/bin/llvm"
                                                                                      "-symbolizer")) == 1
    if hasError:
        print("Memory leak detect in test case %s" % file)
        exit(1)
    else:
        print("Testcase %s ok!" % file)