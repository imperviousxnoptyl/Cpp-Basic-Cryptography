#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

//===Setting Up The Rules of Encryption/Decryption and 5x5 Matrix===
string textFormatting (const string& str) {
    string digraphsRearranged = ""; //Step 1: Convert to capital letters and handling non-alphabetical characters
    string result = "";
    for (char ch : str) {
        if (isalpha(ch)) {
            ch = toupper(ch);
            if (ch == 'J') ch = 'I'; //Normalise J to I in plaintext
            digraphsRearranged += ch;
        }
    }
    for (size_t i = 0; i < digraphsRearranged.length(); ) { //Following the rule of no repeated letters in a digraph
        char c1 = digraphsRearranged[i];
        char c2;
        if (digraphsRearranged.length() > i + 1) 
            c2 = digraphsRearranged[i + 1];
        else 
            c2 = 'Z';
        if (c1 == c2) {
            result += c1;
            result += 'X';
            i += 1;
        }
        else {
            result += c1;
            result += c2;
            i += 2; //move to next digraph
        }
    }
    if (result.length() % 2 != 0) { //Completes a digraph if the plaintext has an odd number of letters
        result += 'Z';
    }
    return result;
}

string contFormatting(vector<unsigned char>& str) {
    string digraphsRearranged = "";
    string result = "";
    for (char ch : str) {
        if (isalpha(ch)) {
            ch = toupper(ch);
            if (ch == 'J') ch = 'I';
            digraphsRearranged += ch;
        }
    }
    for (size_t i = 0; i < digraphsRearranged.length(); ) {
        char c1 = digraphsRearranged[i];
        char c2;
        if (digraphsRearranged.length() > i + 1) 
            c2 = digraphsRearranged[i + 1];
        else 
            c2 = 'Z';
        if (c1 == c2) {
            result += c1;
            result += 'X';
            i += 1;
        }
        else {
            result += c1;
            result += c2;
            i += 2; //move to next digraph
        }
    }
    if (result.length() % 2 != 0) {
        result += 'Z';
    }
    return result;
}

void createMatrix(string &key, char char_table[5][5]) {
    vector<bool> cells(26, false); //Contains the count for 26 letters, false because no entries yet
    int r = 0, c = 0;
    for (char& ch : key) {
        ch = toupper(ch);
        if (ch == 'J') ch = 'I';
    }
    for (char ch : key) {
        if (!cells[ch - 'A']) { //Prevents duplicates
            cells[ch - 'A'] = true;
            char_table[r][c++] = ch;
            if (c == 5) { //5 columns but 1 row
                c = 0; //Increase number of rows
                r++; //Reset value of columns to 0
            } //...Repeats until there are 5 columns and 5 rows (same amount in the char_table)
        }
    }
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue;
        if (!cells[ch - 'A']) {
            cells[ch - 'A'] = true;
            char_table[r][c++] = ch;
            if (c == 5) {
                c = 0;
                r++;
            }
        }
    }
}

struct char_pos {
    int r1, c1;
    int r2, c2;
};

char_pos giveCharacterPosition (char c1, char c2, char char_table[5][5]) {
    char_pos position{};
    if (c1 == 'J') c1 = 'I';
    if (c2 == 'J') c2 = 'I';
    bool found_p1 = false;
    bool found_p2 = false;
    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 5; c++) {
            if (char_table[r][c] == c1) {
                position.r1 = r;
                position.c1 = c;
                found_p1 = true;
            }
            if (char_table[r][c] == c2) {
                position.r2 = r;
                position.c2 = c;
                found_p2 = true;
            }
        }
    }
    if (!found_p1 || !found_p2) {
        throw runtime_error("Character not found in Playfair Table!");
    }
    return position;
}

//===Encrypt and Decrypt Contents in String===
void encryptString(string& str, char char_table[5][5]) {
    size_t p_len = str.size();
    for (int i = 0; i + 1 < p_len; i += 2) { //Check whether each two characters in the string are grouped into digraphs
        char_pos position = giveCharacterPosition(str[i], str[i + 1], char_table);
        if (position.r1 == position.r2) {
            str[i] = char_table[position.r1][(position.c1 + 1) % 5];
            str[i + 1] = char_table[position.r2][(position.c2 + 1) % 5];
        }
        else if (position.c1 == position.c2) {
            str[i] = char_table[(position.r1 + 1) % 5][position.c1];
            str[i + 1] = char_table[(position.r2 + 1) % 5][position.c2];
        }
        else {
            str[i] = char_table[position.r1][position.c2];
            str[i + 1] = char_table[position.r2][position.c1];
        }
    }
}

