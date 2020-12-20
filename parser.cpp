#include <iostream>
#include <strstream>
#include <string>
#include "antlr4-runtime.h"
#include "ExprLexer.h"
#include "ExprParser.h"
#include "ExprBaseListener.h"

class MyParserErrorListener: public antlr4::BaseErrorListener {
  virtual void syntaxError(
      antlr4::Recognizer *recognizer,
      antlr4::Token *offendingSymbol,
      size_t line,
      size_t charPositionInLine,
      const std::string &msg,
      std::exception_ptr e) override {
    std::ostrstream s;
    s << "Line(" << line << ":" << charPositionInLine << ") Error(" << msg << ")";
    throw std::invalid_argument(s.str());
  }
};

// https://github.com/antlr/antlr4/blob/master/doc/cpp-target.md
class MyExprListener : public ExprBaseListener {
protected:
  unsigned int count;
  unsigned int depth;

  std::ostream &stream() {
    for(unsigned i = 0; i < depth; ++i) {
      std::cout << '.';
    }
    std::cout << ' ';
    return std::cout;
  }
public:
  MyExprListener() : count(0), depth(0) {}
  virtual void enterExpr(ExprParser::ExprContext *ctx) {
    ++count;
    ++depth;
    stream() << ctx << std::endl;
    stream() << '\t' << ctx->INT() << std::endl;
    stream() << '\t' << ctx->expr().size() << std::endl;
    if(ctx->INT() != NULL) {
      // This is a Token.
      stream() << '\t' << ctx->INT()->getText() << std::endl;
    }
  }
  virtual void exitExpr(ExprParser::ExprContext * /*ctx*/) override {
    --depth;
  }
  unsigned int get_count(void) const { return count; }
};

int main(int argc, char *argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: parser '<EXPRESSION>'\n";
    return 1;
  }
  antlr4::ANTLRInputStream input(argv[1]);
  ExprLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);

  MyParserErrorListener errorListner;

  tokens.fill();
  // Only if you want to list the tokens
  for (auto token : tokens.getTokens()) {
    std::cout << '"' << token->toString() << '"' << std::endl;
  }
  
  ExprParser parser(&tokens);
  parser.removeErrorListeners();
  parser.addErrorListener(&errorListner);
  try {
    antlr4::tree::ParseTree* tree = parser.main();
    std::cout << tree->toStringTree() << std::endl;
    MyExprListener listener;
    antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
    std::cout << "\tcount in listener = " << listener.get_count() << std::endl;
    return 0;
  } catch (std::invalid_argument &e) {
    std::cout << e.what() << std::endl;
    return 10;
  }
}
