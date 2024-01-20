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

//note to Professor Lau, I referenced sources online to streamline & optimize my code
unsigned int hexToU_I_32(string S_32) {
    if (S_32.length() > 8) { return -1; } // Over 8 characters

    unsigned int sum = 0;
    int iterator = 0;

    for (int i = S_32.length() - 1; i >= 0; --i) {
        char C_Hex = S_32.at(i);
        int decimalValue;

        //efficiently designed check for characters without the use of std::stoi()
        // the ascii value of '0' != 0, it's 48
        if (C_Hex >= '0' && C_Hex <= '9') {
            decimalValue = C_Hex - '0';
        } else if (C_Hex >= 'A' && C_Hex <= 'F') {
            decimalValue = C_Hex - 'A' + 10;
        } else if (C_Hex >= 'a' && C_Hex <= 'f') {
            decimalValue = C_Hex - 'a' + 10;
        } else {
            return -1;
        }
        sum += decimalValue * pow(16, iterator); //val*16^0,1,2,3
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
    string largeh = "FFFFFFFF";

    cout << 
    "Binary: M: " << binaryToU_I_16(medb) << 
    " S: " << binaryToU_I_16(smallb) <<
    " L: " << binaryToU_I_16(largeb) <<

    '\n' << 

    "Hex: M: " << hexToU_I_32(medh) << 
    " S: " << hexToU_I_32(smallh) << " L: " 
    << hexToU_I_32(largeh);

    return 0;
}