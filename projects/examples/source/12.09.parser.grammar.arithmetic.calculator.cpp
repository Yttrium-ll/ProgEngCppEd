#include <cctype>
#include <exception>
#include <iostream>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>

//  ================================================================================================

using Token = std::variant < char, double, std::string > ;

//  ================================================================================================

class Stream
{
public:

	explicit Stream(const std::string & data) : m_sin(data + ';') {}

//  ----------------------------------------------------------------

	auto empty()
	{
		char c; m_sin >> c;

		if (c != ';') 
		{ 
			m_sin.putback(c); return false; 
		} 
		else 
		{
			return true;
		}
	}

	auto get()
	{
		if (m_is_full) 
		{ 
			m_is_full = false; return m_token; 
		}

		char c; m_sin >> c;
		
		switch (c)
		{
			case '+': case '-': case '*': case '/': 
			case '(': case ')': case ';':
			{
				return Token(c);
			}
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
			case '.':
			{
				m_sin.putback(c); double value; m_sin >> value;

				return Token(value);
			}
			default:
			{
				if (std::string string(1, c); std::isalpha(c))
				{
					while (m_sin.get(c) && (std::isalpha(c) || std::isdigit(c))) 
					{
						string += c;
					}

					if (!std::isspace(c)) 
					{
						m_sin.putback(c); 
					}

					return Token(string);
				}
				else 
				{
					throw std::runtime_error("invalid token");
				}
			}
		}
	}

	void putback(const Token & token)
	{
		m_token = token; m_is_full = true;
	}

private:

	std::stringstream m_sin; bool m_is_full = false; Token m_token;
};

//  ================================================================================================

class Calculator
{
public:

	void test()
	{
		std::cout << "Enter 1 or more expressions : \n";

		for (std::string line; std::getline(std::cin >> std::ws, line); )
		{
			if (Stream stream(line); !stream.empty())
			{
				std::cout << "result = " << statement(stream) << '\n';
			}
			else 
			{
				break;	
			}		
		}
	}

private:

	auto statement(Stream & stream) -> double
	{
		auto token = stream.get();

		if (std::holds_alternative < std::string > (token) && 
			std::get               < std::string > (token) == "set")
		{
			return declaration(stream);
		}
		
		stream.putback(token); 
		
		return expression(stream);
	}

	auto declaration(Stream & stream) -> double
	{
		auto name = std::get < std::string > (stream.get());

		m_variables[name] = expression(stream);

		return m_variables[name];
	}

	auto expression(Stream & stream) const -> double
	{
		auto left = term(stream);

		for (auto token = stream.get(); true; token = stream.get())
		{
			if (std::holds_alternative < char > (token))
			{
				switch (std::get < char > (token))
				{
					case '+': { left += term(stream); break; }
					case '-': { left -= term(stream); break; }

					default: 
					{ 
						stream.putback(token); 
						
						return left; 
					}
				}
			}
			else 
			{
				throw std::runtime_error("invalid expression");
			}
		}
	}

	auto term(Stream & stream) const -> double
	{
		auto left = primary(stream);

		for (auto token = stream.get(); true; token = stream.get())
		{
			if (std::holds_alternative < char > (token))
			{
				switch (std::get < char > (token))
				{
					case '*': { left *= term(stream); break; }
					case '/': { left /= term(stream); break; }

					default: 
					{ 
						stream.putback(token); 
						
						return left; 
					}
				}
			}
			else 
			{
				throw std::runtime_error("invalid term");
			}
		}
	}

	auto primary(Stream & stream) const -> double
	{
		auto token = stream.get();

		if (std::holds_alternative < char > (token))
		{
			switch (std::get < char > (token))
			{
				case '(':
				{
					auto result = expression(stream); 
					
					token = stream.get(); 

					if (!std::holds_alternative < char > (token) || 
						 std::get               < char > (token) != ')')
					{
						throw std::runtime_error("invalid token");
					}
					
					return result;
				}
				case '+': { return        primary(stream); }
				case '-': { return -1.0 * primary(stream); }
				
				default: 
				{
					throw std::runtime_error("invalid primary");
				}
			}
		}
		
		if (std::holds_alternative < double > (token))
		{
			return std::get < double > (token);
		}

		return m_variables.at(std::get < std::string > (token));
	}

//  -------------------------------------------------------

	std::unordered_map < std::string, double > m_variables;
};

//  ================================================================================================

int main()
{
	Calculator().test();
}