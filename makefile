objects = main.o ast.o astnode.o matchObj.o rgx.o stringTools.o pattern.o edgeManager.o nfaEdge.o nfaNode.o
flag = -std=c++11 -Wall -g

test : $(objects)
	g++-5 -o test $(flag) $(objects) 

main.o : ast.h astnode.h stringTools.h testCase.h\
	main.cpp
	g++-5 -c $(flag) main.cpp

ast.o : ast.h \
	ast.cpp
	g++-5 -c $(flag) ast.cpp
astnode.o : astnode.h stringTools.h\
	astnode.cpp
	g++-5 -c $(flag) astnode.cpp

stringTools.o : stringTools.h \
	stringTools.cpp
	g++-5 -c $(flag) stringTools.cpp

matchObj.o : matchObj.h \
	matchObj.cpp
	g++-5 -c $(flag) matchObj.cpp

pattern.o : ast.h pattern.h  nfaNode.h \
	pattern.cpp
	g++-5 -c $(flag) pattern.cpp

rgx.o : rgx.h\
	rgx.cpp
	g++-5 -c $(flag) rgx.cpp

edgeManager.o : edgeManager.h \
	edgeManager.cpp
	g++-5 -c  $(flag) edgeManager.cpp

nfaEdge.o : nfaEdge.h \
	nfaEdge.cpp
	g++-5 -c $(flag) nfaEdge.cpp

nfaNode.o : nfaEdge.h nfaNode.h \
	nfaNode.cpp
	g++-5 -c $(flag) nfaNode.cpp

dfaTable.o : dfaTable.h \
	dfaTable.cpp
	g++-5 -c $(flag) dfaTable.cpp

clean :
	rm test *.o
	rm -rf test.*
