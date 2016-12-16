#include<iostream>
#include<fstream>
#include<conio.h>
#include<iomanip>
#include<string>
using namespace std;

#define MAX 512

char Decrypted[MAX]; // Holds decrpyted message
char Encrypted[MAX]; // Holds encrypted message
char Original[MAX]; // Holds original message

void decrypt(int length, char key); // Decrypts an encrypted message
void encrypt(int length, char key); // Encrypts a message


int main(int argc, char *argv[]) {	
	while (true) {
		string path = "";
		char key;
		int numChars = 0;
		string choice = "";
		std::cout << ">> What would you like to do?\n";
		std::cout << ">> 1) Encrypt\n";
		std::cout << ">> 2) Decrypt\n";
		std::cout << ">> 3) Exit\n";
		std::cin >> choice;

		if (choice == "1") {
			std::cout << ">> Enter the filename you would like to encrypt > ";
			cin >> path;
			std::cout << ">> Enter a letter that will be the key for this encryption > ";
			cin >> key;

			std::ifstream in(path);
			if (in) {
				int i = 0;
				if (in.is_open()) {
					while (!in.eof()) {
						in.get(Original[i]);						
						i++;
						numChars++;
					}
				}
				std::cout << ">> FILE SUCCESSFULY READ\n\n";
			}
			else {
				std::cout << ">> FILE NOT FOUND\n\n";
			}
			in.close();

			encrypt(numChars, key);
			ofstream stream;
			stream.open("ENCRYPTED.txt");
			if (!stream) {
				std::cout << ">> ERROR OPENING TEXT FILE\n\n";
			}
			stream.write((char *)Encrypted, numChars - 1);
			stream.close();
			cout << ">> TEXT FILE " << path << " HAS BEEN ENCRYPTED\n";
			cout << ">> CONTENTS PLACED IN FILE 'ENCRYPTED.txt'\n\n";
		}
		else if (choice == "2") {
			std::cout << ">> Enter the filename you would like to decrypt > ";
			cin >> path;
			std::cout << ">> Enter a letter that will be the key for this decryption > ";
			cin >> key;

			std::ifstream in(path);
			if (in) {
				int i = 0;
				if (in.is_open()) {
					while (!in.eof()) {
						in.get(Encrypted[i]);
						i++;
						numChars++;
					}
				}
				std::cout << ">> FILE SUCCESSFULY READ\n\n";
			}
			else {
				std::cout << ">> FILE NOT FOUND\n\n";
			}
			in.close();

			decrypt(numChars, key);
			ofstream stream;
			stream.open("DECRYPTED.txt");
			if (!stream) {
				std::cout << ">> ERROR OPENING TEXT FILE\n\n";
			}
			stream.write((char *)Decrypted, numChars - 1);
			stream.close();
			cout << ">> TEXT FILE " << path << " HAS BEEN DECRYPTED\n";
			cout << ">> CONTENTS PLACED IN FILE 'DECRYPTED.txt'\n\n";
		}
		else if (choice == "3") {
			return 0;
		}
		else {
			std::cout << ">> INVALID KEY ENTERED\n\n\n\n";
		}
	}
}

void decrypt(int length, char key) {
	int i;	// Loop counter
	int numChars = length;

	__asm {
		mov i, 0 // set loop counter to 0
		jmp Check // jump to check

		Looper :
			mov ebx, i // copy i into ebx
			add ebx, 1 // increment ebx by 1
			mov i, ebx // copy ebx back to i, incrementing i by 1

			Check :
				mov eax, numChars // value of length numChars copied into eax
				cmp i, eax // compares loop counter and number of characters entered
				jge endloop // if loop counter is greater than or equal to number of characteres entered, jump to endloop
				push eax // save on stack
				push ecx // save on stack
				mov ebx, i // moves value of i into the ebx for array access
				movzx ecx, byte ptr[Encrypted + ebx] // Moves bit value of character to the least significant bit of the ecx register. Rest of ecx bits are zero			
				lea eax, key // memory address of key is put into eax as a pointer
				push eax // save key memory address pointer to stack
				push ecx // save value of character to stack			
				call decryption // subroutine to decrypt the returned character			
				add esp, 8 // clears used parameters			
				or al, 0x20 // converts uppercase to lowercase
				mov[Decrypted + ebx], al // decrpypted character is stored in least al, copied into decrypted array				
				pop ecx // restore original values
				pop eax
				jmp Looper // jump back to the Looper
				endLoop :
	}
	return;

	// @param [ebp + 12]: address of key
	// @param [ebp + 8]: the character to decrypt
	// returns decrypted character in eax
	__asm {
		decryption:
			push ebp // push ebp register to save current state
			mov ebp, esp // stack pointer pushed into base 
			mov ecx, [ebp + 8] // character to be encrypted copied into ecx
			mov eax, [ebp + 0xC] // key copied into eax
			push edi // save edi register on stack
			push ecx // save character for encryption on stack
			not byte ptr[eax] // least significant bit of key set to opposite, must obtain same modified key
			add byte ptr[eax], 0x04 // increment lest significant value of key by hex 04
			movzx edi, byte ptr[eax] // least significant value of key copied to edi register
			pop eax // character for decryption removed from stack into eax
			sub al, 0x04 // decrypted by reversing encryption routine
			ror al, 2 // then right rotation reverses left rotation
			xor eax, edi // reverses the xor in the encryption
			pop edi // key is cleared from register, original value restored
			pop ebp // restore original ebp values
			ret	
	}
}
void encrypt(int length, char key) {
	int i;	// Loop counter
	int numChars = length;

	__asm {
		mov i, 0 // set loop counter to 0
		jmp Check // jump to check

		Looper :
			mov ebx, i // copy i into ebx
			add ebx, 1 // increment ebx by 1
			mov i, ebx // copy ebx back to i, incrementing i by 1

			Check :
				mov eax, numChars // value of length numChars copied into eax
				cmp i, eax // compares loop counter and number of characters entered
				jge endloop // if loop counter is greater than or equal to number of characteres entered, jump to endloop
				push eax // save on stack
				push ecx // save on stack
				mov ebx, i // moves value of i into the ebx for array access
				movzx ecx, byte ptr[Original + ebx] // Moves bit value of character to the least significant bit of the ecx register. Rest of ecx bits are zero			
				lea eax, key // memory address of key is put into eax as a pointer
				push eax // save key memory address pointer to stack
				push ecx // save value of character to stack			
				call encryption // subroutine to decrypt the returned character			
				add esp, 8 // clears used parameters							
				mov[Encrypted + ebx], al // decrpypted character is stored in least al, copied into decrypted array				
				pop ecx // restore original values
				pop eax
				jmp Looper // jump back to the Looper
				endLoop :
	}
	return;

	// @param [ebp + 12]: address of key
	// @param [ebp + 8]: the character to decrypt
	// returns decrypted character in eax
	__asm {
		encryption:
			push ebp // push ebp register to save current state
			mov ebp, esp // stack pointer pushed into base 
			mov ecx, [ebp + 8] // character to be encrypted copied into ecx
			mov eax, [ebp + 0xC] // key copied into eax
			push edi // save edi register on stack
			push ecx // save character for encryption on stack
			not byte ptr[eax] // least significant bit of key set to opposite, must obtain same modified key
			add byte ptr[eax], 0x04 // increment lest significant value of key by hex 04
			movzx edi, byte ptr[eax] // least significant value of key copied to edi register
			pop eax // character for decryption removed from stack into eax
			xor eax, edi
			pop edi // key is cleared from register, original value restored
			rol al, 2
			add al, 0x04						
			pop ebp // restore original ebp values
			ret
	}
}