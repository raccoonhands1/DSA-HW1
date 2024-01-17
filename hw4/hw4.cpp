using namespace std;
#include <iostream>
#include <fstream>

/*  The following code file is divided into 5 sections

	1. Interface and code for Class Student
	2. Elements of Structure studNode (Heap Node with registration priority and ptr to a Student with that priority)
	3. Interface and code for class SnEsa  (Extended Array. HM1 code here modified to collect studNode struct ptrs)
	4. Interface for class StudMinHeap  (**** HM4 requires writing the code to implement parts of this interface ****)
	5. HM4 test program code

 
*/


// 1.  ************************ class Student  ***************************

class Student
{   // A VERY simple Student consisting only of the student's ID and Name
    // Both the interface and the code will be located here.
   
private:
    int sid; // Student ID
    string sname; // Full Name (Ex: KleinmanRon)

public:
    Student() { // Default constructor
        sid = 0; sname.clear();
    };
    Student(const Student& os) { // Copy constructor - overwrite internal variables
         sid = os.sid;  sname = os.sname;
    };
    Student(int id, string name) { // Initializer Constructor - set internal variables
        sid = id; sname = name;
    };
    ~Student() { // Default Destructor - clear string
        sname.clear();
    };
  

    //Getters 
    string getName() { return (sname); };
    int getId() { return (sid); };
};

//      ********************** End class Student ***************************************

//  2. ********************** Struct studNode ******************************

// Internal of Heap Collection support
// It is studNodes which are collected by a specialized ESA (which knows nothing of the studNode components

struct studNode {    // NodeElement being “heaped”
   unsigned int priority;    // Priority of Student
   Student* studp;           // Ptr to Student (if Null, studNode is NULL)

			// Constructor  for studNode Structure:
   studNode () {priority = 0; studp = NULL;}
   studNode (unsigned int pria, Student* spa) : priority (pria), studp (spa) {}
};

// ********************** End Struct studNode ******************************

// 3.   <StudentEsa code modified to collect studNode STRUCTURES rather than Student PTRS>

class SnEsa
{
private:  // Define whatever additional private variables you need
      studNode* snap; // Underlying pointer to array of Student Heap Nodes Ptrs, which you will allocate
      studNode* tp;  // Optionally used in realloc
      studNode nullSnode;
      
      int cnum; // Number of student nodes currently in array
      int cmz; // Current Max size of the array (may need to be expanded)

public: // Publically supported methods

// ****************  Constructors / Destructor

  SnEsa (int ms) {     //  1. Normal Constructor.  Create ESA of this size.  May need to be resized later

 // Allocate Enhanced Student Node Array capable of holding this many Student Nodes
    snap = new studNode [cmz = ms]; // snap points to an array of Student node ptrs of size cmz
    cnum = 0; tp = NULL; // Initialize with no nodes currently in array
}
       
// SnEsa ();                // 2. Default Constructor <Commented out>

  SnEsa (SnEsa& sep) {   // 3. Copy Constructor  (Create new array and copy Student Nodes, not Student ptrs or Student Node ptrs)
	cnum = sep.cnum;  cmz = sep.cmz;
	tp = sep.snap;  // Start of source array
	snap = new studNode [cmz];  // snap pts to new array.  Copy existing contents of supplied ESA
	for (int i = 0; i < cnum; i++) snap[i] = tp[i];
	tp = NULL;
  }
	

  ~SnEsa() { delete [] snap; }    // 4. Default destructor  (Free array of Student Nodes, not Students ptd to)


// ******************* Remaining public functions of the Extended Student Node Array

	// 1. Return the number of Student Nodes in the Collection
  int getSize() { return (cnum); };


	// 2. GET:  Get and return the Student Node  at the specified Array Index
	//          Error if the supplied index >= number of entries.  Return a NULL ptr
  studNode get(int idx){
	if ((idx<0) || (idx>=cnum)) return (nullSnode);  // Bad index
	return (snap[idx]); // Otherwise return Student Node at that index in the array
  } 


