#include<iostream>
#include<bits/stdc++.h>

using namespace std;


class bstNode{

public:
    int data;   //docID
    int freq;   //number of times that the word occurs in this doc..
    bstNode* left, *right;

    bstNode()
    {
        data = 0;
        freq = 1;
        left = NULL; right = NULL;
    }

    bstNode(int val)
    {
        freq = 1;
        data = val;
        left = NULL; right = NULL;
    }

};

class BST{

private:
    bstNode* root;

    bstNode* insertNode(bstNode* root, int val)
    {
        if(root == NULL)
            return new bstNode(val);
        else if(val > root->data)
            root->right = insertNode(root->right,val);
        else
            root->left = insertNode(root->left,val);

        return root;
    }

    void inOrder(bstNode* root)
    {
        if(!root)
            return;
        inOrder(root->left);
        cout << root->data << "|" << root->freq << "  ";
        inOrder(root->right);
    }

    bstNode* search(bstNode* root, int key) 
    { 
        if (root == NULL || root->data == key) 
           return root; 
         
        if (root->data < key) 
           return search(root->right, key); 
       
        return search(root->left, key); 
    } 

public:
    BST()
    {
        root = new bstNode;
    }

    void insert(int val)
    {
        if(root->data == 0)
            root->data = val;
        else
            root = insertNode(root, val);
    }

    void printTree()
    {
        inOrder(root);
        cout << endl;
    }

    bstNode* find(int docID)
    {
        return search(root,docID);
    }


};


class TrieNode {

public:

    TrieNode* next[26];
    bool isWordEnd;
    int docFreq;
    BST* postingList;

    TrieNode()
    {
        isWordEnd = false;
        memset(next,0,sizeof(next));
        docFreq = 0;
    }

    void updateWordFreq(string word, int i)
    {
        if((*postingList).find(i))  //if the posting list already has the current doc, update the wordfreq in that doc
        {
            bstNode* docNode;
            docNode = (*postingList).find(i);
            docNode->freq = docNode->freq+1;
        }
        else    //if the posting list doesn't have the current doc, add that doc to the postinglist..
        {
            (*postingList).insert(i);
            docFreq++;
        }
    }

};

class Trie {
private:
    TrieNode* root;

    void traversePrint(TrieNode* root,char* out,int index)
    {
        if(root!=NULL)
        {
            for(int i=0;i<26;i++)
            {
                if(root->next[i]!=NULL)
                {
                    out[index] = 'a'+i;
                    traversePrint(root->next[i],out,index+1);
                }
            }
            if(root->isWordEnd)
            {
                out[index]='\0';
                cout << out << "|" << root->docFreq << " --->  ";
                (*(root->postingList)).printTree();
            }
        }
    }

public:

    Trie() {
        
        root = new TrieNode;
    }

    ~Trie(){
        clear(root);
    }

    void clear(TrieNode *root){
        for(int i = 0; i < 26; i++){
            if(root->next[i] != nullptr){
                clear(root->next[i]);
            }
        }
        delete root;
    }

    void insert(string word, int docID)
    {
        auto temp = root;
        for(int i=0;i<word.size();i++)
        {
            if(temp->next[word[i]-'a'] == NULL)
                temp->next[word[i]-'a'] = new TrieNode;
            temp = temp->next[word[i]-'a'];
        }
        temp->isWordEnd = true;

        temp->docFreq = 1;
        temp->postingList = new BST;
        (*(temp->postingList)).insert(docID);
    }
    
    /** Returns the leaf node, if it is in the trie. */
    TrieNode* search(string word) {

        auto temp = root;
        for(int i=0;i<word.size();i++)
        {
            if(temp->next[word[i]-'a'] == NULL)
                return NULL;
            temp = temp->next[word[i]-'a'];
        }
        
        if(temp->isWordEnd)
            return temp;
        
        return NULL;
    }

    void printWords()
    {
        char out[100];
        traversePrint(root,out,0);
    }
    
};

Trie trie;

bool invalidChar (char c) ;
bool Isquote(char c);

int main()
{

    for(int i=101;i<105;i++)
    {
        string fileName = to_string(i);
        fileName = fileName+".txt";

        //reading the text file..
        //reading the text file line by line..
        cout << "Reading " << fileName << endl;
        ifstream file(fileName);
        string line;
        while (getline(file, line)) {

            //replacing punctuations and non ASCII characters with space in each line (except ')
            replace_if(line.begin() , line.end() ,  
                [] (const char& c) { return (ispunct(c)&&c!=39 || (c>=48 && c<=57));},' ');

            replace_if(line.begin() , line.end() ,  
                [] (const char& c) { return invalidChar(c) ;},' ');

            //removing ' in each line
            line.erase(remove_if(line.begin(),line.end(), Isquote), line.end());

            //extract words from line
            istringstream ss(line); 
            do { 
                // Read a word 
                string word; 
                ss >> word; 
          
                TrieNode* temp; 
                temp = trie.search(word);   //if the word is present, we get the leaf node of the word in the trie..
                if(temp == NULL)
                    trie.insert(word,i);
                else
                    (*temp).updateWordFreq(word,i);
          
            } while (ss); 
        }

    }

    //printing the inverted index..
    trie.printWords();

    return 0;

}


bool invalidChar (char c) 
{  
    return !(c>=0 && c <128);   
}

bool Isquote(char c)
{
    if(c == 39)
        return true;
    else
        return false;
}

