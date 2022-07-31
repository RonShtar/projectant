all: run
run:
	gcc sniffer.c -o sniffer
	sudo ./sniffer
