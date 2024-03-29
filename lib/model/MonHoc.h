#ifndef MONHOC_H
#define MONHOC_H

#include <string>

using std::cout;
using std::endl;
using std::string;
using std::to_string;

struct MonHoc {
    string ms;
    string ten;
    int sltclt;
    int sltcth;

    MonHoc() {
        ms = "";
        ten = "";
        sltclt = 0;
        sltcth = 0;
    }

    MonHoc(string ms, string ten, int sltclt, int sltcth) {
        this->ms = ms;
        this->ten = ten;
        this->sltclt = sltclt;
        this->sltcth = sltcth;
    }

    string toString() {
        return ms + "|" + ten + "|" + to_string(sltclt) + "|" +
               to_string(sltcth);
    }
};

struct NodeMonHoc {
    MonHoc monhoc;
    NodeMonHoc *left;
    NodeMonHoc *right;

    NodeMonHoc() {
        left = NULL;
        right = NULL;
    }
};

#endif
