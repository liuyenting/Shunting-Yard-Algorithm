#include <iostream>
#include <stack>
#include <string>

#include <queue>

#include "precedence.h"

/*
 * Stack helper functions
 */
template<class Container, class Stream>
Stream& printOneValueContainer(Stream& outputstream, const Container& container)
{
    typename Container::const_iterator beg = container.begin();
    
    outputstream << "[";
    while(beg != container.end())
        outputstream << " " << *beg++;
    outputstream << "]";

    return outputstream;
}

template<class Type, class Container>
const Container& container(const std::stack<Type, Container>& stack)
{
    struct HackedStack : private std::stack<Type, Container>
    {
        static const Container& container(const std::stack<Type, Container>& stack)
        {
            return stack.*&HackedStack::c;
        }
    };
}

template<class Type, class Container>
const Container& container(const std::queue<Type, Container>& queue)
{
    struct HackedQueue : private std::queue<Type, Container>
    {
        static const Container& container(const std::queue<Type, Container>& queue)
        {
            return queue.*&HackedQueue::c;
        }
    };

    return HackedQueue::container(queue);
}

template<typename Type, template<typename, class Container = std::deque<Type> > class Adapter, class Stream>
Stream& operator<<(Stream& outputstream, const Adapter<Type>& adapter)
{
    return printOneValueContainer(outputstream, container(adapter));
}

int getOperatorWeight(char op)
{
    int weight = -1;

    switch(op)
    {
        case '+':
        case '-':
            weight = 1;
            break;
        case '*':
        case '/':
            weight = 2;
            break;
        case '$':
            weight = 3;
            break;
    }

    return weight;
}

int isRightAssociative(char op)
{
    if(op == '$')
        return true;
    return false;
}



bool hasHigherPrecedence(char op1, char op2)
{
    int op1weight = getOperatorWeight(op1);
    int op2weight = getOperatorWeight(op2);

    if(op1weight == op2weight)
    {
        if(isRightAssociative(op1))
            return false;
        return true;
    }

    return op1weight > op2weight;
}

bool isOperator(std::string token)
{
    // Find the first occurance of the token
    return operators.find(token) != std::string::npos;
}

bool isOperator(const char& parsed)
{
    return operators.find(parsed) != std::string::npos;
}

bool isOperand(std::string token)
{
    return find_if(token.begin(), token.end(), [](const char& c){ return std::isalnum(c); }) != token.end();
}

bool isOperand(const char& c)
{
    return std::isalnum(c);
}

enum tokenType { operator_token, operand_token };

std::string getToken(std::istream& stream, tokenType& type)
{
    // Type0:first-run, Type1:operator, Type2:operandd
    char c;
    std::string token = "";
    
    // determine the character is operand or operator
    bool (*identifier)(const char& c);

    // End of token
    bool eot = false;
    // Type of the token is identified
    bool typeKnown = false;

    do
    {
        stream >> c;
    
        switch(c)
        {
            // Filter out characters to ignore
            case ' ':
            case ',':
                continue;
            // New line character indicates no more input for current query
            case '\n':
                eot = true;
                break;
            default:
                if(typeKnown)
                {
                    if(identifier(c))
                        token += c;
                    else
                        eot = true;
                }
                else
                {
                    if(isOperator(c))
                    {
                        identifier = isOperator;
                        token += c;
                        typeKnown = true;
                        type = operator_token;
                    }
                    else if(isOperand(c))
                    {
                        identifier = isOperand;
                        token += c;
                        typeKnown = true;
                        type = operand_token;
                    }
                }
        }
    }while(eot);
}

std::string evaluate(std::istream& input, int& result)
{
    // Temporary evaluation
    std::string expression;
    std::getline(input, expression);

    // Declare a stack from the STL
    std::stack<char> stack;
    std::string postfix = "";

    std::string token;
    tokenType type;
    do
    {
        token = getToken(std::cin, type);

        // Scanning each character from left
        // If character is a delimitter, move on
        if(type == operator_token)
        {
            // If character is an operator, pop two elements from stack
            while(!stack.empty() && stack.top() != '(' &&
                    hasHigherPrecedence(stack.top(), expression[i]))
            {
                postfix += stack.top();
                stack.pop();
            }

            // Push the result back after performing the operation
            stack.push(expression[i]);
        }
        else if(isOperand(expression[i]))
        {
            postfix += expression[i];
        }
        else if(expression[i] == '(')
        {
            stack.push(expression[i]);
        }
        else if(expression[i] == ')')
        {
            while(!stack.empty() && stack.top() != '(')
            {
                postfix += stack.top();
                stack.pop();
            }
            stack.pop();
        }
    }

    while(!stack.empty())
    {
        postfix += stack.top();
        stack.pop();
    }

    return postfix;
}

int main()
{
    initPrecedenceTable();

    std::cout << "INFIX:\t";

    // Start the evaluation
    int result;
    std::cout << "POSTFIX:\t" << evaluate(std::cin, result) << std::endl;
    std::cout << "RESULT:\t" << result << std::endl;

    return 0;
}
