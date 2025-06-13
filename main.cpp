#include <string>
#include <iostream>
#include <fstream>
#include <termios.h>
#include <unistd.h>

using namespace std;

const int shift = 5;

string encrypt(string plaintext, int shift) {
    string ciphertext = "";
    int n = plaintext.length();

    for (int i = 0; i < n; i++) {   
        char c = plaintext[i];
    
        if (c >= 'A' && c <= 'Z') {
            c = ((c - 'A' + shift) % 26) + 'A';
        } else if (c >= 'a' && c <= 'z') {
            c = ((c - 'a' + shift) % 26) + 'a';
        }

        ciphertext += c;
    }   
    return ciphertext;
}

string decrypt(string ciphertext, int shift) {
    string plaintext = "";
    int n = ciphertext.length();
    
    for (int i = 0; i < n; i++) {
        char c = ciphertext[i];
    
        if (c >= 'A' && c <= 'Z') {
            c = ((c - 'A' - shift) % 26) + 'A';
        } else if (c >= 'a' && c <= 'z') {
            c = ((c - 'a' - shift) % 26) + 'a';
        }
    
        plaintext += c;
    }
    
    return plaintext;
}

int main(int argc, char *argv[]) {
	if (argc) {
		string arg = argv[1];
		if (arg == "-r") {
			ifstream file("passwords.txt");
			if (file.good()) {
				string line;
				while (getline(file, line)) {
					cout << decrypt(line, shift) << endl; 
				}
			}
			file.close();
		} else if (arg == "-w") {
			termios oldt;
			tcgetattr(STDIN_FILENO, &oldt);
			termios newt = oldt;
			newt.c_lflag &= ~ECHO;
			tcsetattr(STDIN_FILENO, TCSANOW, &newt);

			cout << ": ";
			string password;
			getline(cin, password);
				
			cout << endl;

			ofstream file("passwords.txt");
			file << encrypt(password, shift);

			file.close();
			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		} else {
			cout << "wrong flag" << endl;
		}
	} 
	return 0;
}
