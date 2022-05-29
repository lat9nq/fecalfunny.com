CXX=g++
CXXFLAGS=-g -Og -fPIC $(shell pkg-config --cflags vulkan)
LDFLAGS=-shared $(shell pkg-config --libs vulkan)

%o:%.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

fecalfunny.so:fecalfunny.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

fecalfunny.o: fecalfunny.cpp fecalfunny.h
