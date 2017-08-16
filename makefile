cc=gcc
bin=httpd
src=$(shell ls *.c)
LDFLAGS=-lpthread

$(bin):$(src)
	$(cc) -o $@ $^  $(LDFLAGS) -lrt 

.PHONY:debug
debug:
	echo $(src)

.PHONY:clean
clean:
	rm -f $(bin)
