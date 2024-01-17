//Programming Problems 1. [5 pts] Write a function that receives a string containing a 16-bit 
//Binary Unsigned Integer. The function must return the string’s Integer (int) value.
//Test Cases should At Least include one Small, one Middle and one Full 16-bit value.

#include <iostream>
#include <string>

using namespace std;

/*
//my way::: read through each char and ^2 based on position
int binaryToU_I_16(string U_I_16){
    if(sizeof(U_I_16) )
    
    int sum = 0;

    for(int i=0; i<16; i++){

    }

    return sum;
}
*/

int main(){
    string med = "010011000000100";
    string small = "000000000000100";
    string large = "111111111111111";

    cout << sizeof(large);

    return 0;
}