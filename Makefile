EXEC      = net
CC_FLAGS  = -Wall -g -lcurl -ljpeg
CC        = g++
LINKER    = g++

SOURCES   = $(wildcard *.cpp)
OBJECTS   = $(SOURCES:.cpp=.o)

$(EXEC) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

%.o : %.cpp
	$(CC) $(CC_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
