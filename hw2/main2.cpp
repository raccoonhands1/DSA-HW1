#include <iostream>
#include <fstream>
#include "main2.h"

using namespace std;

int main()
{ // Redirect Input
    string infileName; // Use if Input redirected
    string outfileName; // Use if output redirected 
    FILE** input = NULL;  // Recovering Cin

    // Command File Record entries
    int ssize; // Size of extended array
    int nops; // # operations to perform

    char command; // Command (G,Z   F,G,H   B,C,D)
            // Not all of the following are present in each command (Default given)
    int num;  // Student ID Number (-1 default)
    string name;  // Student Name (XXXX default)

    StudentDQI* dqi = NULL;   // Pointer to DeQue
    Student sc; // A class to collect Students is generated

    int x = 0; // Useful variables

    cout << "Use input file:  ";
    cin >> infileName; // Get name of file containing input data
    cout << "Using input file " << infileName << endl << endl;

    // Create an input file stream to read supplied file
    std::ifstream inp;
    inp.open(infileName.c_str());
    if (!inp) {
        cerr << "Error: file " << infileName.c_str() << "  could not be opened" << endl;
        exit(1);
    }

    // First line is array size and # commands to add, every subsequent line is one of:
    // 
    // GetSize:      S / -1 / -1
    // isEmpty:      Z / -1 / -1
    // 
    //  PushFront:   F / Student ID / Student Name
    //  PopFront:    G / -1 / -1
    //  LookFront :  H / -1 / -1
    //
    //  PushBack:    B / Student ID / Student Name
    //  PopBack:     C / -1 / -1
    //  LookBack :   D / -1 / -1

    // Get Size of Extended Array and # of commands
    // ssize is size of extended array, nops is # commands

    inp >> ssize >> nops;
    cout << "Read Array size " << ssize << "  Read # commands " << nops << endl;

    Student* stud;    // Array to hold pointer of created student.
    dqi = new StudentDQI(ssize);  // Create Dequeue of specified size.


    for (int i = 0; i < nops; i++) {  // Process Commands 
        //**************************************************************
        inp >> command;
        inp >> num;
        inp >> name;
        cout << "Command: " << command << "  " << num << "  " << name << endl;

        // Process each command
        switch (command)
        { // Convert to command for Extended Array
        case 'S':  // Get Size
            cout << "Size:  " << dqi->getSize() << endl;
            break;

        case 'Z':  // Check if Empty
            cout << "Empty : " << dqi->isEmpty() << endl;
            break;

        case 'F':  // Push Front
            stud = new Student(num, name);
            (void)dqi->pushFront(stud);
            cout << "Pushed Front " << num << "  " << name << endl;
            break;

        case 'G': // Pop Front
            stud = dqi->popFront();
            if (stud) { // Success
                cout << "Pop Front " << stud->getId() << "  " << stud->getName() << endl;
                delete (stud);
            }
            break;

        case 'H':  // Look Front
            stud = dqi->lookFront();
            if (stud) { // Success ... don't delete Student
                cout << "Look Front " << stud->getId() << "  " << stud->getName() << endl;
            }
            break;

        case 'B':  // Push Back
            stud = new Student(num, name);
            (void)dqi->pushBack(stud);
            cout << "Pushed Back " << num << "  " << name << endl;
            break;

        case 'C':  // Pop Back
            stud = dqi->popBack();
            if (stud) { // Success
                cout << "Pop Back " << stud->getId() << "  " << stud->getName() << endl;
                delete (stud);
            }
	    break;

        case 'D':  // Look Back
            stud = dqi->lookBack();
            if (stud) { // Success ... don't delete Student
                cout << "Look Back " << stud->getId() << "  " << stud->getName() << endl;
            }
	    break;

        default:
            cout << "Illegal Command:  " << command << endl;
        }
    }
    // Print out Current contents of extended array
    x = dqi->getSize(); // Get number of array elements
    cout << "------------------" << endl << "Start Popping from bottom  " << x << "  Elements queued" << endl << endl;

    while (stud = dqi->popBack())
    {
            cout << "Student: ID = " << stud->getId() << "  Name = " << stud->getName() << endl;
    }
    return (0);
};