
//------------------------------------------------------------------------------
#include <iostream>
#include <stdexcept>

class Token
{
public:
    char kind;     // what kind of token
    double value;  // for numbers: a value
    Token(char ch) // make a Token from a char
        : kind(ch), value(0)
    {
    }
    Token(char ch, double val) // make a Token from a char and a double
        : kind(ch), value(val)
    {
    }
};

//------------------------------------------------------------------------------

class TokenStream
{
public:
    TokenStream();        // make a TokenStream that reads from cin
    Token get();           // get a Token (get() is defined elsewhere)
    void putback(Token t); // put a Token back
private:
    bool full;    // is there a Token in the buffer?
    Token buffer; // here is where we keep a Token put back using putback()
};

//------------------------------------------------------------------------------

// The constructor just sets full to indicate that the buffer is empty:
TokenStream::TokenStream()
    : full(false), buffer(0) // no Token in buffer
{
}

//------------------------------------------------------------------------------

// The putback() member function puts its argument back into the TokenStream's buffer:
void TokenStream::putback(Token t)
{
    if (full)
        throw std::runtime_error("putback() into a full buffer");
    buffer = t;  // copy t to buffer
    full = true; // buffer is now full
}

//------------------------------------------------------------------------------

Token TokenStream::get()
{
    if (full)
    { // do we already have a Token ready?
        // remove token from buffer
        full = false;
        return buffer;
    }

    char ch;
    std::cin >> ch; // note that >> skips whitespace (space, newline, tab, etc.)

    switch (ch)
    {
    case ';': // for "print"
    case 'q': // for "quit"
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
        return Token(ch); // let each character represent itself
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
        std::cin.putback(ch); // put digit back into the input stream
        double val;
        std::cin >> val;             // read a floating-point number
        return Token('8', val); // let '8' represent "a number"
    }
    default:
        throw std::runtime_error("Bad token");
    }
}

//------------------------------------------------------------------------------

TokenStream ts; // provides get() and putback()

//------------------------------------------------------------------------------

double expression(); // declaration so that primary() can call expression()

//------------------------------------------------------------------------------

// deal with numbers and parentheses
double primary()
{
    Token t = ts.get();
    switch (t.kind)
    {
    case '(': // handle '(' expression ')'
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')')
            throw std::runtime_error("')' expected");
        return d;
    }
    case '8':           // we use '8' to represent a number
        return t.value; // return the number's value
    default:
        throw std::runtime_error("primary expected");
    }
}

//------------------------------------------------------------------------------

// deal with *, /, and %
double term()
{
    double left = primary();
    Token t = ts.get(); // get the next token from token stream

    while (true)
    {
        switch (t.kind)
        {
        case '*':
            left *= primary();
            t = ts.get();
            break;
        case '/':
        {
            double d = primary();
            if (d == 0)
                throw std::runtime_error("divide by zero");
            left /= d;
            t = ts.get();
            break;
        }
        default:
            ts.putback(t); // put t back into the token stream
            return left;
        }
    }
}

//------------------------------------------------------------------------------

// deal with + and -
double expression()
{
    double left = term(); // read and evaluate a Term
    Token t = ts.get();   // get the next token from token stream

    while (true)
    {
        switch (t.kind)
        {
        case '+':
            left += term(); // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            left -= term(); // evaluate Term and subtract
            t = ts.get();
            break;
        default:
            ts.putback(t); // put t back into the token stream
            return left;   // finally: no more + or -: return the answer
        }
    }
}

//------------------------------------------------------------------------------

int main()
try
{
    double val(0.0);
    while (std::cin)
    {
        Token t = ts.get();

        if (t.kind == 'q')
            break;         // 'q' for quit
        if (t.kind == ';') // ';' for "print now"
            std::cout << "=" << val << '\n';
        else
            ts.putback(t);
        val = expression();
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

//------------------------------------------------------------------------------