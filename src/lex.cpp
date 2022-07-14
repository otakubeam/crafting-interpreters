#include <iostream>

// What's the difference between token and token type?

enum TokenType {
    NUMBER,
    STRING,

    PLUS,
    MINUS,

    PRINT,
    FUN, // FUN foo
    VAR, // VAR name
    // Wait, what?? name and foo should instead be its own things
    // perhaps TokenType ID
    IDENTIFIER,
};


// I wanna take a char stream
// and convert it into tokens
//
// TokenType NextToken(ScanInfo& info) {
//      // info contains the string we are trying to lex
//      // and our current position (anything else?)
// }
//
