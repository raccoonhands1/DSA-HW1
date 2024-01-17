#include <iostream>
#include <fstream>
#include <string>
#include "hw5.h"

using namespace std;
/* not finished
class hashTable {
public: //what do we need out of this hash table? We just need a pointer to a location

private:
    int tableSize = 40;

};
*/

class siDset {
private: 
    SiEsi* sItemArr; //pointer to the orderable array which will be wrapped
public:
    siDset(int mxsz);
    siDset(siDset &);
    ~siDset();

    int getNum() const;
    sItem get(int idx) const;
    bool store(sItem si, int idx);
    bool insert(sItem si, int idx);
    bool remove(int idx);
    int append(sItem si);

    siDset* unify(const siDset*) const; //union, union is a keyword in C++
    siDset *unifyOrdered(const siDset*) const;
    siDset *intersect(const siDset *) const; // intersect
    siDset* difference(const siDset*) const; //difference
};

siDset::siDset(int mxsz){
    SiEsi* sItemArr = new SiEsi(mxsz); // initialize an ESI with size mxsz for the array of sItems
}

siDset::siDset(siDset &){
//write later                                                                                                                   //
}

siDset::~siDset(){
//write later                                                                                                                   //
}

//wrapper for entire class to fit into an ESA. Reused code works well! For this code, we will create 2 sets which are just 
//2 ESA classes and figure out how to unify them

int siDset::getNum() const {
    return sItemArr->getSize();
}

sItem siDset::get(int idx) const { //returns an sItem at given index
    return sItemArr->get(idx);
}

bool siDset::store(sItem si, int idx) { 
    return sItemArr->set(si, idx) != -1;
}

bool siDset::insert(sItem si, int idx) {
    return sItemArr->insert(si, idx) != -1;
}

bool siDset::remove(int idx) {
    return sItemArr->remove(idx) != -1;
}

int siDset::append(sItem si) { //returns index appended at
    return sItemArr->append(si);
}



//goal is to combine them into one set and essentially skip any id's that may be similar
siDset *siDset::unify(const siDset *secondSet) const {  //chatGPT used to elaborate on unification, intersection, and difference
    if(secondSet == nullptr){ return nullptr; } // if set 2 doesn't exist, return a nullptr

    // inefficient method in slides. If im not mistaken this is worst case nearly O(n^2). Instead, we should hash the entire first table and store it, and then 
    // hash each id in the second table to look for matches from the stored table. I can't implement it right now I don't have time so i'll just write the 
    //pseudo 

    hashtable<sItem>* table = new hashtable(); 

    siDset* outputSet = new siDset(10);

    *outputSet = *this; //copy constructor call. ChatGPT used for syntax. Set output set = this array

    for (int i=0; i< this->getNum(); i++){ //step 1: hash table entire first map and store it
        table->hashtableInsert(this->get(i).stuPtr->getId()); 
    }

    sItem* temp = nullptr;
    setValCounter = this->getNum() + 1; //tracker to update the set val for appended values. Start at size of set1 + 1.

    for(int j=0; j<secondSet->getNum(); j++){
        temp = table->lookup(secondSet->get(j).stuPtr->getId()); //return nullptr if false, return a pointer if true

        if(temp == nullptr){ //match has not been found. append to outputSet
            secondSet->get(j).setVal = setValCounter; //set new counter value ()
            outputSet->append(secondSet->get(j)); //append
            setValCounter++; //increment counter
        }//if no match found, do nothing
    }

    //this algorithm runs much quicker for larger data sets. It depends how you decide to handle 
    //the hashtable internally for the actual runtime ie: how many buckets
    return outputSet;
}

