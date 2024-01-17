#include <string>

using namespace std;

class Student //no modifications
{  
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

    //Setters
  
    //Getters 
    string getName() { return (sname); };
    int getId() { return (sid); };

private:
    int sid; // Student ID
    string sname; // Full Name (Ex: KleinmanRon)
};




struct sItem { //no modifications
    Student* stuPtr;
    int setVal;


bool operator==(const sItem& rhs) const {
    return this->stuPtr == rhs.stuPtr;
}
};




class SiEsi {
private:  // Define whatever additional private variables you need
      sItem* snap; // Underlying pointer to array of node ptrs
      sItem* tp;  // Optionally used in realloc
      sItem nullSnode;
      
      int cnum; // Number of student nodes currently in array
      int cmz; // Current Max size of the array (may need to be expanded)

public: // Publically supported methods

  SiEsi (int ms) {     //  1. Normal Constructor.  Create ESA of this size.  May need to be resized later

    snap = new sItem [cmz = ms]; // snap points to an array of Student node ptrs of size cmz
    cnum = 0; tp = NULL; // Initialize with no nodes currently in array
}

  SiEsi (SiEsi& sep) {   // 3. Copy Constructor  (Create new array and copy Student Nodes, not Student ptrs or Student Node ptrs)
	cnum = sep.cnum;  cmz = sep.cmz;
	tp = sep.snap;  // Start of source array
	snap = new sItem [cmz];  // snap pts to new array.  Copy existing contents of supplied ESA
	for (int i = 0; i < cnum; i++) snap[i] = tp[i];
	tp = NULL;
  }
	

  ~SiEsi() { delete [] snap; }    // 4. Default destructor  (Free array of Student Nodes, not Students ptd to)


  int getSize() { return (cnum); };

  sItem get(int idx){
	if ((idx<0) || (idx>=cnum)) return (nullSnode);  // Bad index
	return (snap[idx]); // Otherwise return Student Node at that index in the array
  } 

  int set(sItem sn, int idx) {
	if ((idx<0) || (idx>=cnum)) return (-1);  // Bad index
	snap[idx] = sn;
	return (idx);
  }

  int orderInsert(sItem sn){ /*new method! for the extendible orderable array, this'
   method auto orders based on numerical ascension specifically for the sItem struct. Can be templatified*/

    //i used chatgpt for this because I didn't want to have to think about this. Search and sort algorithms are my weak point!

    if(cnum == 0) { return append(sn); } //if no elements in array

    //check throughout array. When found spot, insert at index.
    //binary search

    int low = 0;
    int high = cnum - 1;

    // Binary search to find the correct position
    while (low <= high) {
        int mid = (low + high) / 2;

        if (sn.setVal == snap[mid].setVal) {
            // Insert at the found position if setVal is equal
            return insert(sn, mid);
        } else if (sn.setVal < snap[mid].setVal) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    // If the loop exits, it means the element is not found,
    // and low is the correct position to insert the new element.
    return insert(sn, low);
  }

  int sort(){ //new method to sort entire array.
    for (int i = 1; i < cnum; i++) {
        sItem key = snap[i];
        int j = i - 1;

        // Move elements of snap[0..i-1] that are greater than key.setVal
        // to one position ahead of their current position
        while (j >= 0 && snap[j].setVal > key.setVal) {
            snap[j + 1] = snap[j];
            j = j - 1;
        }
        snap[j + 1] = key;
    }
  }

  int insert(sItem sn, int idx) {
	if ((idx<0) || (idx>cnum)) return (-1);  // Bad index (note:  Ok if idx == cnum, it's an append)
	if (cnum == cmz) { // Whoa!  Array at max size now ... realloc needed
		tp = new sItem [cmz = (2 * cmz)];  // Allocate twice the current max size and reset max
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

  int remove(int idx) {
	if ((idx<0) || (idx>=cnum)) return (-1);  // Bad index
	for (int i = idx; i < cnum-1; i++) snap[i] = snap[i+1];  // Pull forward all subsequent elements
	return (--cnum);
  }

  int append(sItem sn) { return (insert (sn, cnum)); }

  int prepend(sItem sn) { return (insert (sn, 0)); }
};