#include <iostream>
#include <fstream>

using namespace std;

unsigned int EXPANSION_SIZE = 10; // var to increase size of array reallocation when m_cnum > m_cmax
// note for this var:: in hw2, expansionsize is meant to be an even number. I would include odd numbers, but it would mean altering the realloc code 
// for the student template which isn't the point of the assignment. This way is simpler and still works, but if i were implementing it for a product
// then I would include proper handling of odd/even values. An expansion size of 10 ensures 5 empty elements on each size of the deque.
// I don't want to sacrifice readability for a niche precision.

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
    int getMax(){ return m_cmax; }
    int getCurr() { return m_cnum; }
    void setCurr(int x) { m_cnum = x; }
    ExtendedArray (const unsigned ms){
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
        if (index >= m_cmax || index < 0) {
            cerr << "Error: Index out of bounds \n";
            return nullptr;
        }
        return ap[index];
    }

    bool isEmpty(){ return m_cnum == 0; }

    void reallocateArray(int target = 0) { //use tp to add EXPANSION_SIZE (global variable) into ap. start from target. 
        m_cmax += EXPANSION_SIZE; 
        tp = new T* [m_cmax];
        for(target; target < target + m_cnum-1; target++){ 
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
        if (index < 0) {
            cerr << "Error: invalid removal at object " << this << ", index: " << index << '\n';
            return make_pair(-1, nullptr);
        } else if (index >= m_cnum){ //if the index is too large, remove the last element
            m_cnum--;
            ap[index] = nullptr;
            return make_pair(m_cnum+1, ap[m_cnum]);
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

    int hardSet(T* np, int index){
        ap[index] = np;
        return index;
    }
};

// class "StudentDQI   <Homework #2 - wraps (embeds) Extended Student Array (ESA) so it is invisible to external users of this class

class StudentDQI {
private:
    ExtendedArray<Student>* soa;  // Allocated Extended Student Array 

    unsigned int m_top; // Index value 1 above highest OA element used
    unsigned int m_btm; // Index value 1 below lowest OA element used

public:
    StudentDQI(unsigned int is){ //initial size ms
        soa = new ExtendedArray<Student>(is);
        m_top = is/2; //indexes of half of initial size
        m_btm = is/2;
    } 
    
    StudentDQI(StudentDQI& ms); // Equate this to an existing DQ (can use private parts directly)
    ~StudentDQI(){ delete soa; }

    int getSize(){ return soa->getSize(); }  // Gets # elements (Student *) in the DQ
    bool isEmpty() { return soa->isEmpty(); } // True if no elements held (“pop” will fail)

    void reallocateQ(){
        if(EXPANSION_SIZE % 2 != 0) { EXPANSION_SIZE++; } //if its odd, add one for simplicity

        soa->reallocateArray(EXPANSION_SIZE/2 + m_btm); // take the size of expansion/2 and add bottom to get index to copy to
        m_btm += EXPANSION_SIZE/2;
        m_top += EXPANSION_SIZE/2 - 1;
    }

    int pushFront(Student* stuptr){
        if(stuptr == nullptr){ cerr << "Error: invalid pointer at pushfront \n"; return -1; }

        //success!
        m_top++;
        if(m_top > soa->getMax()){ 
            reallocateQ(); 
        }

        return soa->append(stuptr);
    } // append a new element to the front



//
    Student* popFront(){ //revisit
        if(this->isEmpty()){ cerr << "Error: cannot pop because size is 0 \n"; return nullptr; }

        //success!
        m_top--;
        return soa->remove(m_top+1).second; //get rid of rhs and return ptr in pair
    }      
//




    int pushBack(Student* stuptr){ //must realloc
        if(m_btm < 0){
            reallocateQ();
        }
        m_btm--;
        soa->hardSet(stuptr, m_btm);
        return m_btm;
    } // Append a new element to the back





    Student* popBack(){ //realloc
        if(this->isEmpty()){ cerr << "Error: cannot pop because size is 0 \n"; return nullptr; }

        //success!
        m_btm++;

        return soa->remove(m_btm - 1).second; //get rid of rhs and return ptr in pair
    }     // Remove the first element and return it



    Student* lookFront(){
        if(this->isEmpty()){ cerr << "Error: cannot look because size is 0 \n"; return nullptr; }
        return soa->get(m_top);
    }

    Student* lookBack(){
        if(this->isEmpty()){ cerr << "Error: cannot look because size is 0 \n"; return nullptr; }
        return soa->get(m_btm);
    }     // Return the last element but do not remove it
};

//back is the left, front is the right