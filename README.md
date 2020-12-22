# file-system

A file system simulation made in C.
In the `src` directory is the source code of the project.
In the `data` directory are examples of "disks" to run the code on.
There are two python scripts to easily run the code on all the disks provided. 

## Compilation

Compile using the provided makefile.

* If you with to compile the fsckBFS (file system checker):
	Edit the makefile, uncomment the line (might be by default), regarding fsckBFS
	comment the one regarding dumpBFS, and run `make`
	on the src directory.

* If you wish to compile the dumpBFS:
	Edit the makefile, uncomment the line (might be by default), regarding dumpBFS
	comment the one regarding fsckBFS, and run `make`
	on the src directory.

## Running

* If you want to run dumpBFS, after compiling, in the src directory run `./dumpBFS <disk>`
	To easily run it on all the data disks provided, in the root directory run `python3 test-parser.py`

* If you want to run fsckBFS, after compiling, in the src directory run `./fsckBFS <disk>`
	To easily run it on all the data disks provided, in the root directory run `python3 check-integrity`
