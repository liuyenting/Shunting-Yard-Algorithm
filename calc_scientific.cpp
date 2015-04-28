#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#ifndef NO_PROCESS
#include <sstream>
#endif
#include <cmath>
#include <stack>

/*
 * ==================================================
 * Numeric operations
 * ==================================================
 */
double eval_uplus(double a1, double a2)
{
	//return std::abs(a1);
	return a1;
}

double eval_uminus(double a1, double a2)
{
	return -a1;
}

/*
double eval_bnot(double a1, double a2)
{
	return ~a1;
}
*/

/*
double eval_lnot(double a1, double a2)
{
	return (a1 != 0) ? 0 : 1;
}
*/

double eval_mul(double a1, double a2)
{
	return a1 * a2;
}

double eval_div(double a1, double a2)
{
	if (!a2)
	{
		std::cerr << "ERROR: Division by zero." << std::endl;
		//std::exit(EXIT_FAILURE);
	}
	return a1 / a2;
}

/*
double eval_mod(double a1, double a2)
{
	if (!a2)
	{
		std::cerr << "ERROR: Division by zero." << std::endl;
		std::exit(EXIT_FAILURE);
	}
	return a1 % a2;
}
*/

double eval_add(double a1, double a2)
{
	return a1 + a2;
}

double eval_sub(double a1, double a2)
{
	return a1 - a2;
}

/*
double eval_lshift(double a1, double a2)
{
	return a1 << a2;
}
*/

/*
double eval_rshift(double a1, double a2)
{
	return a1 >> a2;
}
*/

/*
double eval_band(double a1, double a2)
{
	return a1 & a2;
}
*/

/*
double eval_bxor(double a1, double a2)
{
	return a1 ^ a2;
}
*/

/*
double eval_bor(double a1, double a2)
{
	return a1 | a2;
}
*/

/*
double eval_land(double a1, double a2)
{
	return (a1 && a2) ? 1 : 0;
}
*/

/*
double eval_lor(double a1, double a2)
{
	return (a1 || a2) ? 1 : 0;
}
*/

double eval_sin(double a1, double a2)
{
	return std::sin(a1);
}

double eval_cos(double a1, double a2)
{
	return std::cos(a1);
}

/*
double eval_exp(double a1, double a2)
{
	return a2 < 0 ? 0 : (a2 == 0 ? 1 : a1 * eval_exp(a1, a2 - 1));
}
*/

double eval_exp(double a1, double a2)
{
	return std::exp(a1);
}

double eval_log(double a1, double a2)
{

	if (a1 < 0)
	{
		std::cerr << "ERROR: Logarithm of a negative number is undefined." << std::endl;
		//std::exit(EXIT_FAILURE);
	}

	return std::log(a1);
}

double eval_pow(double a1, double a2)
{
	return std::pow(a1, a2);
}

double eval_sqrt(double a1, double a2)
{

	if (a1 < 0)
	{
		std::cerr << "ERROR: Root squaring a negative number." << std::endl;
		//std::exit(EXIT_FAILURE);
	}

	return std::sqrt(a1);
}

double eval_fabs(double a1, double a2)
{
	return std::fabs(a1);
}
/*
 * ==================================================
 */

enum associativity { ASSOC_NONE = 0, ASSOC_LEFT, ASSOC_RIGHT };

struct op_s
{
	char op;
	int prec;
	int assoc;
	int unary;
	double (*eval)(double a1, double a2);
} ops[] =
{
	{'(', 0,   ASSOC_NONE,     0, NULL},
	{')', 0,   ASSOC_NONE,     0, NULL},
	{'S', 20,   ASSOC_RIGHT,    1, eval_sin},
	{'C', 20,   ASSOC_RIGHT,    1, eval_cos},
	{'e', 20,   ASSOC_RIGHT,    1, eval_exp},
	{'l', 20,   ASSOC_RIGHT,    1, eval_log},
	{'p', 20,   ASSOC_RIGHT,    0, eval_pow},
	{'s', 20,   ASSOC_RIGHT,    1, eval_sqrt},
	{'f', 20,   ASSOC_RIGHT,    1, eval_fabs},
	{'#', 17,  ASSOC_RIGHT,    1, eval_uplus},
	{'_', 17,  ASSOC_RIGHT,    1, eval_uminus},
	//{'~', 17,  ASSOC_RIGHT,    1, eval_bnot},
	//{'!', 17,  ASSOC_RIGHT,    1, eval_lnot},
	{'*', 15,  ASSOC_LEFT,     0, eval_mul},
	{'/', 15,  ASSOC_LEFT,     0, eval_div},
	//{'%', 15,  ASSOC_LEFT,     0, eval_mod},
	{'+', 14,  ASSOC_LEFT,     0, eval_add},
	{'-', 14,  ASSOC_LEFT,     0, eval_sub},
	//{'<', 13,  ASSOC_LEFT,     0, eval_lshift},
	//{'>', 13,  ASSOC_LEFT,     0, eval_rshift},
	//{'&', 10,  ASSOC_LEFT,     0, eval_band},
	//{'^', 9,   ASSOC_LEFT,     0, eval_bxor},
	//{'|', 8,   ASSOC_LEFT,     0, eval_bor},
	//{'$', 7,   ASSOC_LEFT,     0, eval_land},
	//{'@', 6,   ASSOC_LEFT,     0, eval_lor}
};

