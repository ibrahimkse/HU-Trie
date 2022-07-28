#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

const int ALPHABET_SIZE = 26;

struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];

    bool wordOver;
};

struct TrieNode* getNode()
{
    struct TrieNode* node = new TrieNode;

    node->wordOver = false;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = nullptr;

    return node;
}

void insert(struct TrieNode* root, string key)
{
    struct TrieNode* start = root;

    for (int i = 0; i < key.length(); i++) {
        int index = key[i] - 'a';
        if (!start->children[index])
            start->children[index] = getNode();

        start = start->children[index];
    }


    start->wordOver = true;
}

bool search(struct TrieNode* root, string key)
{
    struct TrieNode* start = root;

    for (int i = 0; i < key.length(); i++) {
        int index = key[i] - 'a';
        if (!start->children[index])
            return false;

        start = start->children[index];
    }

    return (start != nullptr && start->wordOver);
}

bool isEmpty(TrieNode* root)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i])
            return false;
    return true;
}

TrieNode* remove(TrieNode* root, string key, int depth = 0)
{
    if (!root)
        return nullptr;

    if (depth == key.size()) {

        if (root->wordOver)
            root->wordOver = false;

        if (isEmpty(root)) {
            delete (root);
            root = nullptr;
        }

        return root;
    }


    int index = key[depth] - 'a';
    root->children[index] = remove(root->children[index], key, depth + 1);


    if (isEmpty(root) && !root->wordOver) {
        delete (root);
        root = nullptr;
    }

    return root;
}



int main(int argc, char *argv[]) {

    // Taking inputs
    string inputString = argv[1];
    ifstream inputFile(inputString);
    string outputString = argv[2];
    ofstream outputFile(outputString);

    vector<string> inputVector;

    //taking content of file
    string line;
    while (getline (inputFile, line)) {
        inputVector.push_back(line);
    }
    inputFile.close();


    vector<string> myKeys;
    vector<vector<string>> keyValue;
    struct TrieNode* myRoot = getNode();
    // Main Operation
    for (string commandLine: inputVector) {
        if (commandLine.rfind("i", 0) == 0){
            int pos = commandLine.find(",");
            string key = commandLine.substr(7,pos-7);
            string value = commandLine.substr(pos + 1, commandLine.length()-pos-2);
            vector<string> inner;
            if (!myKeys.empty() && find(myKeys.begin(), myKeys.end(), key) != myKeys.end()){
                for (vector<string> v: keyValue) {
                    if (v[0] == key){
                        if (v[1] == value){
                            outputFile <<  "\"" << key << "\" already exist" << endl;
                        }
                        else {
                            keyValue.erase(remove(keyValue.begin(), keyValue.end(), v), keyValue.end());
                            inner.push_back(key);
                            inner.push_back(value);
                            keyValue.push_back(inner);
                            outputFile <<  "\"" << key << "\" was updated" << endl;
                            break;
                        }
                    }
                }
            }
            else {
                myKeys.push_back(key);
                inner.push_back(key);
                inner.push_back(value);
                keyValue.push_back(inner);
                insert(myRoot,key);
                outputFile <<  "\"" << key << "\" was added" << endl;
            }
        }
        else if (commandLine.rfind("s", 0) == 0){
            string toSearch = commandLine.substr(7,commandLine.length()-8);
            if (search(myRoot, toSearch)){
                for (vector<string> v: keyValue) {
                    if (toSearch == v[0]){
                        outputFile << "\"The English equivalent is " << v[1] << "\"" << endl;
                    }
                }
            }
            else {
                bool isFound = false;
                bool notEnough = false;
                for (string s: myKeys) {
                    if (s.rfind(toSearch, 0) == 0){
                        isFound = true;
                        notEnough = true;
                        outputFile << "\"not enough Dothraki word\"" << endl;
                        break;
                    }
                }
                if (!notEnough){
                    for (string s: myKeys) {
                        if (s.rfind(toSearch.substr(0,1), 0) == 0){
                            isFound = true;
                            outputFile << "\"incorrect Dothraki word\"" << endl;
                            break;
                        }
                    }
                }
                if (!isFound){
                    outputFile << "\"no record\"" << endl;
                }
            }
        }
        else if (commandLine.rfind("l", 0) == 0){
            sort(myKeys.begin(), myKeys.end());
            for (string k: myKeys) {
                for (vector<string> v: keyValue) {
                    if (k == v[0]){
                        outputFile << "-" << v[0] << "(" << v[1] << ")" << endl;
                    }
                }
            }
        }
        else if (commandLine.rfind("d", 0) == 0){
            string toDelete = commandLine.substr(7,commandLine.length()-8);
            if (search(myRoot, toDelete)){
                remove(myRoot, toDelete);
                myKeys.erase(remove(myKeys.begin(), myKeys.end(), toDelete), myKeys.end());
                outputFile << "\"" << toDelete << "\" deletion is successful" << endl;
            }
            else {
                bool isFound = false;
                bool notEnough = false;
                for (string s: myKeys) {
                    if (s.rfind(toDelete, 0) == 0){
                        isFound = true;
                        notEnough = true;
                        outputFile << "\"not enough Dothraki word\"" << endl;
                        break;
                    }
                }
                if (!notEnough){
                    for (string s: myKeys) {
                        if (s.rfind(toDelete.substr(0,1), 0) == 0){
                            isFound = true;
                            outputFile << "\"incorrect Dothraki word\"" << endl;
                            break;
                        }
                    }
                }
                if (!isFound){
                    outputFile << "\"no record\"" << endl;
                }
            }
        }
    }
    return 0;
}
