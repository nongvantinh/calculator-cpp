## Definitions

-   A token is a sequence of characters that represents something we consider a unit, such as a number or an operator. 
That’s the way a C++ compiler deals with its source. Actually, “tokenizing” in some form or another is the way most analysis of text starts. Following the example of C++ expression.

## a simple expression grammar:
* **Expression**:
    *   **Term**  
    *   **Expression** "+" **Term** 
        * _addition_   
    *   **Expression** "–" **Term**
        * _subtraction_
* **Term**:
    *   **Primary**
    *   **Term** "*" **Primary**
        * _multiplication_
    *   **Term** "/" **Primary**
        * _division_
    *   **Term** "%" **Primary**
        * _remainder (modulo)_
* **Primary**:
    *   **Number**
    *   "-"**Number**
    *   "+"**Number**
    *   "(" **Expression** ")"
        * _grouping_
    *   "{" **Expression** "}"
        * _grouping_
    *   **Expression**"!"
        * _factorial_
* **Number**:
    *   **floating-point-literal**