struct op_s* getop(char ch)
{
	int i;
	for (i = 0; i < sizeof ops / sizeof ops[0]; ++i)
	{
		if (ops[i].op == ch)
		{
			return ops + i;
		}
	}
	return NULL;
}

std::stack<struct op_s*> opstack;
std::stack<double> numstack;

#ifndef NO_PROCESS
std::ostream& operator<<(std::ostream& stream, const struct op_s* s)
{
	return stream << s->op;
}

// Helper function to print the stack
template<typename T >
void print(const std::stack<T>& stack)
{
	struct cheat : std::stack<T>
	{
		using std::stack<T>::c;
	};

	const auto& elements = static_cast<const cheat&>(stack).c;
	for (const auto& element : elements)
	{
		std::cout << element << ' ' ;
	}
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out << std::fixed << std::setprecision(n) << a_value;
	return out.str();
}

std::string postfix;
#endif

void push_opstack(struct op_s* op)
{
	opstack.push(op);
}

struct op_s* pop_opstack()
{
	struct op_s* op = opstack.top();
	opstack.pop();

#ifdef NO_PROCESS
	switch (op->op)
	{
		case '(':
		case ')':
			break;
		case '#':
			std::cout << " +";
			break;
		case '_':
			std::cout << " -";
			break;
		case 'S':
			std::cout << " sin";
			break;
		case 'C':
			std::cout << " cos";
			break;
		case 'e':
			std::cout << " exp";
			break;
		case 'l':
			std::cout << " log";
			break;
		case 'p':
			std::cout << " pow";
			break;
		case 's':
			std::cout << " sqrt";
			break;
		case 'f':
			std::cout << " fabs";
			break;
		default:
			std::cout << ' ' << op->op;
	}
#else
	postfix += ' ';
	switch (op->op)
	{
		case '(':
		case ')':
			break;
		case '#':
			postfix += '+';
			break;
		case '_':
			postfix += '-';
			break;
		case 'S':
			postfix += "sin";
			break;
		case 'C':
			postfix += "cos";
			break;
		case 'e':
			postfix += "exp";
			break;
		case 'l':
			postfix += "log";
			break;
		case 'p':
			postfix += "pow";
			break;
		case 's':
			postfix += "sqrt";
			break;
		case 'f':
			postfix += "fabs";
			break;
		default:
			postfix += op->op;
	}
#endif

	return op;
}

void push_numstack(double num)
{
	numstack.push(num);
}

double pop_numstack()
{
	double num = numstack.top();
	numstack.pop();
	return num;
}

void shunt_op(struct op_s* op)
{
	struct op_s* pop;
	double n1, n2;

	if (op->op == '(')
	{
		push_opstack(op);
		return;
	}
	else if (op->op == ')')
	{
		while (opstack.size() > 0 && opstack.top()->op != '(')
		{
			pop = pop_opstack();
			n1 = pop_numstack();
			if (pop->unary)
			{
				push_numstack(pop->eval(n1, 0));
			}
			else
			{
				n2 = pop_numstack();
				push_numstack(pop->eval(n2, n1));
			}
		}
		if (!(pop = pop_opstack()) || pop->op != '(')
		{
			std::cerr << "ERROR: Stack error. No matching \'(\'" << std::endl;
			exit(EXIT_FAILURE);
		}
		return;
	}

	if (op->assoc == ASSOC_RIGHT)
	{
		while (opstack.size() > 0 && op->prec < opstack.top()->prec)
		{
			pop = pop_opstack();
			n1 = pop_numstack();
			if (pop->unary && pop->op != 'p')
			{
				push_numstack(pop->eval(n1, 0));
			}
			else
			{
				n2 = pop_numstack();
				push_numstack(pop->eval(n2, n1));
			}
		}
	}
	else
	{
		while (opstack.size() && op->prec <= opstack.top()->prec)
		{
			pop = pop_opstack();
			n1 = pop_numstack();
			if (pop->unary)
			{
				push_numstack(pop->eval(n1, 0));
			}
			else
			{
				n2 = pop_numstack();
				push_numstack(pop->eval(n2, n1));
			}
		}
	}
	push_opstack(op);

#ifndef NO_PROCESS
	std::cout << " OPSTACK | [ ";
	print<struct op_s*>(opstack);
	std::cout << "]" << std::endl;

	std::cout << "NUMSTACK | [ ";
	print<double>(numstack);
	std::cout << "]" << std::endl;

	std::cout << std::endl;
#endif
}

