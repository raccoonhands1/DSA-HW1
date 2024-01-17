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


template <class T> class ExtendedArray {
private:  
    T* *ap; // Underlying pointer to array of T pointers, which you will allocate
    T* *tp;  // Optionally used in realloc
    int m_cnum; // Number of Ts currently in array
    int m_cmax; // Current Max size of the array (may need to be expanded)

public: 
    ExtendedArray (int ms){
        m_cnum = 0;
        m_cmax = ms;
        ap = new T*[ms];
        tp = nullptr;
    }
       
    ExtendedArray () {
        m_cnum = 0; // No Ts in array
        m_cmax = 0;
        ap = nullptr; // No Ts so no pointers
        tp = nullptr; // No temp array because no Ts
    }

    ExtendedArray (ExtendedArray& ep){
        m_cnum = ep.m_cnum;
        m_cmax = ep.m_cmax;
        ap = new T*[m_cmax]; //unsure if this is correct. Assumes that the copy constructor is called when the array is full
        for (int i = 0; i < m_cnum; i++){
            ap[i] = ep.ap[i];
        }
    } 

    ~ExtendedArray(){
        delete[] ap; // Delete the array of T pointers
        delete[] tp; // Delete the array of Temporary T pointers in case they were previously not deleted
    }               

    int getSize(){ cout << m_cnum << "\n"; return m_cnum; } //t

    T* get(int index) const { 
        if (index >= m_cnum || index < 0) {
            cerr << "Error: Index out of bounds \n";
            return nullptr;
        }
        return ap[index];
    }

    bool isEmpty(){ return m_cnum == 0; }

    void reallocateArray(int target = 0) { //use tp to add EXPANSION_SIZE (global variable) into ap
        m_cmax += EXPANSION_SIZE; 
        tp = new T* [m_cmax];
        for(target; target < m_cnum-1; target++){ 
            tp[target] = ap[target];
        }
        ap = tp;
    }

    //do not destruct
    pair<int, T*> set(T* np, int index) {  //"set" is also known as "store", so return a pointer to the T np replaced. 
        if (index >= m_cnum || index < 0) { cerr << "Error: Index out of bounds \n"; return make_pair(-1, nullptr); }
        if(np == nullptr) { cerr << "Error: invalid pointer \n"; return make_pair(-1, nullptr); }

        //success!
        T* tmpptr = ap[index];

        ap[index] = np;  // replace osp with np
        cout << "T at index " << index << " has been replaced, " << tmpptr->getName() << "\n";

        return make_pair(index, tmpptr); //return the index and also the pointer to the student that was lost
    }

    int append(T* np) {
        if (np == nullptr) { return -1; }
        m_cnum++;
        if (m_cnum > m_cmax) { //realloc
            m_cmax += EXPANSION_SIZE;
            tp = new T* [m_cmax];
            for (int i = 0; i < m_cnum - 1; i++) {
                tp[i] = ap[i];
            }
            ap = tp;
        }
        ap[m_cnum - 1] = np;  // Fix the index

        cout << "appended " << np->getName() << " position: " << m_cnum-1 << "\n"; 

        return m_cnum - 1;
    }

    int insert(T* np, int index) { 
        if(np==nullptr || index < 0){ cerr << "error on insert \n"; return -1; } //failure case
        if(m_cnum == 0) {append(np); return 0;}

        m_cnum++; //success!
        if(index > m_cnum){ //if it's more than the current size
            if(index >= m_cmax){ //if it's more than the max size
                reallocateArray();
            }
            m_cnum--; //subtract one because append already adds one. Creating an optional parameter in append() for this would be unnecessary and less readable
            append(np); 
            cout << "inserted " << np->getName() << " at index " << index <<" \n";
            return 0;
        } else if(m_cnum > m_cmax){ //resize (normal case) and continue
            reallocateArray();
        }

        for(int i = m_cnum - 1; i > index; i--){ //pulls forward
            ap[i] = ap[i-1];
        }

        ap[index] = np;
        cout << "inserted " << np->getName() << " at index " << index <<" \n";
        return index;
    }

    pair<int, T*> remove(int index) { //do not destruct
        if (index >= m_cnum || index < 0) {
            cerr << "Error: invalid removal at object " << this << ", index: " << index << '\n';
            return make_pair(-1, nullptr);
        } else {
            for (int i = index; i < m_cnum - 1; i++) {
                ap[i] = ap[i + 1];
            }
            m_cnum--;  // decrement 
            return make_pair(m_cnum, ap[index]);
        }
    }

    int prepend(T* np) {
        if (np == nullptr) { cerr << "Error: nullptr prepended: " << this << '\n'; return -1; } //nullptr case

        insert(np, 0);
        return 0;  
    }
};