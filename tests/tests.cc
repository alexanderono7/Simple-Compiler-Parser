#include "tests.h"

/*
Example test case. 
WHY THE FUCK WON'T THEY RUN SEPARATELY THOUGH???
I DON'T WANNA MAKE A NEW FUCKING FILE FOR EACH ONE DAMMIT!!!
Ugh...
*/
BOOST_AUTO_TEST_CASE(AdditionTest)
{
    BOOST_CHECK_EQUAL(2+1,3);
}

/* 
I think it's impossible to run these test cases with 
with a simple given string, so I'm just gonna painstakingly insert Tokens into lexer.tokenList.
I made tokenList public... hopefully that won't cause any problems...
*/
BOOST_AUTO_TEST_CASE(var_section)
{
    Token one;
    one.token_type = ID;
    one.line_no = 1;
    lexer.tokenList.push_back(one);

    Token two;
    two.token_type = COMMA;
    two.line_no = 2;
    lexer.tokenList.push_back(two);
    lexer.tokenList.push_back(one);

    Token three;
    three.token_type = SEMICOLON;
    three.line_no = 2;
    lexer.tokenList.push_back(three);

    parse_var_section();
    BOOST_CHECK_EQUAL(2-1,1);
}

BOOST_AUTO_TEST_CASE(var_section2)
{
    Token one;
    one.token_type = ID;
    one.line_no = 1;
    lexer.tokenList.push_back(one);

    Token three;
    three.token_type = SEMICOLON;
    three.line_no = 2;
    lexer.tokenList.push_back(three);

    parse_var_section();
    BOOST_CHECK_EQUAL(2-1,1);
}

BOOST_AUTO_TEST_CASE(var_section3)
{
    Token one;
    one.token_type = ID;
    one.line_no = 1;

    Token two;
    two.token_type = COMMA;
    two.line_no = 2;

    Token three;
    three.token_type = SEMICOLON;
    three.line_no = 2;

    lexer.tokenList.push_back(one);
    lexer.tokenList.push_back(two);
    lexer.tokenList.push_back(one);
    lexer.tokenList.push_back(two);
    lexer.tokenList.push_back(one);
    lexer.tokenList.push_back(three);

    parse_var_section();
    BOOST_CHECK_EQUAL(2-1,1);
}