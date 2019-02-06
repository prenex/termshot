all:
	gcc termshot.c -O2 -o termshot.out
debug:
	gcc termshot.c -g -o termshot.out
install:
	cp -p termshot.out /usr/local/bin/termshot
