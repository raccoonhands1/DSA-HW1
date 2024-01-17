#include <iostream>
#include <fstream>

using namespace std;


const unsigned int EXPANSION_SIZE = 3; // Global to increase size of array reallocation on introduction of m_cnum > m_cmax

class Student
{   
public:
    Student() { 
        m_sid = 0; m_sname.clear();
    };
    Student(const Student& os) { // Copy constructor - overwrite internal variables
         m_sid = os.m_sid;  m_sname = os.m_sname;
    };
    Student(int id, string name) { // Initializer Constructor - set internal variables
        m_sid = id; m_sname = name;
    };
    ~Student() { // Default Destructor - clear string
        m_sname.clear();
    };
  
    //Getters 
    string getName() { return (m_sname); };
    int getId() { return (m_sid); };

private:
    int m_sid; 
    string m_sname; 
};

// 2. *********** Homework 1.  insert the code for the StudentEsa into the StudentEsa interface below *******

// This is the interface to an "Enhanced" (orderable) Student Array that you will write the routines for.
//  Note:  Transparent array auto-resizing may need to be done.


class StudentEsa
{
private:  
    Student* *sap; // Underlying pointer to array of Student pointers, which you will allocate
    Student* *tp;  // Optionally used in realloc
    int m_cnum; // Number of students currently in array
    int m_cmax; // Current Max size of the array (may need to be expanded)

public: 
    StudentEsa (int ms){
        m_cnum = ms;
        m_cmax = m_cnum;
        sap = new Student*[ms];
        tp = NULL;
        }         //  1. Normal Constructor.  Create ESA of this size.  May need to be resized later

     //  StudentEsa(int ms) { // Allocate Enhanced Student Array capable of holding this many Student Ptrs
     //       sap = new Student* [cmz = ms]; // sap points to an array of Student pointers of size cmz
     //       cnum = 0; tp = NULL; // Initialize with no pointers currently in array
     //  }
       
    StudentEsa () {
        m_cnum = 0; // No students in array
        m_cmax = m_cnum;
        sap = nullptr; // No students so no pointers
        tp = nullptr; // No temp array because no students
        cerr << "Error:  StudentEsa created with no size \n"; // error, assumes that the user did not intend 
        //to create an array with no size
    }
    StudentEsa (StudentEsa& sep){
        m_cnum = sep.m_cnum;
        m_cmax = sep.m_cmax;
        sap = new Student*[m_cmax]; //unsure if this is correct. Assumes that the copy constructor is called when the array is full
        for (int i = 0; i < m_cnum; i++){
            sap[i] = sep.sap[i];
        }
    } 

    ~StudentEsa(){
        delete[] sap; // Delete the array of Student pointers
        delete[] tp; // Delete the array of Temporary student pointers in case they were previously not deleted
    }               

    int getSize(){ cout << m_cnum << "\n"; return m_cnum; }


	// 2. GET:  Get and return the ptr to the Student at the specified Array Index
	//          Error if the supplied index >= number of entries.  Return a NULL ptr
    Student* get(int index){ 
        if (index >= m_cnum || index < 0){
            cerr << "Error:  Index out of bounds \n";
            return nullptr;
        }
        return sap[index];
    }



	// 3. SET: Replace whatever is at the supplied index with the new Student ptr. Return index # on success
	// Error if the supplied index >= number of entries.  Return -1
	// <DO NOT Destruct the Student whose ptr is at the supplied index>
	// 

    int set(Student* nsp, int index) {
        if (index >= m_cnum || index < 0) {
            cerr << "Error: Index out of bounds \n";
            return -1;
        }
        sap[index] = nsp;  // Just set the pointer
        cout << "Student at index " << index << " has been replaced \n";
        return index;
    }

    int append(Student* nsp) {
        if (nsp == nullptr) { return -1; }

        m_cnum++;
        if (m_cnum > m_cmax) { //realloc
            m_cmax += EXPANSION_SIZE;
            tp = new Student* [m_cmax];
            for (int i = 0; i < m_cnum - 1; i++) {
                tp[i] = sap[i];
            }
            delete[] sap;
            sap = tp;
        }
        sap[m_cnum - 1] = nsp;  // Fix the index
        return m_cnum - 1;
    }

    int insert(Student* nsp, int index) {
        if (index > m_cnum || index < 0) {
            cerr << "Error: Index out of bounds " << this << '\n';
            return -1;
        }
        else if (index == m_cnum) {
            return append(nsp);
        }
        else {
            m_cnum++;
            if (m_cnum > m_cmax) { //realloc
                m_cmax += EXPANSION_SIZE;
                tp = new Student* [m_cmax];
                for (int i = 0; i < index; i++) {
                    tp[i] = sap[i];
                }
                tp[index] = nsp;
                for (int i = index + 1; i < m_cnum; i++) {
                    tp[i] = sap[i - 1];
                }
                delete[] sap;
                sap = tp;
            } else {
                for (int i = m_cnum - 1; i > index; i--) {
                    sap[i] = sap[i - 1];
                }
                sap[index] = nsp;
            }
            return index;
        }
    }

    int remove(int index) {
        if (index >= m_cnum || index < 0) {
            cerr << "Error: invalid removal at object " << this << ", index: " << index << '\n';
            return -1;
        } else {
            for (int i = index; i < m_cnum - 1; i++) {
                sap[i] = sap[i + 1];
            }
            m_cnum--;  // decrement 
            return m_cnum;
        }
    }

    int prepend(Student* nsp) {
        if (nsp == nullptr) {
            cerr << "Error: nullptr prepended: " << this << '\n';
            return -1;
        }

        m_cnum++;
        if (m_cnum > m_cmax) { // reallocate
            m_cmax += EXPANSION_SIZE;
            tp = new Student* [m_cmax];
            for (int i = 0; i < m_cnum - 1; i++) {
                tp[i + 1] = sap[i];
            }
            delete[] sap;
            sap = tp;
        } else {
            for (int i = m_cnum - 1; i > 0; i--) {
                sap[i] = sap[i - 1];
            }
        }

        sap[0] = nsp;  // Insert the new student at the beginning
        return 0;  // Success
    }

};


// **************************** End class StudentEsa ******************************

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

    StudentEsa* esa = nullptr;   // Pointer to Enhanced Student Array
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
    //  Store:   S / Index to Store (overwrite) /  new StudentID / new Student Name
    
    // Get Size of Extended Array and # of commands
    // ssize is size of extended array, nops is # of commands

    inp >> ssize >> nops;
    cout << "Read Array size " << ssize << "  Read # commands " << nops << endl;
     
    Student* stud;    // Array to hold pointer of created student.
    esa = new StudentEsa (ssize);  // Small Student Enhanced Array.  May have to be resized.
    
    
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

// **********************************  End of test code  *************************************



