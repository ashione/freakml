CFLAGS = -g -Wall -fPIC -std=c++11
CXX = g++
CC = gcc

INCLUDE=-I./include -I/usr/include/eigen3
SRC = $(wildcard src/*.cpp)

TARGET = ./lib/libfreak.so
LDFLAGS = -shared
LIBS = -L./lib -lstdc++ -lfreak

BINDIR  = bin
OBJDIR = bin/obj
TESTDIR = test

TESTSRC_CPP = $(wildcard $(TESTDIR)/*.cpp)
TESTSRC_C = $(wildcard $(TESTDIR)/*.c)
TESTSRC = $(TESTSRC_CPP) $(TESTSRC_C)

$(info $(TESTSRC))

TESTOBJ_CPP = $(patsubst $(TESTDIR)/%.cpp,$(OBJDIR)/%.o,$(filter %.cpp,$(TESTSRC)))
TESTOBJ_C = $(patsubst $(TESTDIR)/%.c,$(OBJDIR)/%.o,$(filter %.c,$(TESTSRC)))

TESTOBJ = $(TESTOBJ_CPP) $(TESTOBJ_C)

$(info $(TESTOBJ))

TESTBIN_CPP = $(patsubst $(TESTDIR)/%.cpp,$(BINDIR)/%.bin,$(filter %.cpp,$(TESTSRC)))
TESTBIN_C   = $(patsubst $(TESTDIR)/%.c,$(BINDIR)/%.bin,$(filter %.c,$(TESTSRC)))

TESTEXE = $(TESTBIN_CPP) $(TESTBIN_C)


all : $(TARGET) 
	
$(TARGET) : pre
	$(CXX) $(CFLAGS) -o $(TARGET) $(SRC)  $(LDFLAGS) $(INCLUDE)
	@echo "generate share library"

pre :
	test -d lib ||  mkdir lib 
	test -d bin || ( mkdir -p bin/obj)

test : $(TESTEXE)

$(BINDIR)/%.bin : $(OBJDIR)/%.o
	$(CXX) $(CFLAGS) -o $@ $^  $(INCLUDE) $(LIBS)

$(OBJDIR)/%.o : $(TESTDIR)/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $^ $(INCLUDE) $(LIBS)

$(OBJDIR)/%.o : $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $^ $(INCLUDE) $(LIBS)


.PHONY : clean

clean :  cleantest
	rm -rf lib  $(TARGET) 

cleantest : 
	rm -rf bin
