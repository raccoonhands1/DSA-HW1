/******************************************************************************

Homework 3:
This is the framework for what you need to write (and verify as compilable) the
"getPrevious(Node*){}" routine for a Binary Search Tree (BST) of Student nodes.

The BST layout is as described in the Homework slides
Note: "getNext(Node*){}" is provided as a guide


*******************************************************************************/
#include <iostream>

using namespace std;

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
    }
    //Getters 
    string getName() { return (sname); };
    int getId() { return (sid); };
};

struct Node {
    Node* parent;   // Parent Node ptr (NULL if this is root of tree)
    Node* LHC;      // Left Hand Child ptr (or NULL)
    Node* RHC;      // Right Hand Child ptr (or NULL)
    int id;         // Key value that assigns Nodes place in tree (Student ID)
    Student *stud;  // The thing that is being collected
};

class BST {
public:
    BST();
    BST(BST&);
    ~BST ();
    
    Node* getNext(Node*);
    Node* getPrev(Node*);
};

// Finding “Successor” node in BST (if exists):  (Non-recursive)

Node* BST:: getPrev (Node* from) { //returns the previous node
    Node* currNode = nullptr;
    if(from->LHC){ //if there's a LHC, go to the rightmost child
        currNode = from->LHC;
        while(currNode->LHC) currNode = currNode->RHC;
        return currNode;
    } 

    Node* currNode = from;
    while (currNode->parent && currNode == currNode->parent->LHC){
        currNode = currNode->parent;
    }

    return currNode->parent;
}

Node* BST:: getNext (Node* from) 
{ // Returns next node
       Node *currNode = NULL; 
       if (from->RHC) {   // If RHC, find leftmost leaf (10-12)
              currNode = from->RHC;
              while (currNode->LHC)  currNode = currNode->LHC ; // Descend on LHS
              return (currNode);
      }
  //No RHC.  If this is the Root (no parent), we are done … root is highest node
       if (!(currNode = from->parent)) return (NULL);
  // No RHC and not Root. Go up tree until parent found where child was LHC. Return Parent of that Parent// (19-20)
       while (currNode->parent) {   // As long as currNode is not root … move up tree if it’s an RHC of its parent
              if (currNode  == currNode->parent->RHC) currNode = currNode->parent;  
              else   // Either (started 40) currNode is the root node (20) and there is no “next parent” 
                        // or (started 19) currNode is at (15), and its parent (10) is the LHC of the “next” node (20)
                   break;
        }
// If currNode is Root node, this node was RHC to everything INCLUDING Root (40)
// So when the walk is over, the return is either Root Parent (Null) showing failure or the next node in sequence
        return (currNode->parent); 
}