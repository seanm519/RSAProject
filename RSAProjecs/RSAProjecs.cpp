// RSAProjecs.cpp : Sean Murphy and Matthew Cowell implimentation of RSA


#include <iostream>
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <vector>
#include <cryptlib.h>
#include <integer.h>

using namespace std;
using namespace CryptoPP;


//struct to store the modulo n and the private expoment d that makes up the private key
struct privateKey {

    //modulo
    int n;
    //private exponent
    int d;

};

//struct to store the modulo n abd the public exponent that makes up the public key
struct publicKey {

    //modulo
    int n;
    //public exponent
    int e;

};

//struct that stores an instance of the publuc key and the private key for quicker access
struct keyPair {

    publicKey pub;
    privateKey priv;

};

//function declaration
void generateKeys(struct keyPair& keys);
unsigned long long int bitToInt(unsigned long long int numBits);
bool isPrime(unsigned long long int n, unsigned long long int k);
unsigned long long modExp(unsigned long long base, unsigned long long exp, unsigned long long mod);
unsigned long long int gcd(unsigned long long int a, unsigned long long int b);
unsigned long long int RSAEncryption(unsigned long long int plainText, struct keyPair keys);
unsigned long long int RSADecryption(unsigned long long int cypherText, struct keyPair keys);


int main()
{
    //create a set of variables to store the plaintext, cypertext, and decyptedmessage as an integer and a string
    unsigned long long int cypherText, plainText, decryptedText;
    string cypherTextString, plainTextString, decryptedtextString;
    Integer cypherTextCPP, plainTextCPP, decryptedTextCPP;
    //vector<int> cypherText, plainText, decryptedText;

    //create an object of keypair called keys to store the public and private keys
    keyPair keys;

    //first we need to create the public and private keys
    generateKeys(keys);

    //now that we have generated the keys we can move on to encryption and decryption

    //display the public key to the public
    cout << endl << endl << endl << endl << endl << "public key:" << endl << "e: " << keys.pub.e << endl << "n: " << keys.pub.n;
    cout << endl << "transmitting...";

    cout << endl << "enter a string of integers to be encrypted: ";
    cin >> plainText;

   //encrpyt the message
   cypherText = RSAEncryption(plainText, keys);
   //display the encrypted message
   cout << endl << "the encryted text is: " << cypherText;

   //decrypt the encrypted message
   decryptedText = RSADecryption(cypherText, keys);
   //display the decrypted message
   cout << endl << "the decrypted message is: " << decryptedText;

}

//check the primality using fermat's primality test where n in the number to check and k in the number if itterations to run
bool isPrime(unsigned long long int n, unsigned long long int k)
{
    do {
        if (n <= 1 || n == 4) return false;
        if (n <= 3) return true;

        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<unsigned long long> dist(2, n - 2);

        for (int i = 0; i < k; ++i) {
            unsigned long long a = dist(gen);
            if (modExp(a, n - 1, n) != 1) {
                return false;
            }
        }

    } while (n % 2 == 0);
        return true;
    
}


//calculates modular exponentiation
unsigned long long modExp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base %= mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }

    return result;
}

