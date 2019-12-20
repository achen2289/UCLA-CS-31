//
//  array.cpp
//  Project 4
//
//  Created by ALEX CHEN (: on 11/5/19.
//  Copyright © 2019 cs. All rights reserved.
//

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int countRuns(const string a[], int n);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int separate(string a[], int n, string separator);

int main()
{
    string h[7] = { "mick", "marie", "fiona", "rudy", "", "gordon", "lindsey" };
    assert(lookup(h, 7, "gordon") == 5);
    assert(lookup(h, 7, "fiona") == 2);
    assert(lookup(h, 2, "fiona") == -1);
    assert(positionOfMax(h, 7) == 3);

    string g[4] = { "mick", "marie", "lindsey", "nancy" };
    assert(differ(h, 4, g, 4) == 2);
    assert(appendToAll(g, 4, "?") == 4 && g[0] == "mick?" && g[3] == "nancy?");
    assert(rotateLeft(g, 4, 1) == 1 && g[1] == "lindsey?" && g[3] == "marie?");

    string e[4] = { "fiona", "rudy", "", "gordon" };
    assert(subsequence(h, 7, e, 4) == 2);

    string d[5] = { "marie", "marie", "marie", "rudy", "rudy" };
    assert(countRuns(d, 5) == 2);

    string f[3] = { "lindsey", "fiona", "mike" };
    assert(lookupAny(h, 7, f, 3) == 2);
    assert(flip(f, 3) == 3 && f[0] == "mike" && f[2] == "lindsey");

    assert(separate(h, 7, "lindsey") == 3);

    cout << "All tests succeeded" << endl;
    
    string people[5] = { "donald", "lindsey", "fiona", "rudy", "mick" };
    int j = appendToAll(people, 5, "!!!");  // returns 5
    cout << "test1" << endl;
    cout << j << endl;
    for (int i=0; i<5; i++)
    {
        cout << people[i] << endl;
    }
    string officeholders[5] = { "ada", "lindsey", "mike", "adas", "nancy" };
    int k = lookup(officeholders, 5, "adam");  // returns 3
    cout << "test2" << endl;
    cout << k << endl;
    string persons[6] = { "rudy", "lindsey", "marie", "rudy", "fiona", "adam" };
    int l = positionOfMax(persons, 6);
    cout << "test3" << endl;
    cout << l << endl;
    string politician[5] = { "mike", "donald", "lindsey", "nancy", "adam" };
    int m = rotateLeft(politician, 4, 4);
    cout << "test4" << endl;
    cout << m << endl;
    for (int i=0; i<5; i++)
    {
        cout << politician[i] << endl;
    }
    string hi[9] = {
        "rudy", "adam", "mike", "mike", "fiona", "fiona", "fiona", "mike", "mike"
    };
    int p = countRuns(hi, 5);  //  returns 5
    cout << "test5" << endl;
    cout << p << endl;
    string folks[7] = { "adam", "", "fiona", "mike", "rudy", "nancy", "donald" };
    int q = flip(folks, 7);
    cout << "test6" << endl;
    for (int i=0; i<7; i++)
    {
        cout << folks[i] << endl;
    }
    cout << q << endl;
    string folks2[7] = { "adam", "", "fiona", "mike", "rudy", "nancy", "donald" };
    string group[6] = { "adam", "", "fiona", "donald", "mike", "rudy" };
    int r = differ(folks2, 7, group, 6);  //  returns 3
    int s = differ(folks2, 2, group, 3);
    cout << "test7" << endl;
    cout << r << " " << s << endl;
    string names[10] = { "marie", "nancy", "marie", "nancy", "lindsey", "gordon" };
    string names1[10] = { "marie", "nancy", "mick" };
    int t = subsequence(names, 6, names1, 2);  // returns 1
    string names2[10] = { "gordon", "mick" };
    int u = subsequence(names, 5, names2, 2);  // returns -1
    cout << "test8" << endl;
    cout << t << " " << u << endl;
    string namesx[10] = { "gordon", "marie", "nancy", "mick", "adam", "lindsey" };
    string set1[10] = { "donald", "adam", "mick", "marie" };
    int v = lookupAny(namesx, 6, set1, 4);  // returns 1 (a1 has "marie" there)
    string set2[10] = { "rudy", "fiona" };
    int w = lookupAny(namesx, 6, set2, 2);
    cout << "test9" << endl;
    cout << v << " " << w << endl;
    string persons5[6] = { "alex", " ", "gordon", "rudy", "fiona", "gordon" };
    int x = separate(persons5, 6, "gordon");
    cout << "test10" << endl;
    cout << x << endl;
    for (int i=0; i<6; i++)
    {
        cout << persons5[i] << endl;
    }
}

// appends value to first n elements of a
int appendToAll(string a[], int n, string value)
{
    if (n < 0)
    {
        return -1;
    }
    // iterates through a, concatenating elements with value
    for (int i=0; i<n; i++)
    {
        a[i] += value;
    }
    return n;
}

// finds first occurence of target in first n elements of a
int lookup(const string a[], int n, string target)
{
    if (n <= 0)
    {
        return -1;
    }
    // iterates through a, comparing elements to target
    for (int i=0; i<n; i++)
    {
        if (a[i] == target)
        {
            return i;
        }
    }
    return -1;
}

