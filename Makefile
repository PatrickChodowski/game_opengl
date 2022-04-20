
# http://nuclear.mutantstargoat.com/articles/make/#a-makefile-for-99-of-your-programs
# Check on which platform are we -> Darwin is mac 
UNAME := $(shell uname)
CXX=g++

# LD Flags are added only to the linker
ifeq ($(UNAME),Darwin)
CXXFLAGS= -MMD -std=c++17 -I/usr/local/Cellar/python@3.8/3.8.13/Frameworks/Python.framework/Versions/3.8/Headers
LDFLAGS = -lSDL2 -lGLEW -lfreetype -L/usr/local/Cellar/python@3.8/3.8.13/Frameworks/Python.framework/Versions/3.8/lib -lpython3.8 -framework OpenGL
endif 
ifeq ($(UNAME),Linux)
CXXFLAGS= -MMD -std=c++17 -I dependencies/python/include
LDFLAGS = -lSDL2 -lGL -lGLEW -lfreetype -lopenal -lpython3.8
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


# Make for tests

src_tests = $(wildcard tests/*.cpp)\
			$(wildcard modules/*.cpp) \
			$(wildcard scripts/*.cpp)
obj_tests = $(src_tests:.cpp=.o )
dep_tests = $(obj:.o=.d)  # one dependency file for each source

test: $(obj_tests)
	$(CXX) -o $@ $^ $(LDFLAGS)

-include $(dep_tests)   # include all dep files in the makefile
