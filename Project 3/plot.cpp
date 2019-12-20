//
//  plot.cpp
//  plot
//
//  Created by ALEX CHEN (: on 10/22/19.
//  Copyright © 2019 cs. All rights reserved.
//

// include libraries that are utilized
#include "grid.h" // to include previously written functions
#include <iostream> // to use cout, cin, etc
#include <sstream> // to convert strings to int
#include <cctype> // for isprint method
#include <string> // to use strings
using namespace std;

// declare and initialize constant variables, used in determining horizontal or vertical lines
const int HORIZ = 0;
const int VERT = 1;

// declare and initialize constant variables, used in determining foreground or background mode
const int FG = 0;
const int BG = 1;

// implemented functions
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);
int performCommands(string commandString, char& plotChar, int& mode, int& badPos);
void plotHorizontalLine2(int r, int c, int distance, char ch);
void plotVerticalLine2(int r, int c, int distance, char ch);

// warmup functions (unused)
//void plotHorizontalLine(int r, int c, int distance, char ch);
//void plotVerticalLine(int r, int c, int distance, char ch);
//void plotRectangle(int r, int c, int height, int width, char ch);

// prompts user for command string, calls performCommands, and return possible errors accordingly
int main()
{
    setSize(20, 30);
    char currentChar = '*';
    int currentMode = FG;
    for (;;)
    {
        cout << "Enter a command string: ";
        string cmd;
        getline(cin, cmd);
        if (cmd == "")
            break;
        int position;
        // calls performCommands which also returns int
        int status = performCommands(cmd, currentChar, currentMode, position);
        // int used to determine if error is present or not
        switch (status)
        {
          case 0:
                draw();
                break;
          case 1:
                cout << "Syntax error at position " << position+1 << endl;
                break;
          case 2:
                cout << "Cannot perform command at position " << position+1 << endl;
                break;
          default:
                // It should be impossible to get here.
                cerr << "performCommands returned " << status << "!" << endl;
        }
    }
    
//    setSize(15, 12);
//    const int middle = getCols() / 2;
//    setChar(6, middle, 'E');
//    setChar(8, middle, 'L');
//    setChar(9, middle, 'O');
//    setChar(7, middle, 'L');
//    setChar(5, middle, 'H');
//    if (getChar(6, middle) == 'E')
//        setChar(10, middle, '!');
//    if (!(plotLine(4, 6, 3, VERT, 'y', BG)))
//    {
//        cout << "hi";
//    }
//    draw();
}