// returns first position of string in a, such that string is > all other strings
int positionOfMax(const string a[], int n)
{
    if (n <= 0)
    {
        return -1;
    }
    int pos = n-1;
    string str = a[n-1];
    // iterates through a, starting from last element
    for (int i=pos; i>=0; i--)
    {
        if (a[i] >= str)
        {
            // reassigns pos and str whenever condition is met
            pos = i;
            str = a[i];
        }
    }
    return pos;
}

// eliminates element at pos, then shifts each following element one pos to right
// places element at pos at the last shifted element
int rotateLeft(string a[], int n, int pos)
{
    if (n < 0 || pos < 0 || pos > n-1)
    {
        return -1;
    }
    if (pos == n-1)
    {
        return pos;
    }
    string temp = a[pos];
    // iterates through elements starting at position pos, ending at n-1
    for (int i=pos; i<n-1; i++)
    {
        // shifts elements left
        a[i] = a[i+1];
    }
    // swaps element at initial pos with last element
    a[n-1] = temp;
    return pos;
}

// return the number of sequences of one or more consecutive identical items in a.
int countRuns(const string a[], int n)
{
    // n < 0 is invalid argument
    if (n < 0)
    {
        return -1;
    }
    // if n = 0, there are no consecutive sequences
    if (n == 0)
    {
        return 0;
    }
    // initializes count of consecutive blocks to one and
    // initial string to first element
    string str = a[0];
    int count = 1;
    // iterates from second to last element, adding one to count
    // if proceeding elements are different
    for (int i=1; i<n; i++)
    {
        if (a[i] != str)
        {
            count ++;
            str = a[i];
        }
    }
    return count;
}

// reverse the order of the elements of the array and return n
int flip(string a[], int n)
{
    if (n < 0)
    {
        return -1;
    }
    // starts with last element
    int end = n-1;
    // iterates forward to mid point, doing a temp switch with
    // elements iterating backwards from last element
    for (int i=0; i<n/2; i++)
    {
        string temp = a[i];
        a[i] = a[end];
        a[end] = temp;
        end --;
    }
    return n;
}

// return the position of the first corresponding elements of a1 and a2 that are not equal
int differ(const string a1[], int n1, const string a2[], int n2)
{
    // only iterates through the lower n value number of ints
    if (n1 < 0 || n2 < 0)
    {
        return -1;
    }
    if (n1 < n2)
    {
        // iterates up to n1, stops at n1 or when element at index i
        // in a1 and a2 are different
        for (int i=0; i<n1; i++)
        {
            if (a1[i] != a2[i])
            {
                return i;
            }
        }
        return n1;
    }
    else
    {
        // same as above but i iterates through n2
        for (int i=0; i<n2; i++)
        {
            if (a1[i] != a2[i])
            {
                return i;
            }
        }
        return n2;
    }
}

// return position of element in a1, after which following elements are identical to a2 elements
int subsequence(const string a1[], int n1, const string a2[], int n2)
{
    if (n2 > n1)
    {
        return -1;
    }
    if (n1 < 0 || n2 < 0)
    {
        return -1;
    }
    // if n2 is 0, then it is a subsequence of any sequence
    if (n2 == 0)
    {
        return 0;
    }
    // if n2 is not equal to 0, but n1 is, then n2 is not a subsequence
    if (n1 == 0)
    {
        return -1;
    }
    if (n2 > n1)
    {
        return -1;
    }
    int i=0;
    // iterates through a1, looking for matches to elements of a2
    for (int j=0; j<n1; j ++)
    {
        // if element in a1 is equal to a2, i is incremented
        if (a1[j] == a2[i])
        {
            i ++;
        }
        // if element in a1 is not equal to a2, i restarts
        else
        {
            i = 0;
        }
        // if i is incremented a number of times equal to n2, then there is success
        if (i == n2)
        {
            return j - n2 + 1;
        }
    }
    return -1;
}

// return the smallest position in a1 of an element that is equal to any of the n2 elements in a2
int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
    // if n1 or n2 are 0, fail
    if (n1 < 0 || n2 < 0)
    {
        return -1;
    }
    // if n2 is 0, then this matches any string
    if (n2 == 0)
    {
        return 0;
    }
    // if n1 is 0 but n2 is not, then there is no element that is equal
    if (n1 == 0)
    {
        return -1;
    }
    // iterates through a1 to ensure lowest index of a1 element is found
    for (int a=0; a<n1; a++)
    {
        // iterates through a2
        for (int b=0; b<n2; b++)
        {
            if (a1[a] == a2[b])
            {
                return a;
            }
        }
    }
    return -1;
}

// reorders array into increasing order of strings
// returns element of array that is larger or equal to separator
// returns n if none are larger or equal to separator
int separate(string a[], int n, string separator)
{
    if (n < 0)
    {
        return -1;
    }
    // if no interesting elements, then no elements can be >= separator
    if (n == 0)
    {
        return n;
    }
    // iterates through elements of array, comparing to further elements in array
    for (int i=0; i<n-1; i++)
    {
        for (int j=i+1; j<n; j++)
        {
            // if further element is smaller than sooner element, swap elements
            if (a[i] >= a[j])
            {
                string temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
    }
    // iterates through elements of array, returning index
    // of element that is >= separator
    for (int i=0; i<n; i++)
    {
        if (!(a[i] < separator))
        {
            return i;
        }
    }
    return n;
}
