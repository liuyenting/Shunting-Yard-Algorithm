#include <iostream>
#include <cstdlib>

#include <stack>

/*
 * ==================================================
 * Numeric operations
 * ==================================================
 */
int eval_uplus(int a1, int a2)
{
	return std::abs(a1);
}

int eval_uminus(int a1, int a2)
{
	return -a1;
}

int eval_bnot(int a1, int a2)
{
	return ~a1;
}

int eval_lnot(int a1, int a2)
{
	return (a1 != 0) ? 0 : 1;
}

int eval_exp(int a1, int a2)
{
	return a2 < 0 ? 0 : (a2 == 0 ? 1 : a1 * eval_exp(a1, a2 - 1));
}

int eval_mul(int a1, int a2)
{
	return a1 * a2;
}

int eval_div(int a1, int a2)
{
	if (!a2)
	{
		std::cerr << "ERROR: Division by zero." << std::endl;
		std::exit(EXIT_FAILURE);
	}
	return a1 / a2;
}

int eval_mod(int a1, int a2)
{
	if (!a2)
	{
		std::cerr << "ERROR: Division by zero." << std::endl;
		std::exit(EXIT_FAILURE);
	}
	return a1 % a2;
}

int eval_add(int a1, int a2)
{
	return a1 + a2;
}

int eval_sub(int a1, int a2)
{
	return a1 - a2;
}

int eval_lshift(int a1, int a2)
{
	return a1 << a2;
}

int eval_rshift(int a1, int a2)
{
	return a1 >> a2;
}

int eval_band(int a1, int a2)
{
	return a1 & a2;
}

int eval_bxor(int a1, int a2)
{
	return a1 ^ a2;
}

int eval_bor(int a1, int a2)
{
	return a1 | a2;
}

int eval_land(int a1, int a2)
{
	return (a1 && a2) ? 1 : 0;
}

int eval_lor(int a1, int a2)
{
	return (a1 || a2) ? 1 : 0;
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
	int (*eval)(int a1, int a2);
} ops[] =
{
	/*
	{'_', 10, ASSOC_RIGHT, 1, eval_uminus},
	{'^', 9, ASSOC_RIGHT, 0, eval_exp},
	{'*', 8, ASSOC_LEFT, 0, eval_mul},
	{'/', 8, ASSOC_LEFT, 0, eval_div},
	{'%', 8, ASSOC_LEFT, 0, eval_mod},
	{'+', 6, ASSOC_LEFT, 0, eval_add},
	{'-', 6, ASSOC_LEFT, 0, eval_sub},
	{'(', 0, ASSOC_NONE, 0, NULL},
	{')', 0, ASSOC_NONE, 0, NULL}
	*/

	{'(', 0,   ASSOC_NONE,     0, NULL},
	{')', 0,   ASSOC_NONE,     0, NULL},
	{'#', 17,  ASSOC_RIGHT,    1, eval_uplus},
	{'_', 17,  ASSOC_RIGHT,    1, eval_uminus},
	{'~', 17,  ASSOC_RIGHT,    1, eval_bnot},
	{'!', 17,  ASSOC_RIGHT,    1, eval_lnot},
	{'*', 15,  ASSOC_LEFT,     0, eval_mul},
	{'/', 15,  ASSOC_LEFT,     0, eval_div},
	{'%', 15,  ASSOC_LEFT,     0, eval_mod},
	{'+', 14,  ASSOC_LEFT,     0, eval_add},
	{'-', 14,  ASSOC_LEFT,     0, eval_sub},
	{'<', 13,  ASSOC_LEFT,     0, eval_lshift},
	{'>', 13,  ASSOC_LEFT,     0, eval_rshift},
	{'&', 10,  ASSOC_LEFT,     0, eval_band},
	{'^', 9,   ASSOC_LEFT,     0, eval_bxor},
	{'|', 8,   ASSOC_LEFT,     0, eval_bor},
	{'$', 7,   ASSOC_LEFT,     0, eval_land},
	{'@', 6,   ASSOC_LEFT,     0, eval_lor}
};

struct op_s *getop(char ch)
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
int nopstack = 0;

std::stack<int> numstack;
int nnumstack = 0;

void push_opstack(struct op_s* op)
{
	opstack.push(op);
}

struct op_s* pop_opstack()
{
	struct op_s* op = opstack.top();
	opstack.pop();
	return op;
}

void push_numstack(int num)
{
	numstack.push(num);
}

int pop_numstack()
{
	int num = numstack.top();
	numstack.pop();
	return num;
}

void shunt_op(struct op_s* op)
{
	struct op_s* pop;
	int n1, n2;

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
}

int main(int argc, char *argv[])
{
	std::string expr;
	char tstart = 0;
	struct op_s startop = {'X', 0, ASSOC_NONE, 0, NULL};    /* Dummy operator to mark start */
	struct op_s *op = NULL;
	int n1, n2;
	struct op_s *lastop = &startop;

	std::getline(std::cin, expr);
	for (int i = 0; i < expr.size(); ++i)
	{
		std::cout << "reading " << expr[i] << std::endl;

		if (i + 1 < expr.size())
		{
			if (expr[i] == '&' && expr[i] == expr[i + 1])
			{
				std::cout << "logical AND found" << std::endl;

				expr[i] = '$';
				expr[i + 1] = ' ';
			}
			else if (expr[i] == '|' && expr[i] == expr[i + 1])
			{
				std::cout << "logical OR found" << std::endl;

				expr[i] = '@';
				expr[i + 1] = ' ';
			}
		}

		if (!tstart)
		{
			if ((op = getop(expr[i])))
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
						case '<':
						case '>':
							continue;
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

				std::cout << "filter complete" << std::endl;

				shunt_op(op);
				lastop = op;
			}
			else if (std::isdigit(expr[i]))
			{
				tstart = expr[i];
			}
			else if (!std::isspace(expr[i]))
			{
				//std::cerr << "ERROR: Syntax error" << std::endl;
				//return EXIT_FAILURE;
			}
		}
		else
		{
			if (isspace(expr[i]))
			{
				push_numstack(std::atoi(&tstart));
				tstart = 0;
				lastop = NULL;
			}
			else if ((op = getop(expr[i])))
			{
				push_numstack(std::atoi(&tstart));
				tstart = 0;
				shunt_op(op);
				lastop = op;
			}
			else if (!std::isdigit(expr[i]))
			{
				std::cerr << "ERROR: Syntax error" << std::endl;
				return EXIT_FAILURE;
			}
		}
	}
	if (tstart)
	{
		push_numstack(std::atoi(&tstart));
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
		return EXIT_FAILURE;
	}

	std::cout << numstack.top() << std::endl;

	return EXIT_SUCCESS;
}