// method to parse command string and setChar according to plotChar, mode, and badPos
// plotChar, mode, and badPos are called by reference, as after each call of performCommand,
// these values should not be changed
int performCommands(string commandString, char& plotChar, int& mode, int& badPos)
{
    // row, column, and index in command string must be reset for each call of performCommands
    int r=1;
    int c=1;
    int i=0;
    // parses through command string
    while (i<commandString.size())
    {
        // if command is c, position is reset to (1,1), plotChar to *, mode to foreground,
        // and grid to all white spaces
        if (tolower(commandString[i]) == 'c')
        {
            r=1;
            c=1;
            plotChar = '*';
            mode = 0;
//            for (int a=1; a<=getRows(); a++)
//            {
//                for (int b=1; b<=getCols(); b++)
//                {
//                    setChar(a, b, ' ');
//                }
//            }
            clearGrid(); // resets grid, filling each position with a space
            i ++;
            
        }
        // if command is f or b and a valid character follows, plotChar is set to
        // the valid character and mode is set to foreground/background
        // otherwise, badPos is set to the index of mistake in string (i) and 1 is returned
        // i is incremented twice if successful
        else if (tolower(commandString[i]) == 'f')
        {
            if (isprint(commandString[i+1]))
            {
                plotChar = commandString[i+1];
                mode = 0;
            }
            else
            {
                badPos = i+1;
                return 1;
            }
            i += 2;
        }
        else if (tolower(commandString[i]) == 'b')
        {
            if (isprint(commandString[i+1]))
            {
                plotChar = commandString[i+1];
                mode = 1;
            }
            else
            {
                badPos = i+1;
                return 1;
            }
            i += 2;
        }
        else if (tolower(commandString[i]) == 'h')
        {
            // deals with minus signs following h
            if (commandString[i+1] == '-')
            {
                i ++;
                // for h commands, this deals with two following characters
                if (isdigit(commandString[i+1]) && isdigit(commandString[i+2]))
                {
                    // string to int
                    stringstream geek(commandString.substr(i+1, 2));
                    int distance = 0;
                    geek >> distance;
                    distance = -1*distance;
                    // calls plotLine and evaluates for mistakes
                    // r, c, HORIZ, plotChar, and mode will never raise errors
                    // thus, if function returns false, drawing leaves grid
                    if (!(plotLine(r, c, distance, HORIZ, plotChar, mode)))
                    {
                        i --;
                        badPos = i;
                        return 2;
                    }
                    c += distance;
                    i += 3;
                }
                // for h commands, this deals with one following character
                else if (isdigit(commandString[i+1]))
                {
                    // string to int
                    stringstream geek(commandString.substr(i+1, 1));
                    int distance = 0;
                    geek >> distance;
                    distance = -1*distance;
                    // calls plotLine and evaluates for mistakes
                    // r, c, HORIZ, plotChar, and mode will never raise errors
                    // thus, if function returns false, drawing leaves grid
                    if (!(plotLine(r, c, distance, HORIZ, plotChar, mode)))
                    {
                        i --;
                        badPos = i;
                        return 2;
                    }
                    c += distance;
                    i += 2;
                }
                // if one or two valid numbers are not following, then there is a syntax error
                else
                {
                    i ++;
                    badPos = i;
                    return 1;
                }
            }
            // deals with following characters, when no minus sign follows h
            else
            {
                // for h commands, this deals with two following characters
                if (isdigit(commandString[i+1]) && isdigit(commandString[i+2]))
                {
                    // string to int
                    stringstream geek(commandString.substr(i+1, 2));
                    int distance = 0;
                    geek >> distance;
                    // calls plotLine and evaluautes mistakes
                    // r, c, HORIZ, plotChar, and mode will never raise errors
                    // thus, if function returns false, drawing leaves grid
                    if (!(plotLine(r, c, distance, HORIZ, plotChar, mode)))
                    {
                        badPos = i;
                        return 2;
                    }
                    c += distance;
                    i += 3;
                }
                // for h commands, this deals with one following character
                else if (isdigit(commandString[i+1]))
                {
                    // string to int
                    stringstream geek(commandString.substr(i+1, 1));
                    int distance = 0;
                    geek >> distance;
                    // calls plotLine and evaluates mistakes
                    // r, c, HORIZ, plotChar, and mode will never raise errors
                    // thus, if function returns false, drawing leaves grid
                    if (!(plotLine(r, c, distance, HORIZ, plotChar, mode)))
                    {
                        badPos = i;
                        return 2;
                    }
                    c += distance;
                    i += 2;
                }
                // if one or two digits are not following, then there is a syntax error
                else
                {
                    i ++;
                    badPos = i;
                    return 1;
                }
            }
        }
        // same functions as previous block with 'h'
        else if (tolower(commandString[i]) == 'v')
        {
            if (commandString[i+1] == '-')
            {
                i ++;
                if (isdigit(commandString[i+1]) && isdigit(commandString[i+2]))
                {
                    stringstream geek(commandString.substr(i+1, 2));
                    int distance = 0;
                    geek >> distance;
                    distance = -1*distance;
                    if (!(plotLine(r, c, distance, VERT, plotChar, mode)))
                    {
                        i --;
                        badPos = i;
                        return 2;
                    }
                    r += distance;
                    i += 3;
                }
                else if (isdigit(commandString[i+1]))
                {
                    stringstream geek(commandString.substr(i+1, 1));
                    int distance = 0;
                    geek >> distance;
                    distance = -1*distance;
                    if (!(plotLine(r, c, distance, VERT, plotChar, mode)))
                    {
                        i --;
                        badPos = i;
                        return 2;
                    }
                    r += distance;
                    i += 2;
                }
                else
                {
                    i ++;
                    badPos = i;
                    return 1;
                }
            }
            else
            {
                if (isdigit(commandString[i+1]) && isdigit(commandString[i+2]))
                {
                    stringstream geek(commandString.substr(i+1, 2));
                    int distance = 0;
                    geek >> distance;
                    if (!(plotLine(r, c, distance, VERT, plotChar, mode)))
                    {
                        badPos = i;
                        return 2;
                    }
                    r += distance;
                    i += 3;
                }
                else if (isdigit(commandString[i+1]))
                {
                    stringstream geek(commandString.substr(i+1, 1));
                    int distance = 0;
                    geek >> distance;
                    if (!(plotLine(r, c, distance, VERT, plotChar, mode)))
                    {
                        badPos = i;
                        return 2;
                    }
                    r += distance;
                    i += 2;
                }
                else
                {
                    i ++;
                    badPos = i;
                    return 1;
                }
            }
        }
        else
        {
            badPos = i;
            return 1;
        }
    }
    return 0;
}

