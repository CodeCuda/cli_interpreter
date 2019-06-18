#include"Interpreter.h"
#include <algorithm>

/*We need map for modeling our finite automaton*/
std::map<Input, Result> StatementsMap;




auto Token::GetIntValue()
{
	if (type == INT)
		return std::stoi(value);
	if (type == STRING) {
		return std::stoi(value);
	}
}


void Interpreter::LexAnalysis(std::string input_string) {
	std::map<Input, Result>::const_iterator it;
	Statement current = S0;
	TokenType type;
	Result res;
	std::string value;
	InitMap();
	input_string += "$";

	for (auto string_iter = input_string.cbegin(); string_iter != input_string.cend(); ++string_iter) {
		
		
		auto it = StatementsMap.find(Input(current, *string_iter));
		auto next_iter = std::next(string_iter, 1);
		if (it == StatementsMap.end()) {
			std::cout << "LEXICAL ERROR" << std::endl;
			system("pause");
			exit(1);
		}
		
		res = it->second; 
		if (res.second == O11) std::cerr << "LEXICAL ERROR"; 
		if (res.first == S11 || next_iter == input_string.cend())
		{
			switch (res.second) //Âûõîäíîé ñèãíàë
			{
			case O1:
				if (value == "add")
				{
					type = ADD;
					break;
				}
				if (value == "mult") 
				{
					type = MULT;
					break;
				}
				if (value == "sub")
				{
					type = SUB;
					break;
				}
				if (value == "concat")
				{
					type = CONCAT;
					break;
				}
				type = IDENT;
				break;
			case O2:
				type = INT;
				break;
			case O3:
				type = PAR_LEFT;
				break;
			case O4:
				type = PAR_RIGHT;
				break;
			case O5:
				type = MINUS;
				break;
			case O6:
				type = STRING;
				
				break;
			default:
				std::cout << "LEXICAL ERROR" << std::endl;
				system("pause");
				exit(1);
				break;


			}
			// *string_iter;
			
			
			if (type == STRING) {

				value.erase(std::remove(value.begin(), value.end(), '\\'), value.end());
				value.erase(std::remove(value.begin(), value.end(), '"'), value.end());

			}
			else
				value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
			
			Token token(type, value);
			lexems.push_back(token);
			res.first = S0;
			res.second = O1;
			value = "";
			current = res.first;
			
			if (*string_iter == '$') {
				continue;
			}
			--string_iter;
			continue;
		}
		current = res.first;
		value += *string_iter;
		
	}
}
	   

auto Interpreter::GetToken() {
	if (!lexems.empty()) {
		auto token = lexems.front();
		lexems.erase(lexems.begin());
		return token;
	}
	Token token(IDENT, "empty");
	return token;
};



int Interpreter::Add() {

	auto token = GetToken();
	if (token.GetType() == PAR_RIGHT)
		return 0;
	if (token.GetStringValue() == "empty")
		return 0;
	if (token.GetType() == PAR_LEFT) {
		token = GetToken();
		if (token.GetType() == ADD)
			return Add() + Add();
		if (token.GetType() == MULT)
			return Mult() + Add();
		if (token.GetType() == SUB) {
			return Sub() + Add();
		}
	}
	if (token.GetType() == INT) {
		return token.GetIntValue() + Add();
	}

	if (token.GetType() == STRING) {
		return token.GetIntValue() + Add();
	}

}
int Interpreter::Sub() {

	auto token = GetToken();
	if (token.GetType() == PAR_RIGHT)
		return 0;
	if (token.GetStringValue() == "empty")
		return 0;
	if (token.GetType() == PAR_LEFT) {
		token = GetToken();
		if (token.GetType() == ADD)
			return Add() - Sub();
		if (token.GetType() == MULT)
			return Mult() - Sub();
		if (token.GetType() == SUB) {
			return Sub() - Sub();
		}
	}
	if (token.GetType() == INT) {
		return token.GetIntValue() - Sub();
	}

	if (token.GetType() == STRING) {
		return token.GetIntValue() - Sub();
	}

}
int Interpreter::Mult()
{
	auto token = GetToken();
	if (token.GetType() == PAR_RIGHT)
		return 1;
	if (token.GetStringValue() == "empty")
		return 1;
	if (token.GetType() == PAR_LEFT) {
		token = GetToken();
		if (token.GetType() == ADD) {
			return Add() * Mult();
		}
		if (token.GetType() == MULT) {
			return Mult() * Mult();
		}
		if (token.GetType() == SUB) {
			return Sub() * Mult();
		}
	}
	if (token.GetType() == INT) {
		return token.GetIntValue() * Mult();
	}

	if (token.GetType() == STRING) {
		return token.GetIntValue() * Mult();
	}
}

