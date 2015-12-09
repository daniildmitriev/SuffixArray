//
//  SuffixTreeBuilder.hpp
//  suffix_array
//
//  Created by Даня on 09.12.15.
//  Copyright © 2015 mipt. All rights reserved.
//

#ifndef SuffixTreeBuilder_hpp
#define SuffixTreeBuilder_hpp

#include <stdio.h>
#include <vector>
#include <string>


using namespace std;

class SuffixArrayBuilder {
    
    vector <int> str;
    vector <int> suffixArray;
    vector <int> lcp;
    bool isBuiltSA, isBuiltLCP;
    int alphabetSize;
    
    vector<int> radixPass(vector<int> &toSort, vector<int> &keys, int shift);
    
    int leq(int c00, int c10, int c01 = 0, int c11 = 0);
    
    void buildSuffixArray();
    
    void buildLCP();
    
public:
    
    SuffixArrayBuilder();
    ~SuffixArrayBuilder();
    
    SuffixArrayBuilder(const string &s, char leastChar, int newAlphabetSize);
    SuffixArrayBuilder(const vector <int> &strArr, int newAlphabetSize);
    void setArr(const vector <int> &strArr, int newAlphabetSize);
    vector <int> getSuffixArray();
    vector <int> getLCP();
    
};

#endif /* SuffixTreeBuilder_hpp */