//void function that takes a reference to the keys struct created in the main function and calculates the necessary information to fill the parameters of the public and private keys
void generateKeys(struct keyPair &keys) {
    
    unsigned long long int NLarge1, NLarge2, lengthOfNLarge, p, q, n, e = 0, d = 0, phi, max;
    bool prime;
    time_t t;

    /* Intializes random number generator */
    srand((unsigned)time(&t));

    //get how long the number should be in bits
    cout << endl << "consider using a smaller number of bits to have a shorter response time";
    //this is not accurate as this is dictating the max that is used to generate p and q where as the true limit of the RSA algorithm is the modulo n
    cout << endl << "enter how long the number should be, in bits:";
    cin >> lengthOfNLarge;

    //find out the largest number that can be represented by the users chosen bit size
    max = bitToInt(lengthOfNLarge);

    cout << endl << "max is : " << max;
    //i believe the issue lies with the rand(0 function as the maximum number it can generate is ~37000

    //when asking the use for the key size are we asking them if it should strictly be that sice of if that number of bits it the max or min size?

    //calculate the first prime
    do {
        p = (rand() % max);
        prime = isPrime(p, 20);
    } while (prime != true);


    //calcualte the second prime
    do {
        q = (rand() % max);
        prime = isPrime(q, 20);
    } while (prime != true && q != p);

    

    //calculate n and phi;
    n = p * q;
    phi = (p - 1) * (q - 1);

    

    //insert n into the public and private keys
    keys.priv.n = n;
    keys.pub.n = n;

    //select a random integer 'e' such that 1<e<phi, gcd(e, phi) =1
   /* do {
        e = rand() % phi;
    } while (gcd(e, phi) != 1);*/

    for (int i = phi - 1; i > 1; i--) {
        e = rand() % phi;
        if (gcd(e, phi) == 1) {
            break;
        }
        else if (gcd(i, phi) == 1) {
            e = i;
            break;
        }
    }

    cout << endl << "randome integer e is " << e;
   
    //enter e into the public key
    keys.pub.e = e;

    //find d for the private key
    /*do {
        d = rand() % phi;
    } while ((e*d)%phi != 1);*/

    for (int i = phi - 1; i > 1; i--) {
        d = rand() % phi;
        if ((e*d) % phi ==1){
            break;
        }
        else if ((e*i) % phi == 1) {
            d = i;
            break;
        }
    }


    //enter d into the private key
    keys.priv.d = d;

    //done generating the keys.

    //DEBUG force the key parameters
   /* keys.priv.n = 119;
    keys.priv.d = 77;
    keys.pub.n = 119;
    keys.pub.e = 5;*/

    //DEBUG see what p and q is
    cout << endl << "prime p is: " << p;
    cout << endl << "prime q is: " << q;
    //DEBUG see what n and phi is
    cout << endl << "n is " << keys.priv.n;
    cout << endl << "phi is " << phi;
    //DEBUG display the max number
    cout << endl << "the max number that can be encrypted is: " << keys.priv.n;
    //DEBUG see what d is
    cout << endl << "d is " << d;


}

//calculates the largest number possible for a given number of bits
unsigned long long int bitToInt(unsigned long long int numBits) {
    

    unsigned long long largest_number = static_cast<unsigned long long>(std::pow(2, numBits) - 1);

    return largest_number;
}

//calcualtes the gcd by using the eucligian algoritm
//DEBUG, the cout should be called several times as the gcd function is called several times in order to fund a gcd of 1, the last cout will be the correct one to look at
unsigned long long int gcd(unsigned long long int a, unsigned long long int b) {
    cout << endl << "the gcd of " << a << " and " << b << " is ";
    while (b != 0) {
        unsigned long long int remainder = a % b;
        a = b;
        b = remainder;
    }
    cout << a;
    return a;
}

//perform the RSA encryption
unsigned long long int RSAEncryption(unsigned long long int plainText, struct keyPair keys) {
    unsigned long long int cypherText;

    //c=m^e (mod n)
    //cypherText = pow(plainText, keys.pub.e);
   // cout << endl << "cypher text after power is : " << cypherText;
    //cypherText = cypherText % keys.pub.n;
    cypherText = modExp(plainText, keys.pub.e, keys.pub.n);

    return cypherText;

}

//perform the RSA decryption
unsigned long long int RSADecryption(unsigned long long int cypherText, struct keyPair keys) {
   unsigned long long int decryptedMessage;
    cout << endl << "decrypted message before power : " << cypherText;
    cout << endl << " the private key d is :" << keys.priv.d;
    decryptedMessage = modExp(cypherText, keys.priv.d, keys.priv.n);
    //cout << endl << "decrypt after power is : " << decryptedMessage;

    return decryptedMessage;
}

//example https://condor.depaul.edu/ichu/csc415/notes/notes4/rsa.html