std::string Interpreter::Concat()
{
	auto token = GetToken();
	if (token.GetType() == PAR_RIGHT)
		return "";
	if (token.GetStringValue() == "empty")
		return "";
	if (token.GetType() == PAR_LEFT) {
		token = GetToken();
		if (token.GetType() == ADD) {
			auto temp = std::to_string(Add());
			return temp + Concat();
		}
		if (token.GetType() == MULT) {
			auto temp = std::to_string(Mult());
			return temp + Concat();
		}
		if (token.GetType() == SUB) {
			auto temp = std::to_string(Sub());
			return temp + Concat();
		}
		if (token.GetType() == CONCAT) {
			return Concat() + Concat();
		}
	}
	if (token.GetType() == INT) {
		return token.GetStringValue() + Concat();
	}

	if (token.GetType() == STRING) {
		return token.GetStringValue() + Concat();
	}
}

/*To parse our input string we use recursive parsing. It guarantees us operation precedence
Every function (Add(), Sub() and etc.) is nonterminal symbol in used grammar*/
void Interpreter::Parse() {
	
	auto token = GetToken();
	if (token.GetType() == ADD)
		std::cout << Add() << std::endl;
	if (token.GetType() == MULT)
		std::cout << Mult() << std::endl;
	if (token.GetType() == SUB)
		std::cout << Sub() << std::endl;
	if (token.GetType() == CONCAT)
		std::cout << Concat() << std::endl;

}




