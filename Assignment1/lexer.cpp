#include <iostream>
#include <fstream>
#include <string>


using namespace std;

// DFSM Table
// int DFSM_table[10][7] = {
// 					// Inputs:	         l  d  _  .  ws 
//                           { 0, 1, 6, 5, 5, 0, 5},		// State 0 (starting state)
//                           { 1, 2, 3, 4, 5, 0, 5},		// State 1 (accepting state) accept identifier
//                           { 2, 2, 3, 4, 5, 0, 5},		// State 2 (accepting state) accept identifier
//                           { 3, 2, 3, 4, 5, 0, 5},		// State 3 (accepting state) accept identifier
//                           { 4, 2, 3, 4, 5, 0, 5},		// State 4 (accepting state) accept identifier
//                           { 5, 5, 5, 5, 5, 5, 5},		// State 5
//                           { 6, 5, 6, 5, 7, 0, 5},		// State 6 (accepting state) accept integer
//                           { 7, 8, 9, 8, 8, 0, 5},		// State 7
//                           { 8, 5, 9, 5, 5, 0, 5},		// State 8
//                           { 9, 8, 9, 8, 8, 0, 5}		  // State 9 (accepting state) accept real
// };

int DFSM_table[24][15] = {

                          {  0,  1,  2, 22, 22, 22,  3,  4,  5,  6,  7,  8,  9, 10, 23},        // state 0 : starting state
                          {  1, 11, 12, 13, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 1 : single letter string
                          {  2, 22,  2, 22, 14, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 2 : integer
                          {  3, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 3 : separator
                          {  4, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 4 : +
                          {  5, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 5 : -
                          {  6, 22, 22, 22, 22, 22, 22, 22, 22, 22, 19, 22, 22, 22, 22},        // state 6 : /
                          {  7, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 7 : *
                          {  8, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 15, 22, 22, 16},        // state 8 : =
                          {  9, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 17, 22, 22, 22},        // state 9 : <
                          { 10, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 18, 22, 22, 22},        // state 10: !
                          { 11, 11, 12, 13, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 11: string + letter
                          { 12, 11, 12, 13, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 12: string + digit
                          { 13, 11, 12, 13, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 13: string + _
                          { 14, 22, 20, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 14: integer + .
                          { 15, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 15: ==
                          { 16, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 16: =>
                          { 17, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 17: <=
                          { 18, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 18: !=
                          { 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 21, 19, 19, 19, 19},        // state 19: inside comment
                          { 20, 22, 20, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 20: real
                          { 21, 19, 19, 19, 19, 19, 19, 19, 19,  0, 21, 19, 19, 19, 19},        // state 21: looking for "/" after "*"
                          { 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},        // state 22: invalid
                          { 23, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22}
                          //input   i   l   d   _   .  ws  sp   +   -   /   *   =   <   !   >          i = index, l = letter, d = digit, _ = underscore, ws = whitespace, sp = separator
                          //col #   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14
};

string KEYWORDS[] = { "integer", "if", "else", "endif", "while", "return", "get", "put", "boolean", "real", "function", "true", "false" };
char SEPARATORS[] = { '#', '(', ')', '{', '}', ';', ',' };

int keywordsSize = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]);
int separatorSize = sizeof(SEPARATORS) / sizeof(SEPARATORS[0]);
// string TESTCASE = "testing 123 test 456"

string printTokenLexeme(string token, int currentState)
{

    if (currentState == 1 || currentState == 11 || currentState == 12 || currentState == 13)      // check for accepting states of strings
    {
        string lexeme = "";
        for (int i = 0; i < keywordsSize; ++i)                                                                  // check if identifier is a keyword
        {
            if (KEYWORDS[i] == token)
            {
                return "\"" + token + "\" ==> KEYWORD\n";
            }
            else {
                lexeme = " ==> IDENTIFIER\n";
            }
        }
        return "\"" + token + "\"" + lexeme;                                                    // print token + "IDENTIFIER" lexeme
    }
    else if (currentState == 2)                                                                // accepting state for integers
    {
        return "\"" + token + "\" ==> INTEGER\n";                                               // print token + "INTEGER" lexeme
    }

    else if (currentState == 20)                                                                // accepting state of reals
    {
        return "\"" + token + "\" ==> REAL\n";                                                  // print token + "REAL" lexeme
    }
    else if (currentState == 4 || currentState == 5 || currentState == 6 || currentState == 7
        || currentState == 8 || currentState == 9 || currentState == 15 || currentState == 16
        || currentState == 17 || currentState == 18 || currentState == 23)
    {
        return "\"" + token + "\" ==> OPERATOR\n";
    }
    else if (currentState == 3)
    {
        return "\"" + token + "\" ==> SEPARATOR\n";
    }
    else if (currentState != 19 && currentState != 21)
    {
        return "\"" + token + "\" ==> INVALID TOKEN\n";                                         // unknown strings are invalid
    }
}

bool isSeparator(char character)
{
    for (size_t i = 0; i < separatorSize; ++i)
    {
        if (character == SEPARATORS[i])
        {
            return true;
        }
    }
    return false;
}

int getNextState(int currentState, char currentChar)                                      // check with DFSM to get next state
{
    if (isalpha(currentChar))                                                                // check if char is a letter
    {
        return DFSM_table[currentState][1];
    }
    else if (isdigit(currentChar))                                                           // check if char is a digit
    {
        return DFSM_table[currentState][2];
    }
    else if (currentChar == '_')                                                             // check if char is an underscore
    {
        return DFSM_table[currentState][3];
    }
    else if (currentChar == '.')                                                             // check if char is a period
    {
        return DFSM_table[currentState][4];
    }
    else if (isSeparator(currentChar))
    {
        return DFSM_table[currentState][6];
    }
    else if (currentChar == '+')
    {
        return DFSM_table[currentState][7];
    }
    else if (currentChar == '-')
    {
        return DFSM_table[currentState][8];
    }
    else if (currentChar == '/')
    {
        return DFSM_table[currentState][9];
    }
    else if (currentChar == '*')
    {
        return DFSM_table[currentState][10];
    }
    else if (currentChar == '=')
    {
        return DFSM_table[currentState][11];
    }
    else if (currentChar == '<')
    {
        return DFSM_table[currentState][12];
    }
    else if (currentChar == '!')
    {
        return DFSM_table[currentState][13];
    }
    else if (currentChar == '>')
    {
        return DFSM_table[currentState][14];
    }
    else {
        return DFSM_table[22][0];
    }

}

void lexer(fstream& fin, ofstream& outputFile)                                     // function that separates tokens
{
    // fstream fin(inputFileName, fstream::in);

    // ofstream outputFile;
    // outputFile.open(outputFileName);

    char character;
    int currentState = 0;
    string token = "";

    while (fin.get(character))                                                                   // check if can get next char
    {
        if (currentState == 22 && token.size() > 1)
        {
            fin.unget();
            fin.unget();
            string temp = token;
            if (!temp.empty())
            {
                temp.resize(token.size() - 1);
            }
            int stringIterator = 0;
            int tempState = 0;
            for (int i = 0; i < temp.size(); ++i)
            {
                tempState = getNextState(tempState, temp[i]);
            }
            if (tempState != 0 && tempState != 10 && tempState != 14
                && tempState != 19 && tempState != 21 && tempState != 22)
            {
                cout << printTokenLexeme(temp, tempState);
                outputFile << printTokenLexeme(temp, tempState);
                token = "";
                currentState = 0;
            }
            else                                                                                    // token was not valid, proceed as normal
            {
                fin.get(character);
                fin.get(character);
                if (isspace(character) && token != "")                                                 // if char is a whitespace and token 
                {                                                                                     // string is not empty
                    cout << printTokenLexeme(token, currentState);                                      // print out the token and lexeme
                    outputFile << printTokenLexeme(token, currentState);                                // store in output file
                    token = "";                                                                         // reset token string
                    currentState = 0;                                                                   // reset state to starting state
                }
                else if (!isspace(character))
                {
                    token += character;
                    currentState = getNextState(currentState, character);
                }
            }
        }
        else if (currentState != 21)
        {
            if (currentState == 19)
            {
                token = "";
            }
            if (!isspace(character))                                                                 // if next char is a not a whitespace
            {
                if (currentState != 19 && currentState != 21)                                         // concat token with char if not inside comment
                {
                    token += character;
                }
                currentState = getNextState(currentState, character);                                 // call function to get next state
            }

            if (isspace(character) && token != "")                                                   // if char is a whitespace and token 
            {                                                                                       // string is not empty
                cout << printTokenLexeme(token, currentState);                                        // print out the token and lexeme
                outputFile << printTokenLexeme(token, currentState);                                  // store in output file
                token = "";                                                                           // reset token string
                currentState = 0;                                                                     // reset state to starting state
            }
        }
        else if (currentState == 21 && character == '/')
        {
            currentState = DFSM_table[currentState][9];
        }
    }
    fin.close();
    outputFile.close();
}

int main()
{
    cout << endl << endl;
    cout << "Running test case 1: ";
    cout << endl;
    // printFile("input1.txt");
    fstream inFile1;
    ofstream outFile1;
    inFile1.open("input1.txt");
    outFile1.open("output1.txt");
    lexer(inFile1, outFile1);
    inFile1.close();
    outFile1.close();
    cout << endl << endl;
    cout << "Running test case 2: ";
    cout << endl;
    // printFile("input2.txt");
    fstream inFile2;
    ofstream outFile2;
    inFile2.open("input2.txt");
    outFile2.open("output2.txt");
    lexer(inFile2, outFile2);
    inFile2.close();
    outFile2.close();
    cout << endl << endl;
    cout << endl << endl;
    cout << "Running test case 3: ";
    // printFile("input3.txt");
    fstream inFile3;
    ofstream outFile3;
    inFile3.open("input3.txt");
    outFile3.open("output3.txt");
    lexer(inFile3, outFile3);
    inFile3.close();
    outFile3.close();
    cout << endl << endl;
    cout << endl << endl;
    // system("pause");


    return 0;
}



