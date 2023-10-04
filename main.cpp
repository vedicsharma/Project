#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;



struct cacheS{
    // currently in config DIRECTMAPPED
    int BYTE_LINES = 32;
    int offset = 5; //equal to logbase2 of bytelines

    int num_bytes_in_cache = 512; //CHANGE
    int Cache_lines = 16; // equal to numbytes/bytelines CHANGE
    int num_bytes_in_block = 4; //equal to logbase2 of cachelines //CHANGE


    int SetAssNum = 4; //2 for 2waysetassociative and 4 for 4 way CHANGE

    int numberSets = 1; // if direct mapped, then = cachelines. If Fully ass., then = 1; CHANGE
    // If Set ass. then divide cachelines by set ass number
    int line = 0; //logbase2 of numberSets CHANGE

    int replaceStrat = 1; // 0 means FIFO, 1 means LRU CHANGE
};
struct tags{
    int tagValue;
    int counter=1;
};
struct Cacheset{
    int max; //depends on type you are using CHANGE
    int setNumber;
    vector<tags> tagList;
};

cacheS cacheSpecs;
vector<Cacheset> cache;
int calls;
int hits;
int maxhit;

bool insertTag(int set, int tag){
    calls++;
    for (int i = 0; i<cache[set].tagList.size(); i++) {
        int ab = cache[set].tagList[i].tagValue;
        if (ab == tag){
            hits++;
            cache[set].tagList[i].counter = calls;
            return true;
        }
    }

    if(cache[set].tagList.size() < cache[set].max){
        tags a;
        a.tagValue = tag;
        a.counter = calls;
        cache[set].tagList.push_back(a);
        return false;
    }
    else{
        if(cacheSpecs.replaceStrat == 0){
            maxhit++;
            tags a;
            a.tagValue = tag;
            a.counter = calls;
            cache[set].tagList.push_back(a);
            cache[set].tagList.erase(cache[set].tagList.begin());
        }
        else{
            tags a;
            maxhit++;
            a.tagValue = tag;
            a.counter = calls;
            int min = INT_MAX;
            int minpos=0;
            for (int i = 0; i<cache[set].tagList.size(); i++) {
                int val = cache[set].tagList[i].counter;
                if(val<min){
                    //cout << val<<endl;
                    min = val;
                    minpos = i;
                }
            }
            //cout << minpos << endl;
            cache[set].tagList[minpos] = a;
        }


    }
    return false;
}

int convertToDec(string s){
    int num = 0;
    for(int i = 0; i< s.size(); i++){
        string temp = "";
        //cout << temp<<endl;
        if(s.substr(i,1) == "1"){
            int pos = s.size() - i -1;
            //cout <<"pos: " <<pos<<endl;
            int val = pow(2, pos);
            num = num + val;
        }
    }
    return num;
}

string converter(char a){
    string output;
    switch (a) {
        case '0':
            output = "0000";
            break;
        case '1':
            output = "0001";
            break;
        case '2':
            output = "0010";
            break;
        case '3':
            output = "0011";
            break;
        case '4':
            output = "0100";
            break;
        case '5':
            output = "0101";
            break;
        case '6':
            output = "0110";
            break;
        case '7':
            output = "0111";
            break;
        case '8':
            output = "1000";
            break;
        case '9':
            output = "1001";
            break;
        case 'a':
            output = "1010";
            break;
        case 'b':
            output = "1011";
            break;
        case 'c':
            output = "1100";
            break;
        case 'd':
            output = "1101";
            break;
        case 'e':
            output = "1110";
            break;
        case 'f':
            output = "1111";
            break;
    }
    return output;
}

string convertBinary(string input){
    string output = "";
    for(auto c : input){
        string a = converter(c);
        a;
        output.append(a);
    }
    return output;
}

bool edit(int size, int type, int replacement){
    int size2 = pow(2,size);
    //cout << size<<endl;
    cacheSpecs.num_bytes_in_cache = cacheSpecs.num_bytes_in_cache * size2;
    cacheSpecs.Cache_lines = cacheSpecs.Cache_lines* size2;
    //cout<< cacheSpecs.Cache_lines << endl;
    cacheSpecs.num_bytes_in_block = cacheSpecs.num_bytes_in_block + size;
    //cout << cacheSpecs.num_bytes_in_block <<endl;

    //type
    //1 means direct mapped. 2 means 2 way SA. 3 means 4 way SA. 4 means Fully associative
    if(type == 1){
        cacheSpecs.numberSets = cacheSpecs.Cache_lines;
        cacheSpecs.line = cacheSpecs.num_bytes_in_block;
    }
    else if(type == 2){
        cacheSpecs.numberSets = cacheSpecs.Cache_lines/2;
        cacheSpecs.line = cacheSpecs.num_bytes_in_block-1;
    }
    else if(type == 3){
        cacheSpecs.numberSets = cacheSpecs.Cache_lines/4;
        cacheSpecs.line = cacheSpecs.num_bytes_in_block-2;
        //cout << cacheSpecs.line<<endl;
    }
    else if(type == 4){
        cacheSpecs.numberSets = 1;
        cacheSpecs.line = 0;
    }


    //replacement strategy
    //if 0 then FIFO, if 1 then LRU
    if (replacement == 0){
        cacheSpecs.replaceStrat = 0;
    }
    else if (replacement == 1){
        cacheSpecs.replaceStrat = 1;
    }

    return true;
}

vector<int> interpreter(string input){
    vector<int> results;
    string newinput = convertBinary(input);
    int tagsize = cacheSpecs.BYTE_LINES - cacheSpecs.line - cacheSpecs.offset;
    string tag = newinput.substr(0,tagsize);
    string line = newinput.substr(tagsize, cacheSpecs.line);
    //cout << "Tag: " << tag << " Line: " << line << endl;
    int lineNum = convertToDec(line);
    int tagNum = convertToDec(tag);
    //cout << "tagNum: " << lineNum << endl;
    //cout << "tagNum: " << tagNum<<" Line: " << lineNum << endl;
    results.push_back(lineNum);
    results.push_back(tagNum);
    return results;
}

int main() {
    calls = 0;
    hits = 0;
    maxhit = 0;

    //size is multiple of 512
    //type
    //1 means direct mapped. 2 means 2 way SA. 3 means 4 way SA. 4 means Fully associative
    //replacement 0 is FIFO, 1 is LRU
    edit(0, 4, 1);





    for (int i = 0; i<cacheSpecs.numberSets; i++){
        Cacheset temp;
        temp.setNumber = i;
        temp.max = cacheSpecs.Cache_lines/cacheSpecs.numberSets;
        //cout << temp.max<<endl;
        cache.push_back(temp);
    }


    fstream inputFile;

    inputFile.open("/Users/VJS866/Downloads/gxx.txt",ios::in); //open a file to perform read operation using file object
    if (inputFile.is_open()) {   //checking whether the file is open
        string line;
        while (getline(inputFile, line)) { //read data from file object and put it into string.
            string num = line.substr(4,8);
            //cout <<  num << "\n"; //print the data of the string
            //string num2 = convertBinary(num);
            //cout <<  num2 << "\n";
            vector<int> entry = interpreter(num);
            //cout << "Set: " << entry[0]<< " Tag: " << entry[1]<<endl;
            insertTag(entry[0], entry[1]);
        }
        //inputFile.close();

    }
    inputFile.close();

    //int a = maxhit;
    //cout<<a<<endl;

    cout<< hits;
    cout << "/";
    cout << calls <<" = ";
    cout << (float)hits/(float)calls;
    //cout << endl<<maxhit;



    return 0;
}


//Vedic Sharma