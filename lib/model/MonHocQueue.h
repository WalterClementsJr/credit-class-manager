#ifndef MH_QUEUE_H
#define MH_QUEUE_H

#include <iostream>
#include <string>

#include "model/DsMonHoc.h"
#include "model/MonHoc.h"
#include "util/import.h"

using std::cout;
using std::endl;
using std::string;

class MonHocQueue {
private:
    static const int MAX_SIZE = 1024;
    NodeMonHoc *list[MAX_SIZE];
    int mFront, mRear;

public:
    MonHocQueue() {
        mFront = -1;
        mRear = -1;
    }

    bool isEmpty() {
        return (mFront == -1 && mRear == -1);
    }

    bool isFull() {
        return (mRear + 1) % MAX_SIZE == mFront ? true : false;
    }

    void enqueue(NodeMonHoc *m) {
        if (isFull()) {
            return;
        }
        if (isEmpty()) {
            mFront = mRear = 0;
        } else {
            mRear = (mRear + 1) % MAX_SIZE;
        }
        list[mRear] = m;
    }

    void dequeue() {
        if (isEmpty()) {
            return;
        } else if (mFront == mRear) {
            mRear = mFront = -1;
        } else {
            mFront = (mFront + 1) % MAX_SIZE;
        }
    }

    NodeMonHoc *front() {
        if (mFront == -1) {
            return NULL;
        }
        return list[mFront];
    }
};

#endif
