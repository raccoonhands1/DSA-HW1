#include <iostream>
#include <fstream>

using namespace std;

class Student
{
public:
    Student()
    {
        m_sid = 0;
        m_sname.clear();
    };
    Student(const Student &os)
    { // Copy constructor - overwrite internal variables
        m_sid = os.m_sid;
        m_sname = os.m_sname;
    };
    Student(int id, string name)
    { // Initializer Constructor - set internal variables
        m_sid = id;
        m_sname = name;
    };
    ~Student()
    { // Default Destructor - clear string
        m_sname.clear();
    };

    // Getters
    string getName() { return (m_sname); };
    int getId() { return (m_sid); };

private:
    int m_sid;
    string m_sname;
};

class StudentEsa
{

private:           // Define whatever additional private variables you need
    Student **sap; // Underlying pointer to array of Student pointers, which you will allocate
    Student **tp;  // Optionally used in realloc
    int cnum;      // Number of students currently in array
    int cmz;       // Current Max size of the array (may need to be expanded)

public: // Publically supported methods YOU NEED TO IMPLEMENT.  Replace each declaration with complete code
        // ****************  Constructors / Destructor
    //  1. Normal Constructor.  Create ESA of this size.  May need to be resized later
    StudentEsa(int ms)
    {

        cmz = ms;

        cnum = 0;

        sap = new Student *[cmz];

        tp = NULL;
    }

    StudentEsa()
    {

        cmz = 50;

        cnum = 0;

        sap = new Student *[cmz];

        tp = NULL;

    } // 2. Default Constructor <Comment out, or use some predefined size, and EXPLAIN YOUR CHOICE>

    // // 3. Copy Constructor  (what gets copied and what does not?)
    StudentEsa(StudentEsa &sep)
    {
        // sap = new Student*[sep.getSize()]; // get cmz, not the cnum
        sap = new Student *[sep.cmz]; // get cmz, not the cnum

        // copy everything over
        cmz = sep.cmz;

        tp = sep.tp;

        // copy the array over
        for (int i = 0; i < sep.cnum; i++)
        {

            sap[i] = sep.sap[i];
        }
    }

    ~StudentEsa()
    {

        cnum = 0;

        cmz = 0;

        delete sap;
        delete tp;

    } // 4. Default destructor  (what gets freed up and what does not?)

    // ******************* Remaining public functions of the Extended Student Array

    // 1. Return the number of Students in the Collection
    int getSize()
    {
        return cnum;
    }

    bool isEmpty()
    {

        if (cnum == 0)
        {

            return true;
        }

        else
            return false;
    }

    // 2. GET:  Get and return the ptr to the Student at the specified Array Index
    //          Error if the supplied index >= number of entries.  Return a NULL ptr
    Student *get(int index)
    {

        // cout<<"START"<<endl;
        // // print the whole thingg
        // for(int i = 0; i < cnum; i ++) {
        //     cout<<"i "<<i<<", soa[i] "<<sap[i]<<endl;
        // }
        // cout<<"END"<<endl;

        if (index < 0)
        {

            return NULL;
        }
        else if (index > cnum)
        {
            return NULL;
        }

        return sap[index];
    }

    // 3. SET: Replace whatever is at the supplied index with the new Student ptr. Return index # on success
    // Error if the supplied index >= number of entries.  Return -1
    // <DO NOT Destruct the Student whose ptr is at the supplied index>
    //
    int set(Student *newStudent, int index)
    {

        if (index < 0)
        {

            return -1;
        }
        else if (index >= cnum)
        {
            return -1;
        }

        sap[index] = newStudent;

        return index;
    }