	// 3. SET: Replace whatever is at the supplied index with the new Student Node ptr. Return index # on success
	// Error if the supplied index >= number of entries.  Return -1
	// <DO NOT Destruct the Student Node whose ptr is at the supplied index>
	// 
  int set(studNode sn, int idx) {
	if ((idx<0) || (idx>=cnum)) return (-1);  // Bad index
	snap[idx] = sn;
	return (idx);
  }


	// 4. INSERT: Insert Student Node at the supplied index, by first "pushing back" every subsequent element
	//    Error if the supplied index >= number of entries.  Return -1
	//    Note:  ESA size increases which may force a reallocation of the array.  Return index # on success
  int insert(studNode sn, int idx) {
	if ((idx<0) || (idx>cnum)) return (-1);  // Bad index (note:  Ok if idx == cnum, it's an append)
	if (cnum == cmz) { // Whoa!  Array at max size now ... realloc needed
		tp = new studNode [cmz = (2 * cmz)];  // Allocate twice the current max size and reset max
		cout << "Doubling Array size to " << cmz << endl << endl;
		for (int i = 0; i < cnum; i++) tp[i] = snap[i];  // Copy all existing Student node ptrs
		delete [] snap;		// Free old array
		snap = tp;		// Set sap to start of new array
	}
	for (int i = cnum-1; i >= idx; i--) snap[i+1] = snap[i]; // Move following ptrs back one
	snap[idx] = sn;		// Insert new entry
	cnum++;			// Increase # Student node ptrs
	return (idx);
  }



	// 5. REMOVE: Delete specified array element by "pulling forward" every subsequent element
	//    Error if the supplied index >= number of entries.  Return -1.  Else return # of remaining elements
	//    Note: <DO NOT Destruct the Student whose Node ptr is at the supplied index>
  int remove(int idx) {
	if ((idx<0) || (idx>=cnum)) return (-1);  // Bad index
	for (int i = idx; i < cnum-1; i++) snap[i] = snap[i+1];  // Pull forward all subsequent elements
	return (--cnum);
  }


	// 6. APPEND:  Append the ptr to the supplied Student node to back of the ESA, bump ESA size
	//    Return index # of new entry on success, -1 on failure
	//    Note:  This may force a reallocation of the array.
  int append(studNode sn) { return (insert (sn, cnum)); }


	// 7. PREPEND: Prepend the ptr to the supplied Student Node to the front of the ESA, increment ESA size
	//    Return 0 on success, -1 on failure
	//    Note:  This may force a reallocation of the array.  
  int prepend(studNode sn) { return (insert (sn, 0)); }
};

//    ********************** End class SnEsa ******************************

// 3. ********** Homework 4.  insert the code for the array-based Min Heap into the StudMinHeap interface below ***************
//
//  A.  Complete "percolDown (int parentIndex)"  <Recursive private function that perculates root node down to its proper position>
//  B.  Write "getPriSize (int priorityLevel)"   <Return number of Student Nodes in Min Heap with the specified priority level>
//        This can be done in 2 very different ways (via recursion or not)
//        Bonus points for coding both and explaining which is superior


// class "StudMinHeap"   <Homework #4> 
// Uses Student priority as Heap value.
// Wraps an internal (embedded) Enhanced Array to collect "studHNode structure ptrs" (instead of Student *'s)

class StudMinHeap {

private:
    SnEsa* snap;  // Allocated Extended Student Node * Array 
    studNode cNode;        // Student Node 
    studNode pNode;	    // Student Node's Parent
    int pidx;               // Index to parent Node
    int cidx;               // Index to child node
    studNode nullSnode; // NULL Student Node

    // *******  Private Methods.

