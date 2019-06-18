#include <iostream>
#include <sstream>
#include <vector>
#include <map>


/*enum TokenType is necessary to determine type of our tokens*/
 enum TokenType { ADD, MULT, SUB, CONCAT, INT, STRING, IDENT, PAR_LEFT, PAR_RIGHT, MINUS, TERMINATOR };
/*We get tokens from deterministic finite automaton, so we need Input states and Result states*/
 enum Statement{ S0, S1, S2, S3, S4, S5, S6, S7, S11 };
 enum Output { O0, O1, O2, O3, O4, O5, O6, O11 };

using Result = std::pair<Statement, Output>; 
using Input = std::pair<Statement, char>;



using Item = std::pair<Input, Result>;


void InitMap();
/*Token class*/
class Token {
public:
	Token(TokenType token_type, std::string token_value) : type(token_type), value(token_value) {};
	~Token() {};
	auto GetIntValue();
	auto GetStringValue(){return value;}
	auto GetType() {return type;}
private:
	TokenType type;
	std::string value;
};

/*our main class*/
class Interpreter {
public:
	Interpreter() {};
	~Interpreter() {};
	void LexAnalysis(std::string);
	void Parse();
	auto GetToken();
	int Add();
	int Mult();
	int Sub();
	std::string Concat();
private:
	std::vector<Token> lexems; /*this is the storage for tokens from lexical analysis*/
	std::string::iterator position; 
};


