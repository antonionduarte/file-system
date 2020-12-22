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
COMMAND_RUN = "cd src/ ; ./fsckBFS ../data/%s"
COMMAND = "cd src/ ; make ; cd .."

system(COMMAND)

for i in range(0, NUMBER_OF_TESTS):
    print("Testing disk: " + TEST_LIST[i])
    system(COMMAND_RUN % (TEST_LIST[i]))
