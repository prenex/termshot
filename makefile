all:
	gcc termshot.c -O2 -o termshot
debug:
	gcc termshot.c -g -o termshot
install:
	cp -p termshot /usr/local/bin/
