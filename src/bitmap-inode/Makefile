CFLAGS = -Wall

tpc3: tpc3.o disk_driver.o tpc3_bytemap.o tpc3_inode.o
	gcc -o tpc3 tpc3.o disk_driver.o tpc3_bytemap.o tpc3_inode.o -lm

tpc3.o: tpc3.c
	gcc $(CFLAGS) -c tpc3.c

disk_driver.o: disk_driver.c disk_driver.h tpc3_errno.h
	gcc $(CFLAGS) -c disk_driver.c

tpc3_bytemap.o: tpc3_bytemap.c tpc3_bytemap.h tpc3_errno.h
	gcc $(CFLAGS) -c tpc3_bytemap.c

tpc3_inode.o: tpc3_inode.c tpc3_inode.h tpc3_errno.h
	gcc $(CFLAGS) -c tpc3_inode.c

clean:
	rm tpc3 tpc3.o disk_driver.o tpc3_bytemap.o tpc3_inode.o
