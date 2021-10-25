/*
A token is a sequence of characters that represents something we consider a unit, such as a number or an operator. That’s the
way a C++ compiler deals with its source. Actually, “tokenizing” in some form or another is the way most analysis of text
starts. Following the example of C++ expression.
*/
/*
// a simple expression grammar:
Expression:
    Term
    Expression "+" Term
    // addition
    Expression "–" Term
    // subtraction
Term:
    Primary
    Term "*" Primary
    // multiplication
    Term "/" Primary
    // division
    Term "%" Primary
    // remainder (modulo)
Primary:
    Number
    "(" Expression ")"
    // grouping
Number:
        floating-point-literal
*/