from os import system

TEST_LIST = [
    "disk0.18entries.empty",
    "disk0.1F.1497B",
    "disk0.1F.512B",
    "disk0.1F.512B.A+B",
    "disk0.2F.0B",
    "disk0.2F.1497B+64B",
    "disk0.4entries.empty",
    "disk0.unformatted"
]

NUMBER_OF_TESTS = 8
COMMAND_RUN = "cd src/ ; ./dumpBFS ../data/%s &> ../output/%s_out.txt"
COMMAND = "cd src/ ; make ; cd .."

system(COMMAND)

for i in range(0, NUMBER_OF_TESTS):
    system(COMMAND_RUN % (TEST_LIST[i], TEST_LIST[i]))
    out = system("diff output/%s_out.txt data/%s.out" %
                 (TEST_LIST[i], TEST_LIST[i]))
    if out != 0:
        print("------------------------")
        print("       FAILED %.2s        " % str(i+1))
        print("------------------------")
        exit(0)
    else:
        print("------------------------")
        print("       PASSED %.2s        " % str(i+1))
        print("------------------------")
