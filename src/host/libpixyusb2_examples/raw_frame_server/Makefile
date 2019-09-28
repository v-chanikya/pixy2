CXX=g++
CPPFLAGS=-g -fpermissive -I/usr/include/libusb-1.0 -I../../libpixyusb2/include -I../../arduino/libraries/Pixy2
LDLIBS=../../../../build/libpixyusb2/libpixy2.a -lusb-1.0

SRCS=raw_frame_server.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: get_blocks

clean:
	rm -f *.o raw_frame_server 

get_blocks: $(OBJS)
	$(CXX) $(LDFLAGS) -o raw_frame_server $(OBJS) $(LDLIBS)