void encryptFlow(string& str, string& key) {
    char char_table[5][5];
    cout << "Enter string: ";
    cin >> str;
    str = textFormatting(str);
    cout << "Enter key: ";
    cin >> key;
    createMatrix(key, char_table);
    encryptString(str, char_table);
    cout << str << endl;
}

void decryptString(string& ciphertext, char char_table[5][5]) {
    size_t p_len = ciphertext.size();
    for (int i = 0; i + 1 < p_len; i += 2) {
        char_pos position = giveCharacterPosition(ciphertext[i], ciphertext[i + 1], char_table);
        if (position.r1 == position.r2) {
            ciphertext[i] = char_table[position.r1][(position.c1 + 4) % 5];
            ciphertext[i + 1] = char_table[position.r2][(position.c2 + 4) % 5];
        }
        else if (position.c1 == position.c2) {
            ciphertext[i] = char_table[(position.r1 + 4) % 5][position.c1];
            ciphertext[i + 1] = char_table[(position.r2 + 4) % 5][position.c2];
        }
        else {
            ciphertext[i] = char_table[position.r1][position.c2];
            ciphertext[i + 1] = char_table[position.r2][position.c1];
        }
    }
    if (p_len > 0 && ciphertext[p_len - 1] == 'Z') {
        ciphertext.erase(p_len - 1);
    }
}

void decryptFlow(string& str, string& key) {
    char char_table[5][5];
    cout << "Enter string: ";
    cin >> str;
    str = textFormatting(str);
    cout << "Enter key: ";
    cin >> key;
    createMatrix(key, char_table);
    decryptString(str, char_table);
    cout << str << endl;
}

bool checkFileExtension(const string& filename) {
    vector<string> validFileExtensions = {".txt", ".rtf", ".doc", ".docx", ".csv"};
    for (const string& file_ext : validFileExtensions) {
        if (filename.find(file_ext) != string::npos) { //If match found:
            return true;
        }
    }
    return false;
}

vector<unsigned char> readf(const string& filename) {
    ifstream file(filename, ios::binary); //makes sure contents in the file is read or written without translating newline to and from \r\n
    vector<unsigned char> tmp((istreambuf_iterator<char>(file)), istreambuf_iterator<char>()); //stores data from file one by one
    return tmp; 
}

void encf(const string& fn, const string& file_cont) {
    ofstream file(fn);
    file << file_cont;
}

void decf(const string& fn, const string& file_cont) {
    ofstream file(fn);
    file << file_cont;
}

void encryptFlowFile(string& key, string& fn) {
    char char_table[5][5];
    ifstream file;
    cout << "Enter filename: ";
    cin >> fn;
    file.open(fn);
    if (checkFileExtension(fn)) {
        if (file) {
        cout << "Enter key: ";
        cin >> key;
        vector<unsigned char> original_cont = readf(fn);
        createMatrix(key, char_table);
        string str = contFormatting(original_cont);
        encryptString(str, char_table);
        string output_file = "encoded.txt";
        encf(output_file, str);
        cout << "Finished encrypting file contents." << endl;
        } else {
            cout << "ERROR: FILE DOES NOT EXIST.";
            exit(-1);
        }
    } else {
        cout << "ERROR: INVALID FILE NAME.";
        exit(-1);
    }
}

void decryptFlowFile(string& key, string& fn) {
    char char_table[5][5];
    ifstream file;
    cout << "Enter filename: ";
    cin >> fn;
    file.open(fn);
    if (checkFileExtension(fn)) {
        if (file) {
        cout << "Enter key: ";
        cin >> key;
        vector<unsigned char> encrypted_cont = readf(fn);
        createMatrix(key, char_table);
        string str = contFormatting(encrypted_cont);
        decryptString(str, char_table);
        string output_file = "decoded.txt";
        encf(output_file, str);
        cout << "Finished decrypting file contents." << endl;
        } else {
            cout << "ERROR: FILE DOES NOT EXIST.";
            exit(-1);
        }
    } else {
        cout << "ERROR: INVALID FILE NAME.";
        exit(-1);
    }
}


int main() {
    string p, k, fn;
    int choice;
    cout << "1. Encrypt Plaintext\n2. Decrypt Plaintext\n3. Encrypt File Contents\n4. Decrypt File Contents\n Choice: ";
    cin >> choice;
    switch (choice) {
        case 1:
            encryptFlow(p, k);
            break;
        case 2:
            decryptFlow(p, k);
            break;
        case 3: 
            encryptFlowFile(k, fn);
            break;
        case 4:
            decryptFlowFile(k, fn);
            break;
        case 0:
            break;
        default:
            cout << "Invalid Selection" << endl;
            main();
    }
    return 0;
}
