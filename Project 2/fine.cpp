//
//  fine.cpp
//  Project 2
//
//  Created by ALEX CHEN (: on 10/11/19.
//  Copyright © 2019 cs. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

// calculates fee based on amount paid and whether or not athletic achievement was faked
double feeAmount(double amtPaid, string fakeAthlete)
{
    double fine = 0;
    double amtPaidInt = amtPaid;
    // uses conditionals to calculate fees based on amount paid
    if (amtPaidInt > 250)
    {
        fine = fine + (amtPaidInt-250)*.14;
        amtPaidInt = 250;
    }
    if (amtPaidInt > 40)
    {
        if (fakeAthlete == "y")
        {
            fine = fine + (amtPaidInt-40)*.22;
            amtPaidInt = 40;
        }
        else if (fakeAthlete == "n")
        {
            fine = fine + (amtPaidInt-40)*.10;
            amtPaidInt = 40;
        }
    }
    fine = fine + (amtPaidInt)*.66;
    fine += 20;
    return fine;
}

// main method to return output sentence
int main()
{
    cout<<"Defendant: ";
    string defendant;
    getline(cin, defendant);
    // if input is empty string, error message is displayed
    if (defendant == "")
    {
        cout<<"---"<<endl;
        cout<<"You must enter a defendant name."<<endl;
        return 1;
    }
    cout<<"Amount paid (in thousands): ";
    double fee;
    cin>>fee;
    // if fee is negative, error message is displayed
    if (fee<0)
    {
        cout<<"---"<<endl;
        cout<<"The amount paid must not be negative."<<endl;
        return 1;
    }
    // ignore the newline that is present in cin
    cin.ignore(10000, '\n');
    cout<<"Fake athelete? (y/n): ";
    string fakeAthlete;
    getline(cin, fakeAthlete);
    // if fakeAthlete contains anything other than a "y" or "n", an error message is displayed
    if (fakeAthlete != "y")
    {
        if (fakeAthlete != "n")
        {
            cout<<"---"<<endl;
            cout<<"You must enter y or n."<<endl;
            return 1;
        }
    }
    cout<<"---"<<endl;
    // call to function that calculates total fees to be paid
    double totalFee = feeAmount(fee, fakeAthlete);
    // sets maximum number of decimal points to be 1
    cout.setf(ios::fixed);
    cout.precision(1);
    cout<<"The suggested fine for "<<defendant<<" is $"<<totalFee<<" thousand."<<endl;
}
