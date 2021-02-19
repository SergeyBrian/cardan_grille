#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

bool encode = true;// 0 For decoding; 1 For encoding
bool ce = false, force = false;
int grill_size[2][1] = {{0},{0}};
int key_size = 0;
string phrase, filename, ifilename, keyfile;
char alphabet[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

int ** readKey();
void viewKey(int ** Key);

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
    cout << "Params:" << endl;
    cout << "-d\tDecode" << endl;
    cout << "-e\tEncode" << endl;
    cout << "-h\tHelp" << endl << endl;
    cout << "-s/-size <x> <y>\tChoose size of grill's sides" << endl;
    cout << "-ks/-key-size <a>\tChoose size of key's side" << endl;
    cout << "-ph/-phrase <phrase>\tPhrase to encode" << endl;
    cout << "-o <filename>\tFile for output" << endl;
    cout << "-i <filename>\tFile for input" << endl;
    cout << "-k/-key <filename>\tFile with key" << endl;
    cout << "-no-random\tDon't use randomness" << endl;
    cout << "-vk/-view-key\tView any key in all positions" << endl;
    cout << "-ce/-clean-endings\tLeave endings of phrase empty" << endl;
    cout << "-f\tForce rewrite key file (ignore any existing keys)" << endl;
    exit(0);
}

void printKey (int ** Key) {
    for (int i = 0; i < key_size; i ++) {
        for (int j = 0; j < key_size; j++)
            cout << Key[i][j] << "\t";
        cout << endl;
    }
    cout << endl;
}

void getValues(int argc, char ** argv) {
    if (argc > 1) {
        for (int i = 0; i < argc; i++) {
            string argvs = argv[i];
            if (argvs == "-vk" || argvs == "-view-key") {
                keyfile = argv[i+1];
                int ** Key = readKey();
                viewKey(Key);
                exit(0);
            }

            if (argvs == "-h") help();

            if (argvs == "-d") encode = false;
            if (argvs == "-e") encode = true;
            if (argvs == "-s" || argvs == "-size") {
                grill_size[0][0] = stoi(argv[i + 1]);
                grill_size[1][0] = stoi(argv[i + 2]);
            }
            if (argvs == "-ks" || argvs == "-key-size") key_size = stoi(argv[i + 1]);
            if (argvs == "-ph" || argvs == "-phrase") phrase = argv[i + 1];
            if (argvs == "-o") filename = argv[i + 1];
            if (argvs == "-i") ifilename = argv[i + 1];
            if (argvs == "-k" || argvs == "-key") keyfile = argv[i + 1];
            if (argvs == "-no-random") srand(0);
            if (argvs == "-ce" || argvs == "-clean-endings") ce = true;
            if (argvs == "-f") force = true;
        }
    } else help();
}

int checkValues() {
    if (!key_size) {
        int need = phrase.length()/4 + 1;
        int max = 0;
        while (max < need || (key_size%2) || key_size<=3) {
            key_size++;
            max = key_size*key_size/4;
            cout << key_size << ":" << max << endl ;
        }
    }
    if (!grill_size[0][0] || !grill_size[1][0]) {
        grill_size[0][0] = key_size;
        grill_size[1][0] = key_size;
    }
    if (grill_size[0][0] < key_size || grill_size[1][0] < key_size) return 1;
    if (key_size < 3) return 2;
    if (!encode && phrase.length()) return 3;
    if (!key_size % 2) return 4;
    if (!filename.length()) filename = "out.txt";
    if (!keyfile.length()) keyfile = "key.txt";
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

    for (int i = 0; i < key_size; i ++) {
        delete [] Key2[i];
        delete [] Key3[i];
        delete [] Key4[i];
    }
    delete [] Key2;
    delete [] Key3;
    delete [] Key4;


    // Remove temporary data
    for (int i = 0; i < key_size; i ++)
        for (int j = 0; j < key_size; j ++)
            if (Key[i][j] == -1) Key[i][j] = 0;
    cout << "Generated key: " << endl;
    printKey(Key);
    return Key;
}

