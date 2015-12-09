//
//  SuffixTreeBuilder.cpp
//  suffix_array
//
//  Created by Даня on 09.12.15.
//  Copyright © 2015 mipt. All rights reserved.
//

#include "SuffixArrayBuilder.hpp"

int SuffixArrayBuilder::leq(int c00, int c10, int c01, int c11) {
    if (c00 == c10 && c01 == c11) {
        return 0;
    }
    if (c00 < c10 || (c00 == c10 && c01 < c11)){
        return -1;
    }
    return 1;
}

vector<int> SuffixArrayBuilder::radixPass(vector<int> &toSort, vector<int> &keys, int shift) {
    vector<int> counter(alphabetSize, 0);
    vector<int> sorted(toSort.size());
    for (int i = 0; i < toSort.size(); ++i) {
        counter[keys[toSort[i] + shift]]++;
    }
    int curPos = 0;
    for (int i = 0; i < alphabetSize; ++i) {
        int curAdd = counter[i];
        counter[i] = curPos;
        curPos += curAdd;
    }
    for (int i = 0; i < toSort.size(); ++i) {
        sorted[counter[keys[toSort[i] + shift]]++] = toSort[i];
    }
    return sorted;
}

void SuffixArrayBuilder::buildSuffixArray() {
    bool extraEnding = false;
    int n = str.size();
    int n0 = (n + 2) / 3, n2 = n / 3;
    
    str.push_back(0);
    str.push_back(0);
    if ((n % 3) == 1) {
        str.push_back(0);
        extraEnding = true;
        n++;
    }
    n += 2;
    vector<int> triples;
    for (int i = 0; i < n - 2; ++i) {
        if (!!(i % 3)) {
            triples.push_back(i);
        }
    }
    
    vector<int> radixSort(n0 + n2), sort12(n0 + n2), unmodifiedSA12(n0 + n2), SA12(n0 + n2), SA0(n0), reversedSA12(n);
    
    radixSort = radixPass(triples, str, 2);
    radixSort = radixPass(radixSort, str, 1);
    radixSort = radixPass(radixSort, str, 0);
    
    int currentTriplesSize = 0;
    int c0, c1, c2;
    c0 = c1 = c2 = -1;
    
    for (int i = 0; i < n0 + n2; ++i) {
        if (str[radixSort[i]] != c0 || str[radixSort[i] + 1] != c1 || str[radixSort[i] + 2] != c2) {
            c0 = str[radixSort[i]];
            c1 = str[radixSort[i] + 1];
            c2 = str[radixSort[i] + 2];
            currentTriplesSize++;
        }
        if (radixSort[i] % 3 == 1) {
            sort12[radixSort[i] / 3] = currentTriplesSize;
        } else {
            sort12[radixSort[i] / 3 + n0] = currentTriplesSize;
        }
    }
    
    if (currentTriplesSize < n0 + n2) {
        SuffixArrayBuilder nextBuilder = SuffixArrayBuilder(sort12, currentTriplesSize + 1);
        radixSort = nextBuilder.getSuffixArray();
    } else {
        for (int i = 0; i < n0 + n2; ++i) {
            radixSort[sort12[i] - 1] = i;
        }
    }
    
    for (int i = 0; i < n0 + n2; ++i) {
        if (radixSort[i] < n0) {
            SA12[i] = 1 + 3 * radixSort[i];
        } else {
            SA12[i] = 2 + 3 * (radixSort[i] - n0);
        }
    }
    
    int j = 0;
    for (int i = 0; i < n0 + n2; ++i) {
        if ((SA12[i] % 3) == 1) {
            SA0[j++] = SA12[i] - 1;
        }
    }
    
    SA0 = radixPass(SA0, str, 0);
    
    int indexSA12 = 0, indexSA0 = 0;
    
    for (int i = 0; i < n0 + n2; ++i) {
        reversedSA12[SA12[i]] = i;
    }
    
    int indexSA = 0;
    if (extraEnding) {
        indexSA12++;
    }
    while (indexSA12 < n0 + n2 && indexSA0 < n0) {
        if ((SA12[indexSA12] % 3) == 1) {
            int currentLeq = leq(str[SA0[indexSA0]], str[SA12[indexSA12]]);
            if (currentLeq == -1 ||
                (currentLeq == 0 && reversedSA12[SA0[indexSA0] + 1] < reversedSA12[SA12[indexSA12] + 1])) {
                suffixArray[indexSA++] = SA0[indexSA0++];
            } else {
                suffixArray[indexSA++] = SA12[indexSA12++];
            }
        } else {
            int currentLeq = leq(str[SA0[indexSA0]], str[SA12[indexSA12]], str[SA0[indexSA0] + 1], str[SA12[indexSA12] + 1]);
            if (currentLeq == -1 || (currentLeq == 0 && reversedSA12[SA0[indexSA0] + 2] < reversedSA12[SA12[indexSA12] + 2])) {
                suffixArray[indexSA++] = SA0[indexSA0++];
            } else {
                suffixArray[indexSA++] = SA12[indexSA12++];
            }
        }
    }
    
    if (indexSA12 == n0 + n2) {
        while (indexSA0 < n0) {
            suffixArray[indexSA++] = SA0[indexSA0++];
        }
    } else {
        while (indexSA12 < n0 + n2) {
            suffixArray[indexSA++] = SA12[indexSA12++];
        }
    }
    
    str.pop_back();
    str.pop_back();
    if (extraEnding) {
        str.pop_back();
    }
}

