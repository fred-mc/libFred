#==================================================#
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
# do something Linux-y
SYSLIB=-ldl -lrt -lpthread
CCC=g++ -std=c++11 
CC=gcc
CCVERSION = $(shell gcc --version | grep ^gcc | awk '{print $$3}')
PLATFORM=linux
LIBDIR=../../lib/linux
endif
ifeq ($(UNAME), Darwin)
# do something Darwin-y
SYSLIB=-ldl -lpthread
CCC=clang++ -std=c++11 
CC=clang
CCVERSION = $(shell clang --version | grep ^Target | awk '{print $$2}' | sed 's/^.*darwin//')
PLATFORM=mac
endif
#==================================================#
LIBDIR=../../lib/$(PLATFORM)
#==================================================#
default:
	ln -sf $(LIBDIR)/libFred.so 
	ln -sf $(LIBDIR)/libFred.h
	ln -sf $(LIBDIR)/libFred.data
	clang++ main.cpp -L. -lFred -o test.x