    // 4. INSERT: Insert Student ptr at the supplied index, by first "pushing back" every subsequent element
    //    Error if the supplied index > number of entries.  Return -1
    //    OK if supplied index == number of entries (equivalent to an append)
    //    Note:  ESA size increases which may force a reallocation of the array.  Return index # on success
    int insert(Student *nsp, int index)
    {

        // if index > number of entries
        if (index > sizeof(sap))
        {

            return -1;
        }

        else if (index < 0)
        {

            return -1;
        }

        else
        {

            if (cnum + 1 >= cmz)
            {

                // create new array
                cmz += 50;

                Student **temp = new Student *[cmz];

                // initialize the array up to index
                for (int i = 0; i < index; i++)
                {

                    temp[i] = sap[i];
                }

                // index = nsp
                temp[index] = nsp;

                cnum++;

                // initialize the rest of the array
                for (int j = index; j < cnum; j++)
                {

                    temp[j + 1] = sap[j];
                }
                sap = temp;
            }

            else
            {

                // bounce each element of array up one index, from back to front
                for (int i = cnum; i > index; i--)
                {

                    sap[i + 1] = sap[i];
                }
                // insert element at index
                sap[index] = nsp;

                cnum++;
            }
        }
        return 1;
    }

    // 5. REMOVE: Delete specified array element by "pulling forward" every subsequent element
    //    Error if the supplied index >= number of entries.  Return -1.  Else decrement ESA size and return it
    //    Note: <DO NOT Destruct the Student whose ptr is at the supplied index>
    int remove(int index)
    {

        if (index < 0)
        {

            return -1;
        }

        else if (index >= cnum)
        {

            return -1;
        }

        for (int i = index; i < cnum; i++)
        {

            sap[i] = sap[i + 1];
        }
        cnum--;
        return 1;
    }

    // 6. APPEND:  Append the ptr to the supplied Student to back of the ESA, bump ESA size
    //    Return index # of new entry on success, -1 on failure
    //    Note:  This may force a reallocation of the array.
    int append(Student *nsp)
    {

        insert(nsp, cnum - 1);

        return (cnum - 1);
    }

    // 7. PREPEND: Prepend the ptr to the supplied Student to the front of the ESA, increment ESA size
    //    Return 0 on success, -1 on failure
    //    Note:  This may force a reallocation of the array.
    int prepend(Student *nsp)
    {

        insert(nsp, 0);
        return 0;
    }
};

// class "StudentDQI   <Homework #2 - wraps (embeds) Extended Student Array (ESA) so it is invisible to external users of this class

class StudentDQI
{
private:
    StudentEsa *soa; // Allocated Extended Student Array
    // These might be useful if your solution is for extra credit
    // and positions the DQ in the MIDDLE of the Extended Array
    unsigned int top; // Index value 1 above highest OA element used
    unsigned int btm; // Index value 1 below lowest OA element used
public:
    StudentDQI(unsigned int i)
    {
        btm = 0;
        soa = new StudentEsa(i);
        top = 0;
    }

    // Create a DQ with this initial size
    StudentDQI(StudentDQI &s)
    {
        btm = s.btm;
        top = s.top;

        soa = new StudentEsa(*s.soa);

    } // Equate this to an existing DQ (can use
    // private parts directly)
    ~StudentDQI()
    {
        top = 0;
        btm = 0;
        delete soa;
    } // Destruct the DQI (free OA space)
    int getSize()
    {
        return soa->getSize();
    } // Gets # elements (Student *) in the DQ
    bool isEmpty()
    {
        return soa->isEmpty();
    } // True if no elements held (âpopâ will fail)
    int pushFront(Student *s)
    {
        top++;
        soa->insert(s, top);
        return top;
        // soa[top] = s;
        // top ++;
    } // Prepend a new element to the front


    Student *popFront() { // make a copy of top
        if (this->isEmpty())
        {
            cout << "DQ is empty \n";
            return nullptr;
        }

        Student *copyOfFront = soa->get(top); // set top to null
        soa->remove(top);                     // return top
        top--;
        return copyOfFront;
    } // Remove the first element and return it



    Student *lookFront()
    {
        return soa->get(top);
    } // Return the first element but do not remove it
    int pushBack(Student *s)
    {
        // Student studentObject = *s;
        soa->prepend(s);

        return 1;
    } // Append a new element to the back
    Student *popBack()
    {
        // make a copy of btm
        Student *copyOfBottom = soa->get(btm);

        soa->remove(btm);
        // return bottom
        return copyOfBottom;
    } // Remove the last element and return it
    Student *lookBack()
    {
        if(this->isEmpty()){ return nullptr; }

        return soa->get(btm);
    } // Return the last element but do not remove
    // it
};