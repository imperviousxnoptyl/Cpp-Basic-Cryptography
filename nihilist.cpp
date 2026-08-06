#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

//The Idea:
// > First key with its letters put into the 5x5 matrix, with no repetition
// > Second key, converted to strings of numbers, two for each letter from the 5x5 matrix
// > The plaintext, first goes through the matrix then performs addition with the second key
// This is basically Playfair + Polybius, but without the character wrapping rules

void createNihilistMatrix(string& keyword, char char_table[5][5]) {
    vector<bool> isFilled(26, false);
    int r = 0, c = 0;
    for (char& ch : keyword) {
        if (isalpha(ch)) {
            ch = toupper(ch);
            if (ch == 'J') ch = 'I';
        }
    }
    for (char ch : keyword) {
        if (!isFilled[ch - 'A']) {
            isFilled[ch - 'A'] = true;
            char_table[r][c++] = ch;
            if (c == 5) {
                c = 0;
                r++;
            }
        }
    }
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (!isFilled[ch - 'A']) {
            isFilled[ch - 'A'] = true;
            char_table[r][c++] = ch;
            if (c == 5) {
                c = 0;
                r++;
            }
        }
    }
}

vector<int> polybiusOperation(string& str, char char_table[5][5]) {
    vector<int> str_to_nums;
    int r, c;
    for (char& ch : str) {
        if (isalpha(ch)) {
            ch = toupper(ch);
            if (ch == 'J') ch = 'I';
        }
    }
    for (int i = 0; str[i]; i++) {
        for (r = 0; r < 5; r++) {
            for (c = 0; c < 5; c++) {
                if (str[i] == char_table[r][c]) {
                    str_to_nums.push_back(((r + 1) * 10) + (c + 1));
                }
            }
        }
    }
    return str_to_nums;
}

string nihilistEncryptStr(string& str, string& key, char char_table[5][5]) {
    vector<int> new_str = polybiusOperation(str, char_table);
    vector<int> new_key = polybiusOperation(key, char_table);
    ostringstream encrypted_text;
    for (size_t i = 0; i < new_str.size(); i++) {
        int encrypted = new_str[i] + new_key[i % new_key.size()];
        if (i > 0) {
            encrypted_text << " ";
        }
        encrypted_text << encrypted;
    }
    return encrypted_text.str();
}

//encrypted string -> vector of integers -> decrypt one by one -> convert back to string
string nihilistDecryptStr(string& str, string& key, char char_table[5][5]) {
    vector<int> encrypted_str;
    string var;
    stringstream ss(str);
    vector<int>new_key = polybiusOperation(key, char_table);
    string decrypted;
    for (size_t i = 0; i < str.length(); i++) {
        if (isdigit(str[i])) {
            while (ss >> var) {
                int r = (var[0] - '0');
                int c = (var[1] - '0');
                int l = (r * 10) + c; //letter of string in original table
                encrypted_str.push_back(l);
            }
        }
    }
    for (int i = 0; i < (int)encrypted_str.size(); i++) {
        int toclean = encrypted_str[i] - new_key[i % new_key.size()];
        int r = toclean / 10 - 1; //map original letter with row and column number (e.g. with normal polybius square grid A-Z, r=4 c=4 corresponds to the letter t)
        int c = toclean % 10 - 1;
        decrypted.push_back(char_table[r][c]);
    }
    return decrypted;
}

bool checkFileExtension(const string& filename) {
    vector<string> validFileExtensions = {".txt", ".rtf", ".doc", ".docx", ".csv"};
    for (const string& file_ext : validFileExtensions) {
        if (filename.find(file_ext) != string::npos) {
            return true;
        }
    }
    return false;
}

vector<unsigned char> readf(const string& file_name) {
    ifstream file(file_name, ios::binary);
    vector<unsigned char> tmp((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
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

string nihilistEncryptCont(vector<unsigned char>& str, string& key, char char_table[5][5]) {
    string cont = "";
    for (char ch : str) {
        cont += ch;
    }
    vector<int> new_str = polybiusOperation(cont, char_table);
    vector<int> new_key = polybiusOperation(key, char_table);
    ostringstream encrypted_text;
    for (size_t i = 0; i < new_str.size(); i++) {
        int encrypted = new_str[i] + new_key[i % new_key.size()];
        if (i > 0) {
            encrypted_text << " ";
        }
        encrypted_text << encrypted;
    }
    return encrypted_text.str();
}

string nihilistDecryptCont(vector<unsigned char>& str, string& key, char char_table[5][5]) {
    string cont = "";
    for (char ch : str) {
        cont += ch;
    }
    vector<int> encrypted_str;
    string var;
    stringstream ss(cont);
    vector<int>new_key = polybiusOperation(key, char_table);
    string decrypted;
    for (size_t i = 0; i < cont.length(); i++) {
        if (isdigit(cont[i])) {
            while (ss >> var) {
                int r = (var[0] - '0');
                int c = (var[1] - '0');
                int l = (r * 10) + c;
                encrypted_str.push_back(l);
            }
        }
    }
    for (int i = 0; i < (int)encrypted_str.size(); i++) {
        int toclean = encrypted_str[i] - new_key[i % new_key.size()];
        int r = toclean / 10 - 1;
        int c = toclean % 10 - 1;
        decrypted.push_back(char_table[r][c]);
    }
    return decrypted;
}


int main() {
    int choice;
    char char_table[5][5];
    string keyword, str, key;
    cout << "Enter keyword: ";
    getline(cin, keyword);
    cout << "Enter string: ";
    getline(cin, str);
    cout << "Enter key: ";
    getline(cin, key);
    cout << "Choose: \n 1) Encrypt String\n 2) Encrypt Text in File\n 3) Decrypt String\n 4) Decrypt Contents in File\n";
    cin >> choice;
    switch(choice) {
        case 1:
            createNihilistMatrix(keyword, char_table);
            polybiusOperation(str, char_table);
            cout << nihilistEncryptStr(str, key, char_table);
            break;
        case 2:
            createNihilistMatrix(keyword, char_table);
            polybiusOperation(str, char_table);
            cout << nihilistDecryptStr(str, key, char_table);
            break;
        case 3:
            createNihilistMatrix(keyword, char_table);
            polybiusOperation(str, char_table);
            //unfinished
            break;
        case 4:
            createNihilistMatrix(keyword, char_table);
            polybiusOperation(str, char_table);
            //unfinished
            break;
        default:
            cout << "Invalid Selection" << endl;
            main();
    }
    return 0;
}
