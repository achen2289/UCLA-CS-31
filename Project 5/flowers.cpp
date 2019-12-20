//
//  flowers.cpp
//  Project_5
//
//  Created by ALEX CHEN (: on 11/14/19.
//  Copyright © 2019 cs. All rights reserved.
//

#define _CRT_SECURE_NO_DEPRECATE

#include "utilities.h"
#include <iostream>
#include <cstring>
using namespace std;

// method prototype
int playOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum);

// text file location
const char WORDFILENAME[] = "/Users/alexchen/Desktop/CS31/Project_5/smallwords.txt";

// main to prompt user for input, calculate average score, min, and max, and invoke playOneRound
int main()
{
    // declare array holding all possible words
    char w[9000][7];
    // place words into array
    // declare and initialize int n, containing number of words in w
    int n = getWords(w, 9000, WORDFILENAME);
    
    // ensures there is at least one word in w
    if (n < 1)
    {
        cout << "No words were loaded, so I can't play the game." << endl;
        return 0;
    }
    
    // initial questions
    cout << "How many rounds do you want to play? ";
    int rounds;
    cin >> rounds;
    cin.ignore(10000, '\n');
    
    // deals with if negative or no rounds are inputted
    if (rounds <= 0)
    {
        cout << "The number of rounds must be positive." << endl;
        return 0;
    }
    
    cout << endl;
    int total = 0;
    int i = 1;
    while (i <= rounds)
    {
        // random index for word to be retreived from w
        int mysteryIndex = randInt(0, n-1);
        cout << "Round " << i << endl;
        cout << "The mystery word is " << strlen(w[mysteryIndex]) << " letters long." << endl;
        int x = playOneRound(w, n, mysteryIndex);
        int min;
        int max;
        // first loop, min and max declared and initialized
        if (i == 1)
        {
            min = x;
            max = x;
        }
        // total calculated for average value display
        total += x;
        if (x < min)
        {
            min = x;
        }
        else if (x > max)
        {
            max = x;
        }
        if (x != 1)
        {
            cout << "You got it in " << x << " tries." << endl;
        }
        else
        {
            cout << "You got it in " << x << " try." << endl;
        }
        // two decimal points
        cout.setf(ios::fixed);
        cout.precision(2);
        cout << "Average: " << (double)total/i << ", minimum: " << min << ", maximum: " << max << endl << endl;
        i++;
    }
}

// calculates number of flowers and bees
int playOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum)
{
    // invaild conditions (which should not be met)
    if (nWords <= 0 || wordnum < 0 || wordnum >= nWords)
    {
        return -1;
    }
    // declare and initialize variable corresponding to number of tries
    int i = 0;
    while (true)
    {
        // initalizes input cstring;
        char trial[1000];
        bool overall = true;
        // loop - if input is invalid, user is repeatedly prompted for a trial word
        while (overall)
        {
            // questions user for word
            cout << "Trial word: ";
            cin.getline(trial, 1000);
            int d = 0;
            bool status = true;
            // loops through word to determine validity
            while (trial[d])
            {
                if (isupper(trial[d]) || strlen(trial) > 6 || strlen(trial) < 4)
                {
                    cout << "Your trial word must be a word of 4 to 6 lower case letters." << endl;
                    status = false;
                    break;
                }
                d++;
            }
            if (status)
            {
                // iterate through words array
                for (int a=0; a<nWords; a++)
                {
                    // if trial is found in word array, break out of loop
                    if (strcmp(words[a], trial) == 0)
                    {
                        overall = false;
                    }
                }
                // if trial not found, message returned
                if (overall)
                {
                    cout << "I don't know that word." << endl;
                }
            }
        }
        // i iterated to account for number of tries
        i++;
        // if guess is equal to mystery word, return number of tries taken
        if (strcmp(trial, words[wordnum]) == 0)
        {
            return i;
        }
        else
        {
            // initalize and declare number of flowers and bees
            int flowers = 0;
            int bees = 0;
            // initialize and declare copy of mystery word
            char wordsCopy[10];
            strcpy(wordsCopy, words[wordnum]);
            
            // iterates through trial
            for (int i=0; i<strlen(trial); i++)
            {
                // if flower found, increment flowers and replace flower char with space
                if (trial[i] == wordsCopy[i])
                {
                    flowers++;
                    wordsCopy[i] = ' ';
                }
                else
                {
                    // if flower not found, go through mystery word to look for bees
                    for (int j=0; j<strlen(wordsCopy); j++)
                    {
                        // if bee is found, increment bee, then replace bee char with space
                        if (trial[i] == wordsCopy[j])
                        {
                            bees++;
                            wordsCopy[j] = ' ';
                            break;
                        }
                    }
                }
            }
            // cout result
            cout << "Flowers: " << flowers << ", " << "Bees: " << bees << endl;
        }
    }
}