void InitMap() {

	// Çàïîëíåíèå òàáëèöû 
	//0 
	for (auto i = 'A'; i != 'Z' + 1; ++i)
		StatementsMap.insert(Item(Input(S0, i), Result(S1, O1)));
	for (auto i = 'a'; i != 'z' + 1; ++i)
		StatementsMap.insert(Item(Input(S0, i), Result(S1, O1)));
	for (auto i = '0'; i != '9' + 1; ++i)
		StatementsMap.insert(Item(Input(S0, i), Result(S2, O2)));
	StatementsMap.insert(Item(Input(S0, '+'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, '-'), Result(S5, O5)));
	StatementsMap.insert(Item(Input(S0, '*'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, '/'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, '('), Result(S3, O3)));
	StatementsMap.insert(Item(Input(S0, ')'), Result(S4, O4)));
	StatementsMap.insert(Item(Input(S0, ' '), Result(S0, O0)));
	StatementsMap.insert(Item(Input(S0, '['), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, ']'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, '\n'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, '='), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, '%'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, '&'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, ';'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, '{'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, '}'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, '>'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, '<'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, '$'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S0, '"'), Result(S6, O6)));
	
	for (auto i = 'A'; i != 'Z' + 1; ++i)
		StatementsMap.insert(Item(Input(S1, i), Result(S1, O1)));
	for (auto i = 'a'; i != 'z' + 1; ++i)
		StatementsMap.insert(Item(Input(S1, i), Result(S1, O1)));
	for (auto i = '0'; i != '9' + 1; ++i)
		StatementsMap.insert(Item(Input(S1, i), Result(S1, O1)));
	StatementsMap.insert(Item(Input(S1, '+'), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, '-'), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, '*'), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, '/'), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, '('), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, ')'), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, ' '), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, '['), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, ']'), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, '\n'), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, '='), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, '%'), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, '&'), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, ';'), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, '{'), Result(S11, O1)));
	StatementsMap.insert(Item(Input(S1, '}'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S1, '>'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S1, '<'), Result(S11, O11)));
	StatementsMap.insert(Item(Input(S1, '$'), Result(S11, O1)));

	for (auto i = 'A'; i != 'Z' + 1; ++i)
		StatementsMap.insert(Item(Input(S2, i), Result(S11, O11)));
	for (auto i = 'a'; i != 'z' + 1; ++i)
		StatementsMap.insert(Item(Input(S2, i), Result(S11, O11)));
	for (auto i = '0'; i != '9' + 1; ++i)
		StatementsMap.insert(Item(Input(S2, i), Result(S2, O2)));
	StatementsMap.insert(Item(Input(S2, '+'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '-'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '*'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '/'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '('), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, ')'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, ' '), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '['), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, ']'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '\n'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '='), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '%'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '&'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, ';'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '{'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '}'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '>'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '<'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S2, '$'), Result(S11, O2)));

	for (auto i = 'A'; i != 'Z' + 1; ++i)
		StatementsMap.insert(Item(Input(S3, i), Result(S11, O3)));
	for (auto i = 'a'; i != 'z' + 1; ++i)
		StatementsMap.insert(Item(Input(S3, i), Result(S11, O3)));
	for (auto i = '0'; i != '9' + 1; ++i)
		StatementsMap.insert(Item(Input(S3, i), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '+'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '-'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '*'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '/'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '('), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, ')'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, ' '), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '['), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, ']'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '\n'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '='), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '%'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '&'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, ';'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '{'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '}'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '>'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '<'), Result(S11, O3)));
	StatementsMap.insert(Item(Input(S3, '$'), Result(S11, O3)));

	for (auto i = 'A'; i != 'Z' + 1; ++i)
		StatementsMap.insert(Item(Input(S4, i), Result(S11, O4)));
	for (auto i = 'a'; i != 'z' + 1; ++i)
		StatementsMap.insert(Item(Input(S4, i), Result(S11, O4)));
	for (auto i = '0'; i != '9' + 1; ++i)
		StatementsMap.insert(Item(Input(S4, i), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '+'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '-'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '*'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '/'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '('), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, ')'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, ' '), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '['), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, ']'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '\n'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '='), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '%'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '&'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, ';'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '{'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '}'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '>'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '<'), Result(S11, O4)));
	StatementsMap.insert(Item(Input(S4, '$'), Result(S11, O4)));

	for (auto i = 'A'; i != 'Z' + 1; ++i)
		StatementsMap.insert(Item(Input(S5, i), Result(S11, O2)));
	for (auto i = 'a'; i != 'z' + 1; ++i)
		StatementsMap.insert(Item(Input(S5, i), Result(S11, O2)));
	for (auto i = '0'; i != '9' + 1; ++i)
		StatementsMap.insert(Item(Input(S5, i), Result(S5, O5)));
	StatementsMap.insert(Item(Input(S5, '+'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '-'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '*'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '/'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '('), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, ')'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, ' '), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '['), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, ']'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '\n'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '='), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '%'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '&'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, ';'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '{'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '}'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '>'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '<'), Result(S11, O2)));
	StatementsMap.insert(Item(Input(S5, '$'), Result(S11, O2)));

	for (auto i = 'A'; i != 'Z' + 1; ++i)
		StatementsMap.insert(Item(Input(S6, i), Result(S6, O6)));
	for (auto i = 'a'; i != 'z' + 1; ++i)
		StatementsMap.insert(Item(Input(S6, i), Result(S6, O6)));
	for (auto i = '0'; i != '9' + 1; ++i)
		StatementsMap.insert(Item(Input(S6, i), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '+'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '-'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '*'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '/'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '('), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, ')'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, ' '), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '['), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, ']'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '\n'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '='), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '%'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '&'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, ';'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '{'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '}'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '>'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '<'), Result(S6, O6)));
	StatementsMap.insert(Item(Input(S6, '$'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S6, '"'), Result(S7, O6)));


	for (auto i = 'A'; i != 'Z' + 1; ++i)
		StatementsMap.insert(Item(Input(S7, i), Result(S11, O6)));
	for (auto i = 'a'; i != 'z' + 1; ++i)
		StatementsMap.insert(Item(Input(S7, i), Result(S11, O6)));
	for (auto i = '0'; i != '9' + 1; ++i)
		StatementsMap.insert(Item(Input(S7, i), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '+'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '-'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '*'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '/'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '('), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, ')'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, ' '), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '['), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, ']'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '\n'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '='), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '%'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '&'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, ';'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '{'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '}'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '>'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '<'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '$'), Result(S11, O6)));
	StatementsMap.insert(Item(Input(S7, '"'), Result(S11, O6)));
}



