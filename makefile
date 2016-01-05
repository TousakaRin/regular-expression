objects = main.o ast.o astnode.o matchObj.o rgx.o  
flag = -std=c++11 -Wall -g

test : main.o astnode.o ast.o
	g++-5 -o test $(flag) main.o astnode.o ast.o

#test : $(objects)
#	g++-5 -o test $(flag) $(objects) 
#
#main.o : ast.h astnode.h \
#	main.cpp
#	g++-5 -c $(flag) main.cpp
#
ast.o : ast.h \
	ast.cpp
	g++-5 -c $(flag) ast.cpp
astnode.o : astnode.h \
	astnode.cpp
	g++-5 -c $(flag) astnode.cpp

#matchObj.o : matchObj.h \
#	matchObj.cpp
#	g++-5 -c $(flag) matchObj.cpp
#
#pattern.o : pattern.h  \
#	pattern.cpp
#	g++-5 -c $(flag) pattern.cpp
#
rgx.o : rgx.h\
	rgx.cpp
	g++-5 -c $(flag) rgx.cpp

