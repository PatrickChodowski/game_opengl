
# http://nuclear.mutantstargoat.com/articles/make/#a-makefile-for-99-of-your-programs
# Check on which platform are we -> Darwin is mac 
UNAME := $(shell uname)
CXX=g++
CXXFLAGS= -MMD -std=c++17

ifeq ($(UNAME),Darwin)
LDFLAGS = -lSDL2 -lGLEW -lfreetype -framework OpenGL -I /usr/include/python3.8/
endif
ifeq ($(UNAME),Linux)
LDFLAGS = -lSDL2 -lGL -lGLEW -lfreetype -I /usr/include/python3.8/
endif

src = $(wildcard *.cpp)\
			$(wildcard modules/*.cpp) 
obj = $(src:.cpp=.o )
dep = $(obj:.o=.d)  # one dependency file for each source

game: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

-include $(dep)   # include all dep files in the makefile

.PHONY: clean
clean:
	rm -f $(obj) game

.PHONY: cleandep
cleandep:
	rm -f $(dep)


scripts: scripts/scripts.cpp
	g++ -std=c++17 scripts/scripts.cpp -fPIC -shared -o scripts/scripts.so -I dependencies/python/include