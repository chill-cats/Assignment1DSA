import os
import subprocess
import sys

programPath = sys.argv[1]
testPath = sys.argv[2]

files = [os.path.join(testPath, f) for f in os.listdir(testPath) if os.path.isfile(os.path.join(testPath, f))]
for idx, file in enumerate(files):
    print('\x1bc')
    print(f'\nRunning file #{idx}: {file}')
    returnCode = subprocess.run(
        [programPath, file], env=dict(os.environ, ASAN_OPTIONS="detect_leaks=1",
                                                                 ASAN_SYMBOLIZER_PATH="/usr/local/opt/llvm/bin/llvm"
                                                                                         "-symbolizer")).returncode;
    print("Test finished with status code %d" % returnCode)
    if returnCode != 0:
        print("\e[1m\e[31mERROR:\e[0m in file %s" % file)
