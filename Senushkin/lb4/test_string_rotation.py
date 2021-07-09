import os

DIRECTORY = "Tests_string_rotation"

tests = os.listdir(DIRECTORY)

for i in range(len(tests)):
    testPath = DIRECTORY + "/" + tests[i]
    print()
    print("Test ", i+1, ":   ", testPath, sep="")
    print(*open(testPath, "r").readlines(), sep="")
    os.system("./string_rotation_Lb4 < ./" + testPath)
