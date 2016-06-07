#0MQ C++ BINDING:
ZEROMQ_BINDING_PATH	= ./

CXXFLAGS =	-O2 -g -Wall -std=c++0x -fmessage-length=0 -I$(ZEROMQ_BINDING_PATH) 

OBJS =	main.o

LIBS =	-l zmq 

TARGET =	persistent_buffer

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
