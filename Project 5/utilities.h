//
//  utilities.h
//  Project_5
//
//  Created by Alex Chen on 11/14/19.
//  Copyright © 2019 cs. All rights reserved.
//

#ifndef utilities_h
#define utilities_h

#include <stdio.h>

const int MINWORDLEN = 4;
const int MAXWORDLEN = 6;

int getWords(char words[][MAXWORDLEN+1], int maxWords, const char wordfilename[]);
int randInt(int min, int max);

#endif /* utilities_h */
