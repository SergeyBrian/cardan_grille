#include <iostream>
#include <string>
#include <fstream>

using namespace std;

bool encode = false; // 0 For decoding; 1 For encoding
int grill_size[2][1] = {{10},{10}};
int key_size = 10;
string phrase, filename, ifilename;
char alphabet[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};



void help () {
    cout << "Cardan grille encoder/decoder" << endl;
    cout << endl << "Use syntax <action> [params] <filename>" << endl;
    cout << "Actions:" << endl;
    cout << "-d\tDecode" << endl;
    cout << "-e\tEncode" << endl;
    cout << "-h\tHelp" << endl << endl;
    cout << "Params:" << endl;
    cout << "-s/-size <x> <y>\tChoose size of grill's sides" << endl;
    cout << "-ks/-key-size <a>\tChoose size of key's side" << endl;
    cout << "-ph/-phrase <phrase>\tInput phrase to encode manually (only for encoding)" << endl;
    cout << "-f <filename>\tFile for output" << endl;
    cout << "-if <filename>\tFile for input" << endl;
}

void getValues(int argc, char ** argv) {
    if (argc > 1) {
        for (int i = 0; i < argc; i ++) {
            string argvs = argv[i];
            if (argc == i-1) filename = argvs;

            if (argvs == "-h") help();
            if (argvs == "-d") encode = false;
            if (argvs == "-e") encode = true;
            if (argvs == "-s" || argvs == "-size") {
                grill_size[0][0] = stoi(argv[i+1]);
                grill_size[1][0] = stoi(argv[i+2]);
            }
            if (argvs == "-ks"||argvs=="-key-size") key_size = stoi(argv[i+1]);
            if (argvs == "-ph"||argvs=="-phrase") phrase = argv[i+1];
            if (argvs == "-f") filename = argv[i+1];
            if (argvs == "-if") ifilename = argv[i+1];
        }
    } else {
        cout << "Do encoding or decoding? [1 for decoding, 2 for encoding]: ";
        int de;
        cin >> de;
        encode = de-1;
        cout << "Enter grill sides (x and y): ";
        cin >> grill_size[0][0] >> grill_size[1][0];
        cout << "Enter key side: ";
        cin >> key_size;
        if (encode) {
            cout << "Enter phrase to encode (optional):";
            cin >> phrase;
        }
        cout << "Enter input filename: ";
        cin >> ifilename;
        cout << "Enter output filename: ";
        cin >> filename;
    }
}

int checkValues() {
    if (grill_size[0][0] < key_size || grill_size[1][0] < key_size) return 1;
    if (key_size < 3) return 2;
    if (!encode && phrase.length()) return 3;
    return 0;
}

int doEncoding() {
    return 0;
}

int doDecoding() {
    return 0;
}

int main(int argc, char ** argv) {
    getValues(argc, argv);
    if (checkValues()) {
        cout << checkValues();
        return checkValues();
    }
    cout << "Doing " << ((encode) ? "encoding" : "decoding");
    cout << " with params:" << endl;
    cout << "Grill size: " << grill_size[0][0] << "\t" << grill_size[1][0] << endl;
    cout << "Key size: " << key_size << endl;
    cout << "Phrase: " << phrase << endl;
    cout << "Output filename: " << filename << endl;
    cout << "Input filename: " << ifilename << endl;
    if (encode) doEncoding();
    else doDecoding();
    return 0;
}