int ** readKey() {
    fstream key;
    key.open(keyfile, ios::in);
    string ksz;
    getline(key, ksz);
    string noFormatKey;
    getline(key,noFormatKey);
    key_size = stoi(ksz);

    int ** Key = new int * [key_size];
    initKey(Key);

    int cursor = 0;
    for (int i = 0; i < key_size; i ++) {
        for (int j = 0; j < key_size; j ++) {
            Key[i][j] = noFormatKey[cursor] - '0';
            cursor ++;
        }
    }

    printKey(Key);

    return Key;
}

void hideMessage(char ** message, int ** Key, int * c) {
    for (int i = 0; i < key_size; i ++) {
        for (int j = 0; j < key_size; j ++) {
            if (Key[i][j] == 1) {
                if (*c >= phrase.length()) {
                    if (ce) message[i][j] = ' ';
                    else return;
                } else message[i][j] = phrase[*c];
                *c += 1;
            }
        }
    }
}

void findMessage(char ** message, int ** Key, int * c) {
    for (int i = 0; i < key_size; i ++) {
        for (int j = 0; j < key_size; j ++) {
            if (Key[i][j] == 1) {
                phrase[*c] = message[i][j];
                *c += 1;
            }
        }
    }
}

int doEncoding() {
    cout << "Encoding message..." << endl;
    fstream output;
    fstream key;
    ifstream input;

    output.open(filename, ios::out);

    key.open(keyfile, ios::in);

    char ** message = new char * [grill_size[0][0]];

    int ** Key;
    if (!key || force) {
        cout << "No existing key found! Creating a new one" << endl;
        key.close();
        key.open(keyfile, ios::out);
        key.close();
        key.open(keyfile, ios::app);
        Key = generateKey();
        key << key_size << "\n";
        for (int i = 0; i < key_size; i ++)
            for (int j = 0; j < key_size; j ++)
                key << Key[i][j];
        key.close();
    } else {
        cout << "Using key from file " << keyfile << endl;
        Key = readKey();
    }

    for (int i = 0; i < grill_size[0][0]; i ++) {
        message[i] = new char [grill_size[1][0]];
        for (int j = 0; j < grill_size[1][0]; j  ++) {
            message[i][j] = alphabet[rand() % 26];
        }
    }

    int c = 0;
    hideMessage(message, Key, &c);
    flipV(Key);
    hideMessage(message, Key, &c);
    flipH(Key);
    hideMessage(message, Key, &c);
    flipV(Key);
    hideMessage(message, Key, &c);

    for (int i = 0; i < grill_size[0][0]; i ++) {
        for (int j = 0; j < grill_size[0][0]; j++) {
            cout << message[i][j];
            output << message[i][j];
        }
        cout << endl;
        output << endl;
    }
    return 0;
}

int doDecoding() {
    cout << "Decoding message from file " << ifilename << endl;
    cout << "Using key from file " << keyfile << endl;
    int ** Key = readKey();
    char ** message;
    string tmp;
    ifstream msg(ifilename);
    int x = 0, y;
    while (getline(msg, tmp)) {
        x++;
        y = tmp.length();
    }

    msg.clear();
    msg.seekg(0);
    char c;
    message = new char * [x];
    for (int i = 0; i < x; i ++) {
        message[i] = new char [y];
        for (int j = 0; j < y; j ++) {
            msg >> noskipws >> c;
            if (c != '\n') message[i][j] = c;
            else j--;
            cout << c;
        }
//        cout << endl;
    }
    phrase = string(key_size*key_size, ' ');
    int cursor = 0;


    findMessage(message, Key, &cursor);
    flipV(Key);
    findMessage(message, Key, &cursor);
    flipH(Key);
    findMessage(message, Key, &cursor);
    flipV(Key);
    findMessage(message, Key, &cursor);

    cout << endl << phrase;

    return 0;
}

void viewKey (int ** Key) {
    printKey(Key);
    flipV(Key);
    printKey(Key);
    flipH(Key);
    printKey(Key);
    flipV(Key);
    printKey(Key);
}

int main(int argc, char ** argv) {
    srand(time(nullptr));
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
    cout<<endl;
    return 0;
}
