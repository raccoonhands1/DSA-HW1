//Programming Problems 1. [5 pts] Write a function that receives a string containing a 16-bit 
//Binary Unsigned Integer. The function must return the string’s Integer (int) value.
//Test Cases should At Least include one Small, one Middle and one Full 16-bit value.

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

//note to Professor Lau, I chose not to include edge cases for every case I could think of and instead opted for simplicity with checking only the string length
//I hope this is OK and I am still preparing my list of questions to ask you about microprocesses
int binaryToU_I_16(string S_32){
    if(S_32.length() > 16){ return -1; } //over 16 char
    
    int sum = 0;
    int iterator = 0;

    for(int i=S_32.length()-1; i>=0; i--){
        if(S_32.at(i) == '1'){
            sum += pow(2, iterator);
        }
    iterator++;
    }

    return sum;
}

int hexToU_I_32(string S_16){
    if(S_16.length() > 8){ return -1; } //over 8 char
    
    int sum = 0;
    int iterator = 0;

    for(int i=S_16.length()-1; i>=0; i--){
        if(S_16.at(i) < 15){
            sum += pow(16, iterator);
        }
    iterator++;
    }

    return sum;
}




int main(){
    string medb = "010011000000100";
    string smallb = "000000000000100";
    string largeb = "111111111111111";

    string medh = "0003455B";
    string smallh = "00001A";
    string largeh = "FFFFFFFA";

    //cout << binaryToU_I_16(smallb);

    cout << hexToU_I_32(medh);

    return 0;
}