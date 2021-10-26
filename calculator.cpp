
#include <iostream>
#include <stdexcept>

class Token
{
public:
    Token(char p_kind, double p_value = 0.0);

    void set_kind(char p_kind);
    char kind() const;

    void set_value(double p_value);
    double value() const;

private:
    // what kind of token.
    char m_kind;
    // for numbers: a value.
    double m_value;
};

Token::Token(char p_kind, double p_value) : m_kind(p_kind), m_value(p_value) {}

void Token::set_kind(char p_kind) { m_kind = p_kind; }
char Token::kind() const { return m_kind; }

void Token::set_value(double p_value) { m_value = p_value; }
double Token::value() const { return m_value; }

class TokenStream
{
public:
    TokenStream();
    Token get();
    void putback(Token p_token);

private:
    bool m_full;
    Token m_buffer;
};

TokenStream::TokenStream() : m_full(false), m_buffer(' ') {}

// The putback() member function puts its argument back into the TokenStream's m_buffer:
void TokenStream::putback(Token p_token)
{
    if (m_full)
        throw std::runtime_error("putback() into a full buffer");
    m_buffer = p_token; // copy p_token to m_buffer
    m_full = true;      // m_buffer is now m_full
}

Token TokenStream::get()
{
    if (m_full)
    {
        m_full = false;
        return m_buffer;
    }

    char kind(' ');
    std::cin >> kind; // note that >> skips whitespace (space, newline, tab, etc.)

    switch (kind)
    {
    case '=': // for "print"
    case 'q': // for "quit"
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
        return Token(kind); // let each character represent itself.
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        // Put digit back to get a complete number.
        std::cin.putback(kind);
        double value(0.0);
        std::cin >> value;
        return Token('8', value); // let '8' represent "a number".
    }
    default:
        throw std::runtime_error("Bad token");
    }
}

TokenStream stream; // provides get() and putback().

// deal with + and -.
double expression();
// deal with numbers and parentheses.
double primary();
// deal with *, /, and %.
double term();

int main()
try
{
    double result(0.0);
    while (std::cin)
    {
        Token token = stream.get();

        if (token.kind() == 'q')
            break;               // 'q' for quit
        if (token.kind() == '=') // '=' for "print now"
            std::cout << result << '\n';
        else
            stream.putback(token);
        result = expression();
    }
}
catch (std::exception &e)
{
    std::cerr << "error: " << e.what() << '\n';
    return 1;
}
catch (...)
{
    std::cerr << "Oops: unknown exception!\n";
    return 2;
}

// deal with + and -
double expression()
{
    double left = term();   // read and evaluate a Term.
    Token token = stream.get(); // get the next token from token stream.

    while (true)
    {
        switch (token.kind())
        {
        case '+':
            left += term(); // evaluate Term and add.
            token = stream.get();
            break;
        case '-':
            left -= term(); // evaluate Term and subtract.
            token = stream.get();
            break;
        default:
            stream.putback(token); // put token back into the token stream.
            return left;       // finally: no more + or -: return the answer.
        }
    }
}

// deal with *, /, and %.
double term()
{
    double left = primary();
    Token token = stream.get(); // get the next token from token stream.

    while (true)
    {
        switch (token.kind())
        {
        case '*':
            left *= primary();
            token = stream.get();
            break;
        case '/':
        {
            double d = primary();
            if (d == 0)
                throw std::runtime_error("divide by zero");
            left /= d;
            token = stream.get();
            break;
        }
        default:
            stream.putback(token); // put token back into the token stream.
            return left;
        }
    }
    return 0.0;
}

// deal with numbers and parentheses.
double primary()
{
    Token token = stream.get();
    switch (token.kind())
    {
    case '(': // handle '(' expression ')'.
    {
        double result(expression());
        token = stream.get();
        if (token.kind() != ')')
            throw std::runtime_error("')' expected.");
        return result;
    }
    case '8':                 // we use '8' to represent a number.
        return token.value(); // return the number's value.
    default:
        throw std::runtime_error("primary expected.");
    }
    return 0.0;
}
