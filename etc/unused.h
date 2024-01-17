#include <iostream>
#include <fstream>

using namespace std;

const unsigned int EXPANSION_SIZE = 10; // var to increase size of array reallocation when m_cnum > m_cmax

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


class StudentEsa
{
private:  
    Student* *sap; // Underlying pointer to array of Student pointers, which you will allocate
    Student* *tp;  // Optionally used in realloc
    int m_cnum; // Number of students currently in array
    int m_cmax; // Current Max size of the array (may need to be expanded)

public: 
    StudentEsa (int ms){
        m_cnum = 0;
        m_cmax = ms;
        sap = new Student*[ms];
        tp = nullptr;
    }
       
    StudentEsa () {
        m_cnum = 0; // No students in array
        m_cmax = 0;
        sap = nullptr; // No students so no pointers
        tp = nullptr; // No temp array because no students
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

    Student* get(int index) const { 
        if (index >= m_cnum || index < 0) {
            cerr << "Error: Index out of bounds \n";
            return nullptr;
        }
        return sap[index];
    }

    bool isEmpty(){ return m_cnum == 0; }

    void reallocateArray() { //use tp to add EXPANSION_SIZE (global variable) into sap
        m_cmax += EXPANSION_SIZE; 
        tp = new Student* [m_cmax];
        for(int i=0; i < m_cnum-1; i++){ 
            tp[i] = sap[i];
        }
        sap = tp;
    }

    Student* set(Student* nsp, int index) {  //"set" is also known as "store", so return a pointer to the student nsp replaced. 
        if (index >= m_cnum || index < 0) { cerr << "Error: Index out of bounds \n"; return nullptr; }
        if(nsp == nullptr) { cerr << "Error: invalid pointer \n"; return nullptr; }

        //success!
        Student* tmpptr = sap[index];

        sap[index] = nsp;  // replace osp with nsp
        cout << "Student at index " << index << " has been replaced, " << tmpptr->getName() << "\n";

        return tmpptr;
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
            sap = tp;
        }
        sap[m_cnum - 1] = nsp;  // Fix the index

        cout << "appended " << nsp->getName() << " position: " << m_cnum-1 << "\n"; 

        return m_cnum - 1;
    }

    int insert(Student* nsp, int index) { 
        if(nsp==nullptr || index < 0){ cerr << "error on insert \n"; return -1; } //failure case
        if(m_cnum == 0) {append(nsp); return 0;}

        m_cnum++; //success!
        if(index > m_cnum){ //if it's more than the current size
            if(index >= m_cmax){ //if it's more than the max size
                reallocateArray();
            }
            m_cnum--; //subtract one because append already adds one. Creating an optional parameter in append() for this would be unnecessary and less readable
            append(nsp); 
            cout << "inserted " << nsp->getName() << " at index " << index <<" \n";
            return 0;
        } else if(m_cnum > m_cmax){ //resize (normal case) and continue
            reallocateArray();
        }

        for(int i = m_cnum - 1; i > index; i--){ //pulls forward
            sap[i] = sap[i-1];
        }

        sap[index] = nsp;
        cout << "inserted " << nsp->getName() << " at index " << index <<" \n";
        return index;
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
        if (nsp == nullptr) { cerr << "Error: nullptr prepended: " << this << '\n'; return -1; } //nullptr case

        insert(nsp, 0);
        return 0;  
    }
};