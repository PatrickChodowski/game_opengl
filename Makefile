
# http://nuclear.mutantstargoat.com/articles/make/#a-makefile-for-99-of-your-programs
# Check on which platform are we -> Darwin is mac 
UNAME := $(shell uname)
CXX=g++
CXXFLAGS= -MMD -std=c++17 -I dependencies/python/include

# LD Flags are added only to the linker
ifeq ($(UNAME),Darwin)
LDFLAGS = -lSDL2 -lGLEW -lfreetype -framework OpenGL -lpython3.8
endif 
ifeq ($(UNAME),Linux)
LDFLAGS = -lSDL2 -lGL -lGLEW -lfreetype -lpython3.8
endif

src = $(wildcard *.cpp)\
			$(wildcard modules/*.cpp) \
			$(wildcard scripts/*.cpp)
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

# not using right now but keeping as example
scripts: scripts/scripts.cpp
	g++ -std=c++17 scripts/scripts.cpp -fPIC -shared -o scripts/scripts.so -I dependencies/python/include
