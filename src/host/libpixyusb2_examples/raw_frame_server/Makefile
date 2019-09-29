CXX=g++
CPPFLAGS=-g -fpermissive -I/usr/include/libusb-1.0 -I../../libpixyusb2/include -I../../arduino/libraries/Pixy2
LDLIBS=../../../../build/libpixyusb2/libpixy2.a -lusb-1.0

SRCS_SERVER=raw_frame_server.cpp
OBJS_SERVER=$(subst .cpp,.o,$(SRCS_SERVER))

SRCS_DUMP=raw_frame_dump.cpp
OBJS_DUMP=$(subst .cpp,.o,$(SRCS_DUMP))

all: server dump

clean:
	rm -f *.o raw_frame_server 

server: $(OBJS_SERVER)
	$(CXX) $(LDFLAGS) -o raw_frame_server $(OBJS_SERVER) $(LDLIBS)

dump: $(OBJS_DUMP)
	$(CXX) $(LDFLAGS) -o raw_frame_dump $(OBJS_DUMP) $(LDLIBS)