// unused
//void plotHorizontalLine(int r, int c, int distance, char ch)
//{
//    if (distance >= 0)
//    {
//        for (int i=c; i<=(c+distance); i++)
//        {
//            if (i<=getCols() && i>=1 && r<=getRows() && r>=1)
//                setChar(r, i, ch);
//        }
//    }
//}

// used to plot horizontal lines
void plotHorizontalLine2(int r, int c, int distance, char ch, int fgbg)
{
    if (distance >= 0)
    {
        for (int i=c; i<=(c+distance); i++)
        {
            if (i<=getCols() && i>=1 && r<=getRows() && r>=1)
            {
                // allows all characters to be reset if in foreground mode
                if (fgbg == 0)
                {
                    setChar(r, i, ch);
                }
                else if (fgbg == 1)
                {
                    // only allows character to be reset if character is a blank space and mode is background
                    if (getChar(r, i) == ' ')
                    {
                        setChar(r, i, ch);
                    }
                }
            }
        }
    }
    else
    {
        // with negative distances, for loop must be modified
        // rest of block is same as in previous block
        for (int i=c; i>=(c+distance); i--)
        {
            if (i<=getCols() && i>=1 && r<=getRows() && r>=1)
            {
                if (fgbg == 0)
                {
                    setChar(r, i, ch);
                }
                else if (fgbg == 1)
                {
                    if (getChar(r, i) == ' ')
                    {
                        setChar(r, i, ch);
                    }
                }
            }
        }
    }
}

// unused
//void plotVerticalLine(int r, int c, int distance, char ch)
//{
//    if (distance >= 0)
//    {
//        for (int i=r; i<=(r+distance); i++)
//        {
//            if (i<=getRows() && i>=1 && c<=getCols() && c>=1)
//            {
//                setChar(i, c, ch);
//            }
//        }
//    }
//}


// same as plotHorizontalLine2, except incremements the row value
// rather than column to create vertical lines
void plotVerticalLine2(int r, int c, int distance, char ch, int fgbg)
{
    if (distance >= 0)
    {
        for (int i=r; i<=(r+distance); i++)
        {
            if (i<=getRows() && i>=1 && c<=getCols() && c>=1)
            {
                if (fgbg == 0)
                {
                    setChar(i, c, ch);
                }
                else if (fgbg == 1)
                {
                    if (getChar(r, c) == ' ')
                    {
                        setChar(i, c, ch);
                    }
                }
            }
        }
    }
    else
    {
        for (int i=r; i>=(r+distance); i--)
        {
            if (i<=getRows() && i>=1 && c<=getCols() && c>=1)
            {
                if (fgbg == 0)
                {
                    setChar(i, c, ch);
                }
                else if (fgbg == 1)
                {
                    if (getChar(r, c) == ' ')
                    {
                        setChar(i, c, ch);
                    }
                }
            }
        }
    }
}

// unused
//void plotRectangle(int r, int c, int height, int width, char ch)
//{
//    if (height >= 0 && width >= 0)
//    {
//        plotHorizontalLine(r, c, width, ch);
//        plotHorizontalLine(r+height, c, width, ch);
//        plotVerticalLine(r, c, height, ch);
//        plotVerticalLine(r, c+width, height, ch);
//    }
//}


// plots the actual line
// called in performCommands
// calls horizontal and vertical line drawing functions
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg)
{
    // if blocks to determine if invalid arguments are passed
    if (dir != 0 && dir != 1) // cannot be true based on previously initialized values
    {
        return false;
    }
    if (fgbg != 0 && fgbg != 1) // cannot be true based on previously initialized values
    {
        return false;
    }
    if (r>getRows() || r<1 || c>getCols() || c<1) // cannot be true based on previously initialized values
    {
        return false;
    }
    if (dir == 0)
    {
        // if drawing will leave grid on left or right side, false is returned
        if ((c+distance)>getCols() || (c+distance)<1)
        {
            return false;
        }
    }
    if (dir == 1)
    {
        // if drawing will leave grid on top or bottom, false is returned
        if (r+distance>getRows() || r+distance<1)
        {
            return false;
        }
    }
    if (!(isprint(plotChar)))
    {
        return false;
    }
    // if nothing is incorrect at this point, the lines can be plotted
    if (dir == 0)
    {
        plotHorizontalLine2(r, c, distance, plotChar, fgbg);
    }
    if (dir == 1)
    {
        plotVerticalLine2(r, c, distance, plotChar, fgbg);
    }
    return true;
}
