
ANTLR=antlr4
GRAMMAR=Expr
CXXFLAGS=-Wall -Wextra -DNDEBUG
LDFLAGS=-g
LIBS=-lantlr4-runtime
ANTLRGEN=BaseListener Lexer Listener Parser 
OBJS=$(addsuffix .o,$(addprefix $(GRAMMAR),$(ANTLRGEN)))

all: ExprParser.h parser

%.o:	%.cpp
	$(CXX) -c -DNDEBUG -I /usr/include/antlr4-runtime -o $@ $+

clean:
	rm -f *.o
	rm -f $(GRAMMAR)*.h
	rm -f $(GRAMMAR)*.cpp
	rm -f $(GRAMMAR)*.interp
	rm -f $(GRAMMAR)*.tokens
	rm -f parser

ExprParser.h:	Expr.g4
	$(ANTLR) -Dlanguage=Cpp $+

parser: parser.o $(OBJS)
	$(CXX) $(LDFLAGS) -o parser $+ $(LIBS)


