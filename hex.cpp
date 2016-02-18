
/*
 *  Ryan McCoppin
 *  January 26, 2016
 *  
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

#define DEBUG true

void printBytes(unsigned char * , int );
char* bytesToHex(unsigned char * );
void printBytes(unsigned char *, int );
char hexArray[] = "0123456789ABCDEF";

char* bytesToHex(unsigned char * bytes) {
    size_t len_bytes = strlen((const char *) bytes);
    char* hexChars = (char*) malloc (2*len_bytes+1);
    int j;
    for ( j = 0; j < len_bytes; j++ ) {
        int v = bytes[j] & 0xFF;
        hexChars[j * 2] = hexArray[v >> 4];
        hexChars[j * 2 + 1] = hexArray[v & 0x0F];
    }
    hexChars[j*2] = 0;
    return hexChars;
}

unsigned char* hexToBytes( char *hexChars) {
    //cout << strlen(hexChars) << endl;
    
    size_t sizeof_bytes = (strlen(hexChars)+1)/2+1;
    unsigned char* bytes = (unsigned char*) malloc ( sizeof_bytes);
    char *pos = hexChars;
    int j;

    size_t count = 0;
    for(count = 0; count < sizeof_bytes; count++) {
        sscanf(pos, "%2hhx", &bytes[count]);
        pos += 2;
    }
    bytes[count] = 0;
    return bytes;
}

void printBytes(unsigned char * bytes, int len) {
    size_t count = 0;
    printf("0x");
    for(count = 0; count < len; count++)
        printf("%02X", bytes[count]);
    printf("\n");
}

int main(int argc, char* argv[]) {
    cout << "Hello, world" << endl;
    
    unsigned char bob[] = {0,1,2,3,4,5};
    cout << sizeof(bob) << endl;
    cout << bytesToHex(bob) << endl;
    cout << "argc = " << argc << endl; 
    for(int i = 0; i < argc; i++) 
      cout << "argv[" << i << "] = " << argv[i] << endl; 
      
    char hexstring[] = "deadbeef10203040b00b1e50";
    unsigned char * val;
    size_t count = 0;

    //cout << sizeof(hexstring) << endl;
    val = hexToBytes(hexstring);
    printf("0x");
    for(count = 0; count < 12; count++)
        printf("%02x", val[count]);
    printf("\n");
    
    
    
    return 0;
}