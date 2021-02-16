#include <iostream>
#include <string>
#include <fstream>

using namespace std;

bool encode = false; // 0 For decoding; 1 For encoding
int grill_size[2][1] = {{10},{10}};
int key_size = 10;
string phrase;



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
}

int main(int argc, char ** argv) {
    for (int i = 0; i < argc; i ++) {
        string argvs = argv[i];
        if (argvs == "-h") help();
        encode = (argvs != "-d");
        encode = (argvs == "-e");
        if (argvs == "-s" || argvs == "-size") {
            grill_size[0][0] = stoi(argv[i+1]);
            grill_size[1][0] = stoi(argv[i+2]);
        }
        if (argvs == "-ks"||argvs=="-key-size") key_size = stoi(argv[i+1]);
        if (argvs == "-ph"||argvs=="-phrase") phrase = argv[i+1];
    }
    cout << encode << endl << grill_size[0][0] << "\t" << grill_size[1][0] << endl;
    cout << key_size << endl << phrase << endl;
    return 0;
}