		// Perculate up
		// New element has been be appended to the back of the heap.  Must be perculated up
		// 
    int percolUp (int sidx) {  // Perculate new element up from starting position.
				 // Recursive routine.  Bottoms out when starting position = 0
				 // Returns index of parent (after possible swap) or -1 (done)

	    if (!sidx) return (-1);  // Bottomed out at lower level	 
	    pidx = (sidx % 2) ? ((sidx -1) / 2) : ((sidx -2) / 2);	// pidx is Parent of both 2*pidx+1 and 2*pidx+2 sidx children

		// Get Student Node ptrs of parent and possible children
	    pNode = snap->get(pidx);  //  Parent Node
    	cNode = snap->get(sidx); //   Child Node
    	
	    if (pNode.priority <= cNode.priority) return (-1); // If swap not needed, done.
		// Note:  Not swapping on equal priorities gives advantage to older entries

		//Swap needed.  Then recursively call self with child now the parent
	    snap->set (cNode, pidx);
	    snap->set (pNode, sidx);
	    return (percolUp (pidx));
    }

   // ****************  THIS IS THE FIRST ROUTINE YOU HAVE TO WRITE  ***************
		// Perculate Down
		// Top (root) element has been popped from Heap.  Last element has been moved there to replace it 
		// Perculate root node down to correct position (i.e. swap parent with child until Min Heap order is reestablished)
		// Strategy:
		//    This is a recursive routine.  It starts with the index of the root node (0)
		//	if no children:  done  (Return -1)
		//	if 1 child:  It is the left child. Perculate down if needed, otherwise done (Return -1)
		//  	if 2 children:
		//		Always try to perculate down with the child node of lower priority
		//		In case of ties, try perculating to the left
		//		If no perculate needed ... done (Return -1)
		//      If perculate down did occur, , return index # of child after swap


      int percolDown (int sidx)  	{ // If needed, swap parent node and lower priority child node
                                          // If swap occurs, return child as new parent or -1 if no swap
	    int hsz, lidx, ridx;
	    
	    if (sidx == -1) return (-1);		// Perculate Down bottomed out at lower level
	    
	    hsz = snap->getSize(); // Size of Heap
	    
		// Get Node indexes within Heap Array (starting offset in sidx is parent)
	    pidx = sidx;		    			// Parent index
	    lidx = (hsz > (2*pidx+1)) ?  (2*pidx+1) : -1;  // Left child index or -1
	    ridx = (hsz > (2*pidx+2)) ?  (2*pidx+2) : -1;  // Right child index or -1
      
      pNode = snap->get(pidx);  //  Parent Node
    	cNode = snap->get(sidx); //   Child Node

      if (lidx == -1 && ridx == -1) {
          return -1; // No children, done
      } else if (ridx == -1) {
          // Only left child exists
          if (pNode.priority > cNode.priority) {
              // Swap parent and left child
              snap->set(cNode, pidx);
              snap->set(pNode, sidx);
              return lidx;
          } else { return -1; } // no swap needed
      } else {
          // Both left and right children exist
          int swapIndex = (snap->get(lidx).priority < snap->get(ridx).priority) ? lidx : ridx;

          if (pNode.priority > snap->get(swapIndex).priority) {
              // Swap parent and lower priority child
              snap->set(snap->get(swapIndex), pidx);
              snap->set(pNode, swapIndex);
              return swapIndex;
          } else {
              return -1; // No swap needed, done
            }
        }
    }
      

public: 
    StudMinHeap(int sz) {  // Create a Student Minimum Heap
         snap = new SnEsa (sz); // Create array of Student Nodes at specified Heap size (SnEsa will auto-extend if neccessary)
	     cNode = pNode = nullSnode;
    }

    int getSize() { return (snap->getSize()); };  // Gets # elements (Student Node's) in the Heap

    bool isEmpty() { return ((getSize() == 0) ? true : false); } // True if no node ptrs held in heap

    StudMinHeap(StudMinHeap& hold) { // Equate this to an existing Heap (can use private parts directly)
				     // *** Does NOT have to duplicate Nodes ... that is done in the EA equate
	   *snap = *hold.snap;	// Equate 2 ESAs
	    cNode = pNode = nullSnode;
    }

    ~StudMinHeap() { // Destruct the Heap (free ESA space)
       		     // *** Can just call ESA destructor (freeing Student Nodes, but not students)
	    delete (snap);	// Delete Extended array	
    }