int main()
{
	char *expr;
	char *tstart = NULL;
	struct op_s startop = {'X', 0, ASSOC_NONE, 0, NULL};    /* Dummy operator to mark start */
	struct op_s *op = NULL;
	double n1, n2;
	struct op_s *lastop = &startop;

	std::string input;
	while (std::getline(std::cin, input))
	{
		expr = new char[input.length() + 1];
		std::strcpy(expr, input.c_str());

#ifndef NO_PROCESS
		postfix = "Postfix Exp:";
#else
		std::cout << "Postfix Exp:";
#endif
		for (int i = 0 ; *expr; ++expr, ++i)
		{
			//std::cout << "reading -> " << *expr << std::endl;

			switch (*expr)
			{
				case 's':
					if (i + 2 < input.size() && *(expr + 1) == 'i')
					{
						*expr = 'S';
						*(expr + 1) = *(expr + 2) = ' ';
					}
					else if (i + 3 < input.size() && *(expr + 1) == 'q')
					{
						*(expr + 1) = *(expr + 2) = *(expr + 3) = ' ';
					}
					break;
				case 'c':
					if (i + 2 < input.size() && *(expr + 1) == 'o')
					{
						*expr = 'C';
						*(expr + 1) = *(expr + 2) = ' ';
					}
					break;
				case 'e':
					if (i + 2 < input.size() && *(expr + 1) == 'x')
					{
						*(expr + 1) = *(expr + 2) = ' ';
					}
					break;
				case 'l':
					if (i + 2 < input.size() && *(expr + 1) == 'o')
					{
						*(expr + 1) = *(expr + 2) = ' ';
					}
					break;
				case 'p':
					if (i + 2 < input.size() && *(expr + 1) == 'o')
					{
						*expr = ' ';
						*(expr + 1) = *(expr + 2) = ' ';
					}
					break;
				case ',':
					*expr = 'p';
					break;
				case 'f':
					if (i + 3 < input.size() && *(expr + 1) == 'a')
					{
						*(expr + 1) = *(expr + 2) = *(expr + 3) = ' ';
					}
					break;
			}

			//std::cout << std::endl << "after filter: " << expr << std::endl;

			if (!tstart)
			{
				if ((op = getop(*expr)))
				{
					if (lastop && (lastop == &startop || lastop->op != ')'))
					{
						switch (op->op)
						{
							case '+':
								op = getop('#');
								break;
							case '-':
								op = getop('_');
								break;
							case '(':
								break;
							default:
								if (!op->unary)
								{
									std::cerr << "ERROR: Illegal use of binary operator (" << op->op << ")" << std::endl;
									exit(EXIT_FAILURE);
								}
						}
					}

					//std::cout << "filter complete" << std::endl;

					shunt_op(op);
					lastop = op;
				}
				else if (std::isdigit(*expr) || *expr == '.')
				{
					tstart = expr;
				}
			}
			else
			{
				if (isspace(*expr))
				{
					//std::cout << "<isspace, " << &tstart << ">" << std::endl;

					push_numstack(std::atof(tstart));

#ifndef NO_PROCESS
					postfix += ' ';
					postfix += to_string_with_precision(numstack.top());
#else
					std::cout << std::fixed << std::setprecision(6) << ' ' << numstack.top();
#endif

					tstart = NULL;
					lastop = NULL;
				}
				else if ((op = getop(*expr)))
				{
					//std::cout << "<getop, " << tstart << ", atof=" << std::atof(tstart) << ">" << std::endl;

					push_numstack(std::atof(tstart));

#ifndef NO_PROCESS
					postfix += ' ';
					postfix += to_string_with_precision(numstack.top());
#else
					std::cout << std::fixed << std::setprecision(6) << ' ' << numstack.top();
#endif

					tstart = NULL;
					shunt_op(op);
					lastop = op;
				}
				else if (!std::isdigit(*expr) && *expr != '.' && *expr != ',')
				{
					std::cerr << "ERROR: Syntax error" << std::endl;
					return EXIT_FAILURE;
				}
			}
		}
		if (tstart)
		{
			push_numstack(std::atof(tstart));

#ifndef NO_PROCESS
			postfix += ' ';
			postfix += to_string_with_precision(numstack.top());
#else
			std::cout << std::fixed << std::setprecision(6) << ' ' << numstack.top();
#endif
		}

		while (opstack.size() > 0)
		{
			op = pop_opstack();
			n1 = pop_numstack();
			if (op->unary)
			{
				push_numstack(op->eval(n1, 0));
			}
			else
			{
				n2 = pop_numstack();
				push_numstack(op->eval(n2, n1));
			}
		}
		if (numstack.size() != 1)
		{
			std::cerr << "ERROR: Number stack has " << numstack.size() << " elements after evaluation. Should be 1." << std::endl;

			std::cerr << "opstack: [ ";
			while (!opstack.empty())
			{
				std::cerr << pop_opstack() << ' ';
			}
			std::cerr << ']' << std::endl;

			std::cerr << "numstack: [ ";
			while (!numstack.empty())
			{
				std::cerr << std::fixed << std::setprecision(6) << pop_numstack() << ' ';
			}
			std::cerr << ']' << std::endl;

			return EXIT_FAILURE;
		}

		std::cout << std::endl;
#ifndef NO_PROCESS
		std::cout << postfix << std::endl;
#endif
		std::cout << "RESULT: " << std::fixed << std::setprecision(6) << pop_numstack() << std::endl;

		tstart = NULL;
	}
	return EXIT_SUCCESS;
}

