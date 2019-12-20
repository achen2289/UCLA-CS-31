//
//  logic_error.cpp
//  1.2
//
//  Created by ALEX CHEN (: on 10/3/19.
//  Copyright © 2019 cs. All rights reserved.
//

// Code for Project 1
// Report poll results

#include <iostream>
using namespace std;

int main()
{
    int numberSurveyed;
    int forImpeachment;
    int antiImpeachment;

    cout << "How many people were surveyed? ";
    cin >> numberSurveyed;
    cout << "How many of them support impeachment of the president? ";
    cin >> forImpeachment;
    cout << "How many of them oppose impeachment of the president? ";
    cin >> antiImpeachment;

    double pctFor = 100.0 * forImpeachment / numberSurveyed;
    double pctAnti = 100.0 * antiImpeachment / numberSurveyed;

    cout.setf(ios::fixed);
    cout.precision(1);

    cout << endl;
    cout << pctFor << "% say they support impeachment." << endl;
    cout << pctAnti << "% say they oppose impeachment." << endl;

    if (forImpeachment < antiImpeachment)
        cout << "More people support impeachment than oppose it." << endl;
    else
        cout << "More people oppose impeachment than support it." << endl;
}
