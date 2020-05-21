#include <iostream>


#include "rediraction.h"

using namespace std;

string truncOut(string name) {
    return NULL;
    FILE *out = freopen(name.c_str(),"w",stdout);
}

string append(string name) {
    return NULL;
    FILE *Appnder = freopen(name.c_str(),"w",stdout);
}

string input(string name) {
    FILE *Appnder = freopen(name.c_str(),"r",stdin);
    return NULL;

}