    Student* look() {
	   return (snap->get(0).studp); // Return Ptr to root (highest priority) Student.  If Heap empty this is NULL
    }	 
	
// ****************************** Three Min Heap Functions:  Push, Pop and Get Number Students with priority X  *************************

    int push (unsigned int pri, Student *sp) { // Add new Student to heap at indicated priority
              // Rebalance and return array size
       int pidx = 0; // Parent Student Node* index location
       studNode pp = nullSnode;  // Parent Student Node

       studNode sn(pri, sp);  // Set new Student Node and initialize it to supplied arguments
       int snidx = snap->append (sn);  // Append new element to back of Heap (at index snidx). Heap size incremented
					// Now percolate up
       (void) percolUp (snidx);
       
    // Debug statements
       for (int i = 0; i <= snidx; i++)
       {
           cout << "Pos  " << i << "  Pri =  " << snap->get(i).priority;
           cout << "  Name =   "<< snap->get(i).studp->getName() << endl;
       }
       return (getSize());  // Return New size of Heap
    }

    Student* pop() { // Pop highest priority node from Min Heap (in root position) & rebalance lower nodes.
		     // Returns pointer to the Student whose priority was Heap Root
	int hsz = getSize();	// # Student Nodes in the Heap
	Student *sp; // Student Ptr to return  (Client (mainline) does NOT know about Student Nodes)
	
	if (!(sp = snap->get (0).studp)) return (sp); // A Zero student pointer in Root Node means Heap is empty

		//  Else replace the root Student Node with the bottom (lowest priority) Student
	snap->set (snap->get (hsz-1), 0);
		// Delete last entry in Student Node ptr Extended array
	hsz = snap->remove (hsz-1);
		
// Now percolate down Start with Parent Index of 0 (root position).  If >= than any children, stop)
	(void) percolDown (0);
	return (sp);
    }

    //METHOD 1::
    int getPriSize (unsigned int priNum) { // Return number of Students at specified priority 
    //note: We must traverse the entire tree, or array in this case. O(n) runtime
      int count = 0;

      for (int i = 0; i < snap->getSize(); i++) {
          studNode currentNode = snap->get(i);
          if (currentNode.priority == priNum) {
              count++;
          }
      }
      return count;
    }

    //METHOD 2:: Note:: ChatGPT used for help on recursive function. 
public:
    int getPriSize(unsigned int priNum) {
        // Start the recursion from the root (index 0)
        return getPriSizeRecursive(0, priNum);
    }

private:
    int getPriSizeRecursive(int currentIndex, unsigned int priNum) {
        // Base case: reached the end of the heap
        if (currentIndex >= snap->getSize()) {
            return 0;
        }

        // Check the priority of the current element
        unsigned int currentPriority = snap->get(currentIndex).priority;

        // If the priority matches, increment the count
        int count = (currentPriority == priNum) ? 1 : 0;

        // Recur on left and right children
        int leftChildIndex = 2 * currentIndex + 1;
        int rightChildIndex = 2 * currentIndex + 2;

        // Recursive call for left and right children, and accumulate the counts
        count += getPriSizeRecursive(leftChildIndex, priNum);
        count += getPriSizeRecursive(rightChildIndex, priNum);

        return count;
    }

  /*
  I'm inclined to say that the more straight forward approach of checking the array is the better option here as it's much simpler and readable. After doing some online research as well, I'd say that in this case
  the iterative approach is more reasonable to implement because of its much more practical approach.  
  */

};

/*
 Why can’t AVL trees also be implemented by an Extended Array?
• Similar balanced tree structure
• Similar node structures (Parent *, LHC*, RHC*, ID, Thing*)
*/


/*
AVL trees can't be implemented by extended arrays because of their reliance on tree balancing. One could implement something similar to an AVL tree through an 
extended array, but its practicality would be severly limited by its runtime and the complexity of such an implementation would be unrealistic compared to 
a standard implementation. Additionally, the array would require a pointer system to track relative students and parents, so you'd need an even higher amount
of complexity to keep track of all variables within the array. It's not practical at all, but it's technically possible. 
*/



