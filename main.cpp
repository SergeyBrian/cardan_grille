#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

bool encode = false; // 0 For decoding; 1 For encoding
int grill_size[2][1] = {{10},{10}};
int key_size = 10;
string phrase, filename, ifilename, keyfile;
char alphabet[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
void initKey (int ** Key) {
    for (int i = 0 ; i < key_size ; i ++) Key[i]=new int[key_size];
}

void initKey (int ** Key, int fill) {
    for (int i = 0 ; i < key_size ; i ++) {
        Key[i]=new int[key_size];
        for (int j = 0; j < key_size; j ++) Key[i][j] = fill;
    }
}

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
    cout << "-k/-key <filename>\tFile with key" << endl;
}

void printKey (int ** Key) {
    for (int i = 0; i < key_size; i ++) {
        for (int j = 0; j < key_size; j++)
            cout << Key[i][j] << "\t";
        cout << endl;
    }
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
        cout << "Enter filename for keyfile: ";
        cin >> keyfile;
    }
}

int checkValues() {
    if (grill_size[0][0] < key_size || grill_size[1][0] < key_size) return 1;
    if (key_size < 3) return 2;
    if (!encode && phrase.length()) return 3;
    if (!key_size % 2) return 4;
    return 0;
}

int flipV (int ** Key) {
    for (int i = 0; i < key_size; i ++) {
        int start = 0, end = key_size-1;
        while (start < end) {
            int tmp = Key[i][start];
            Key[i][start] = Key[i][end];
            Key[i][end] = tmp;
            start ++;
            end --;
        }
    }
    return 0;
}

int flipH (int ** Key) {
    int ** tmp = new int * [key_size];
    for (int i = 0; i < key_size; i ++) {
        tmp[i] = new int[key_size];
        for (int j = 0; j < key_size; j ++) {
            tmp[i][j] = Key[j][i];
        }
    }
    flipV(tmp);
    for (int i = 0; i < key_size; i ++) {
        for (int j = 0; j < key_size; j ++) {
            Key[i][j] = tmp[j][i];
        }
    }

    for (int i = 0; i < key_size; i ++)
        delete [] tmp[i];
    delete [] tmp;
    return 0;
}

void eq (int ** KeyA, int ** KeyB) {
    for (int i = 0 ; i < key_size ; i ++)
        for (int j = 0; j < key_size ; j ++) KeyA[i][j] = KeyB[i][j];
}

int ** generateKey() {
    cout << "Generating new key with size " << key_size << " for phrase length " << phrase.length() << endl;
    int neededCutouts = (phrase.length() / 4) + 1;
    int maxCutOuts = key_size*key_size/4;
    cout << endl << "Needed cutouts: " << neededCutouts << endl;
    cout << endl << "Max cutouts: " << maxCutOuts << endl;

    if (neededCutouts > maxCutOuts) {
        cout << "Error! Your message is too long for given key size!" << endl;
        cout << "Recommended key size for this message: " << neededCutouts * 4 / key_size << endl;
        exit(1);
    }
    int ** Key = new int * [key_size];
    int ** Key2 = new int * [key_size];
    int ** Key3 = new int * [key_size];
    int ** Key4 = new int * [key_size];

    initKey(Key, 0);
    initKey(Key2);
    initKey(Key3);
    initKey(Key4);

    int numberOfCutouts = 0;
    while (neededCutouts > numberOfCutouts) {
        // Generate random coords
        int x = rand() % key_size;
        int y = rand() % key_size;
        // Check if coords are empty
        if (Key[x][y] == 0) {
            numberOfCutouts ++;
            cout << endl << numberOfCutouts << endl;

            Key[x][y] = 1;
            // Get all forbidden coords

            eq(Key2, Key);
            eq(Key3, Key);
            eq(Key4, Key);

            flipV(Key2);
            flipV(Key3);
            flipV(Key4);

            flipH(Key3);
            flipH(Key4);

            flipV(Key4);


            // Write forbidden coords to the key
            for (int i = 0; i < key_size; i ++)
                for (int j = 0; j < key_size; j ++) {
                    if (Key2[i][j] == 1) Key[i][j] = -1;
                    if (Key3[i][j] == 1) Key[i][j] = -1;
                    if (Key4[i][j] == 1) Key[i][j] = -1;
                }
        }
    }

    cout << "Generated key: ";
    printKey(Key);
    return Key;
}

//int ** readKey(fstream key) {
//    int ** Key;
//    // Read key
//    return Key;
//}

int doEncoding() {
    cout << "Encoding message..." << endl;
    fstream output;
    fstream key;
    ifstream input;

    output.open(filename, ios::out);

    key.open(keyfile, ios::in);

    if (!key) {
        cout << "No existing key found! Creating a new one" << endl;
        key.close();
        key.open(keyfile, ios::app);
        int ** Key = generateKey();
        key << key_size << "\n";
        for (int i = 0; i < key_size; i ++)
            for (int j = 0; j < key_size; j ++)
                key << Key[i][j];
    } else {
//        int ** Key = readKey(key);
    }

    return 0;
}

int doDecoding() {
//    int ** key = readKey();
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
    cout << "Key filename: " << keyfile << endl;
    if (encode) doEncoding();
    else doDecoding();
    return 0;
}
