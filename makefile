CC= g++
cflags= -Wall -g
clibs= -lm -lpthread
targets= udp_recv
objs= udp_socket.o main.o

all: ${targets}

udp_recv: $(objs)
	$(CC) $(cflags) $(objs) -o udp_recv $(clibs)

%.o: %.cpp
	$(CC) -std=c++11 -g -c $< -o $@ $(clibs)

clean:
	rm -f $(objs) $(targets) *~