/*
Is there any way to “enhance” a Heap such that Heap elements with the
same priority level will be “popped” in the order they were “pushed” (i.e.
each priority level is a FIFO Queue instead of a Bag?
• If there is, how could that be done? If not, explain why not.
*/


/*
Yes. You could do so for this example by creating a second data structure. By using this method, when an add command is called, the heap would add the element normally
and an extendable array of structs containing both a priority number and a FIFO queue would push that same element to its own FIFO queue. When a remove command is called,
the order to the FIFO queue would be called first (Note: the program would need to take in a priority level for its parameter).  And this element would be popped from the FIFO queue. 
That same element would need to be located within the heap and removed accordingly. 

It's confusing and hard to describe in words, but it's possible.

ChatGPT used for suggestions
*/


// **************************** End class StudMinHeap ******************************


// 4.  ***********************  Start of StudMinHeap Test Program Code  ************

int main()
{ // Redirect Input
    string infileName; // Use if Input redirected
    string outfileName; // Use if output redirected
    FILE** input = NULL;  // Recovering Cin

    // Command File Record entries
    int ssize; // Size of extended array
    int nops; // # operations to perform

    char command; // Command (S,N,Z   H,P,L)
            // Not all of the following are present in each command (Default given)
    int sid;  // Student ID Number (-1 default)
    string sname;  // Student Name ("-1" default)
    unsigned int spri; // Priority of Student (0 default)

    StudMinHeap* smh = NULL;   // Pointer to Heap
    Student *sp = NULL; // Student Ptr;

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

    // First line is initial Heap size and # commands to execute, every subsequent command is one of:
    // 
    // GetSize:      S 
    // GetNumatPri   N  / PriorityLevel
    // isEmpty:      Z
    // 
    //  Push:  	     H / StudentID / StudentName / PriorityLevel
    //  Pop:         P 
    //  Look:  	     L 
  

    // Get Size of Extended Array and # of commands
    // ssize is size of extended array, nops is # commands

    inp >> ssize >> nops;
    cout << "Read Array size " << ssize << "  Read # commands " << nops << endl;

    smh = new StudMinHeap(ssize);  // Create Student Min Heap of specified size.


    for (int i = 0; i < nops; i++) {  // Process Commands 
        //**************************************************************
        inp >> command;
        cout << "Command: " << command << "   ";

        // Process each command
        switch (command)
        { // Convert to command for Extended Array
            case 'S':  // Get Size
                cout << "Size:  " << smh->getSize() << endl;
                break;

	       case 'N': // Get Number at supplied Priority Level
 	            inp >> spri;
	            cout << "Number of Students at Priority Level  " << spri << "   is   " << smh->getPriSize (spri) << endl;
	            break;

            case 'Z':  // Check if Empty
               cout << "Empty (T/F) : " << smh->isEmpty() << endl;
               break;

             case 'H':  // Push
                inp >> sid;
                inp >> sname;
	            inp >> spri;
	            cout << "   ID:   "<< sid << "  NAME:  " << sname << "  Priority:  " << spri <<endl;
	            sp = new Student (sid, sname);
                (void)smh->push(spri, sp);
                break;

            case 'P': // Pop
                sp = smh->pop();
                if (sp) // Success
                    cout << "   Popped  " << sp->getId() << "  " << sp->getName() << endl;
                else
		            cout << "****  Heap Empty  ****" << endl;
                break;

            case 'L':  // Look (at highest priority student)
                sp = smh->look();
                if (sp) // Success ... Report on Student
                    cout << "  Next to Select " << sp->getId() << "  " << sp->getName() << endl;
	            else
		            cout << "****  Heap Empty  ****" << endl;
    	        break;

            default:
                cout << "   Illegal Command:  " << command << endl;
        }
    }
    // All commands processed. Print out Current contents of extended array
    x = smh->getSize(); // Get number of array elements
    cout << "------------------" << endl << "Start Popping from bottom  " << x << "  Elements queued" << endl << endl;

    while (sp = smh->pop())
    {
            cout << "Student: ID = " << sp->getId() << "  Name = " << sp->getName() << endl;
    }
    return (0);
};