//goal is to combine them into one set and essentially skip any id's that may be similar. Only difference is that we need to order the array afterwards
//sort lowest to highest based on student ID
siDset *siDset::unifyOrdered(const siDset *secondSet) const {  

    //my function works like a zipper. Elements merge together.

    if(secondSet == nullptr){ return nullptr; } // if set 2 doesn't exist, return a nullptr

    //create a new set that contains all of unified data from 1 & 2 so that the user has free will to delete the other 
    siDset* outputSet = new siDset(10); //initialize new array. To reduce complexity, we can use int 10 to initialize since it will be expanded anyways.


    int i=0;
    int j=0;

    //so, I can't tell what we're comparing based off of for this assignment, I'm going to guess student ID.
    //I'm also unsure whether the setVal is supposed to also be the student ID or if it's supposed to be the index value, so I'll assume it's index value. 

    while(i != this->getNum() && j != secondSet->getNum()){ //while a first or second sets have elements remaining


        if(this->get(i).stuPtr->getId() < secondSet->get(j).stuPtr->getId() || secondSet->get(j) == NULL){
            outputSet->append(this->get(i));
            i++;
        } else 
        if(this->get(i).stuPtr->getId() > secondSet->get(j).stuPtr->getId() || this->get(i) == NULL){
            outputSet->append(secondSet->get(j));
            j++;
        } else { //duplicate. In the case of a duplicate, we need to void 1 entry and append the other
            outputSet->append(this->get(i));  //append one (doesn't matter which to append)
            i++; //increment both!
            j++;
    }

    return outputSet;
}

//goal is to include only the ones that share the same key in this new set
siDset *siDset::intersect(const siDset *secondSet) const {
    if(secondSet == nullptr){ return nullptr; }

//not written yet

    return nullptr;
}

//difference and intersect are the same code, but we need to return the opposite sets


//goal is to only include ones that don't show up in both sets
siDset *siDset::difference(const siDset *secondSet) const {
    if(secondSet == nullptr){ return nullptr; }

//not written yet

    return nullptr;
}


int main(){

    //code for db1 & db2 written twice for readability.
    ifstream db1;
    db1.open("db1.txt");     //chatGPT used to create a random fake database with varying id's

    siDset* set1 = new siDset(10); //pointer to a new set (set 1). siDset uses an orderable extendible array so that I can reuse the code from hw4

    if(db1.is_open()){ //read from file 1
        string ifname;
        string ifid;

        while (db1.good()){
            db1 >> ifid;
            db1 >> ifname;

            Student* s = new Student(stoi(ifid), ifname); // initialize students from db1

            sItem i = {s, stoi(ifid)}; //pointer to the student and the unique identifier (student id).

            //time to order in ascending order 000000 --> 999999
            //could use another class for this, but why would I?

            set1->append(i); //append this new sItem to the first set
        }
        cout << "all sItems appended. Total: " << set1->getNum() << '\n';
    } //must order the array at insertion. 

    ifstream db2;
    db2.open("db2.txt");     //chatGPT used to create a random fake database with varying id's

    siDset* set2 = new siDset(10); //pointer to a new set (set 2) siDset uses an orderable extendible array so that I can reuse the code from hw4

    if(db2.is_open()){ //read from file 2
        string ifname;
        string ifid;

        while (db2.good()){
            db2 >> ifid;
            db2 >> ifname;

            Student* s = new Student(stoi(ifid), ifname); // initialize students from db2

            sItem i = {s, stoi(ifid)}; //pointer to the student and the unique identifier (student id).

            set2->append(i); //append this new sItem to the second set
        }
        cout << "all sItems appended. Total: " << set2->getNum() << '\n';
    }

    // we have allocated all of the students. The class above siDset class will create corresponding structs to go along with this class.
    //so, we will need to allocate 2 siDsets and ultimately merge the 2


    siDset* set3 = set1->unify(set2); // unify set 1 with set 2. Contents will be written to new set.

    siDset* set4 = set1->intersect(set2); // unify set 1 with set 2. Contents will be written to new set.

    siDset* set5 = set1->difference(set2); // unify set 1 with set 2. Contents will be written to new set.

    delete set1;
    delete set2;
    delete set3;
    delete set4;
    delete set5;

    return 0;
}