void SuffixArrayBuilder::buildLCP() {
    int n = str.size();
    vector<int> reversedSA(n);
    for (int i = 0; i < n; ++i) {
        reversedSA[suffixArray[i]] = i;
    }
    for (int i = 0, k = 0; i < n; ++i) {
        if (k > 0) {
            k--;
        }
        if (reversedSA[i] == n - 1) {
            lcp[n - 1] = -1;
            k = 0;
            continue;
        }
        int j = suffixArray[reversedSA[i] + 1];
        while (max(i + k, j + k) < str.size() && str[i + k] == str[j + k]) {
            k++;
        }
        lcp[reversedSA[i]] = k;
    }
}

SuffixArrayBuilder::SuffixArrayBuilder() {}
SuffixArrayBuilder::~SuffixArrayBuilder() {}


SuffixArrayBuilder::SuffixArrayBuilder(const string &s, char leastChar, int newAlphabetSize): alphabetSize(newAlphabetSize) {
    str.resize(s.length());
    for (int i = 0; i < s.length(); ++i) {
        str[i] = s[i] - leastChar + 1;
    }
    suffixArray.resize(str.size());
    isBuiltSA = false;
    isBuiltLCP = false;
}

SuffixArrayBuilder::SuffixArrayBuilder(const vector <int> &strArr, int newAlphabetSize): alphabetSize(newAlphabetSize) {
    str.resize(strArr.size());
    for (int i = 0; i < strArr.size(); ++i) {
        str[i] = strArr[i];
    }
    suffixArray.resize(str.size());
    isBuiltSA = false;
    isBuiltLCP = false;
}

void SuffixArrayBuilder::setArr(const vector <int> &strArr, int newAlphabetSize) {
    str.resize(strArr.size());
    for (int i = 0; i < strArr.size(); ++i) {
        str[i] = strArr[i];
    }
    alphabetSize = newAlphabetSize;
    suffixArray.resize(str.size());
    isBuiltSA = false;
    isBuiltLCP = false;
}

vector <int> SuffixArrayBuilder::getSuffixArray() {
    if (!isBuiltSA) {
        buildSuffixArray();
        isBuiltSA = true;
    }
    return suffixArray;
}

vector <int> SuffixArrayBuilder::getLCP() {
    if (!isBuiltSA) {
        buildSuffixArray();
        isBuiltSA = true;
    }
    if (!isBuiltLCP) {
        lcp.resize(str.size());
        buildLCP();
        isBuiltLCP = true;
    }
    return lcp;
}


