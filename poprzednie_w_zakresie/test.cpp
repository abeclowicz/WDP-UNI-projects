#include <bits/stdc++.h>
#include "prev.h"

std::vector<int>values;

std::mt19937 e;
int rand(int a, int b) {
    return std::uniform_int_distribution<int>{a,b}(e);
}

int main(int argv, char *argc[]) {
    e.seed(std::stoi(argc[1]));
    init({});
    int operations = 2 * rand(100, 2000); // pushBack, prevInRange
    int n = 0;  
    while(operations--) {
        int val = rand(INT_MIN, INT_MAX);
        
        values.push_back(val);
        pushBack(val);

        int i = rand(0, n++);
        int lo = rand(-INT_MAX, INT_MAX), hi = rand(lo, INT_MAX);

        int ans_brut = -1;
        for(int j=i;j>=0;j--) {
            if(values[j] >= lo && values[j] <= hi){
                ans_brut = j;
                break;
            }
        }

        int ans_wzor = prevInRange(i, lo, hi);

        if(ans_brut != ans_wzor) {
            return 0;
            break;
        }
    }
    done();
    return 1;
}


