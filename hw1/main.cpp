#include <iostream>
#include <fstream>
#include "main.h"

using namespace std;

int main()
{ // Redirect Input
    string infileName; // Use if Input redirected
    string outfileName; // Use if output redirected
    FILE** input = NULL;  // Recovering Cin


    // Command File Record entries
    int ssize; // Size of extended array
    int nops; // # operations to perform

    char command; // Command (I,A,R,S)
            // Not all of the following are present in each command (Default given)
    int index; // Array Index (-1 default)
    int num;  // Student ID Number (-1 default)
    string name;  // Student Name (XXXX default)

    ExtendedArray<Student>* esa = nullptr;   // Pointer to Enhanced Student Array
    Student sc; // A class to collect Students is generated

    int x=0; // Useful variables
    
    cout << "Use input file:  ";
    cin >> infileName; // Get name of file containing input data
    cout << "Using input file " << infileName << endl << endl;

    // Create an input file stream to read supplied file
    std::ifstream inp;
    inp.open(infileName.c_str());
    if (!inp) {
        cerr << "Error: file " << infileName.c_str()<< "  could not be opened" << endl;
        exit(1);
    }
      
    // First line is array size and # commands to add, every subsequent line is one of:
    //  Append:  A / -1 / StudentID / Student Name
    //  Insert:  I / Index to Insert / StudentID / Student Name
    //  Remove:  R / Index to Remove / -1 / XXXX
    //  Set & store:   S / Index to set (overwrite) /  new StudentID / new Student Name (returns old student pointer)
    
    // Get Size of Extended Array and # of commands
    // ssize is size of extended array, nops is # of commands

    inp >> ssize >> nops;
    cout << "Read Array size " << ssize << "  Read # commands " << nops << endl;
     
    Student* stud;    // Array to hold pointer of created student.
    esa = new ExtendedArray<Student> (ssize);  // Small Student Enhanced Array.  May have to be resized.
    
    
    for (int i = 0; i < nops; i++) {  // Process Commands 
        //**************************************************************
        inp >> command;
        inp >> index;
        inp >> num;
        inp >> name;
        cout << "Command: " << command << "  " << index <<"  " << num << "  " << name << endl;

        // Process each command
        switch (command)
        { // Convert to command for Extended Array
           case 'A':  // Append
               stud = new Student(num, name);
               esa->append(stud);
               break;
           case 'I':  // Insert
               stud = new Student(num, name);
               esa->insert(stud, index);
               break;
           case 'R':  // Remove (delete)
               esa->remove(index);
               break;
           case 'S': // Store over existing Student
               stud = new Student(num, name);
               esa->set(stud, index);
               break;
            case 'P': //Prepend (note:: added)
                stud = new Student(num, name);
                esa->prepend(stud);
           default:
               cout << "Illegal Command:  " << command << endl;
        }
    }
    
    // Print out Current contents of extended array
    cout << "------- \n \n";

    x = esa->getSize(); // Get size of array (number of active elements
    for (int i = 0; i < x; i++) {  // Print each element

        stud = esa->get(i); // Get ptr to ith student in array
        cout << i << "  Student: ID = " << stud->getId() << "  Name = " << stud->getName() << endl;
    
 }
    return (0);
}