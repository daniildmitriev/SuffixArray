//
//  main.cpp
//  suffix_array
//
//  Created by Даня on 16.11.15.
//  Copyright (c) 2015 mipt. All rights reserved.
//

#include <iostream>
#include "SuffixTreeBuilder.hpp"

void timusSolution() {
    int k;
    string s;
    cin >> k;
    cin >> s;
    s += s;
    vector <int> arr(s.length()), ans(s.length());
    vector <int> curarr(k + 1), sa(k), lcparr(k);
    
    SuffixArrayBuilder SABuilder = SuffixArrayBuilder();
    
    for (int i = 0; i < s.length(); ++i) {
        arr[i] = s[i] - 'a' + 1;
    }
    
    for (int i = 0; i < s.length() / 2; ++i) {
        for (int j = 0; j < k; ++j) {
            curarr[j] = arr[i + j];
        }
        curarr[k] = 0;
        SABuilder.setArr(curarr, 30);
        lcparr = SABuilder.getLCP();
        int sum = 0;
        for (int j = 0; j < k; ++j) {
            sum += lcparr[j];
        }
        cout << (k * (k + 1) / 2) - sum << " ";
    }
}



int main() {
    timusSolution();
    return 0;
}
