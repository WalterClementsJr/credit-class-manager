#ifndef DSMH_H
#define DSMH_H

#include <iostream>
#include <string>

#include "Helper.h"
#include "MonHoc.h"

using namespace std;

class DsMonHoc {
   public:
    DsMonHoc();
    ~DsMonHoc();

    void insert(string ms, string ten, int sltclt, int sltcth);
    void remove(string ms);
    void update(MonHoc mh);
    void read();
    void write();
    bool isEmpty();
    int getCount();
    void toArray(MonHoc *arr[]);

    // debugging
    void displayPostOrder();
    void displayInOrder();

   private:
    NodeMonHoc *root;
    int getHeight(NodeMonHoc *root);
    int getCount(NodeMonHoc *root);
    bool isBalanced(NodeMonHoc *root);
    NodeMonHoc *emptyTree(NodeMonHoc *root);
    NodeMonHoc *search(NodeMonHoc *root, string cmnd);
    void traverseInOrder(NodeMonHoc *root);
    void traversePostOrder(NodeMonHoc *root);
    NodeMonHoc *insertNode(NodeMonHoc *root, MonHoc &m);
    // NodeMonHoc *updateNode(NodeMonHoc *root, MonHoc &m);
    // NodeMonHoc *removeNode(NodeMonHoc *root, string ms);
    // NodeMonHoc *readFromFile(NodeMonHoc *root, ifstream *reader);
    // void writeToFile(NodeMonHoc *root, ofstream *writer);
    void addNodeToArray(NodeMonHoc *root, MonHoc *arr[], int &index);
};

// public
DsMonHoc::DsMonHoc() { root = nullptr; }

DsMonHoc::~DsMonHoc() { root = emptyTree(root); }

void DsMonHoc::insert(string ms, string ten, int sltclt, int sltcth) {
    MonHoc m(ms, ten, sltclt, sltcth);

    root = insertNode(root, m);
}

void DsMonHoc::remove(string ms) {}

void DsMonHoc::update(MonHoc m) {}

void DsMonHoc::displayInOrder() {
    cout << "\nDisplay inorder\n";
    traverseInOrder(root);
}

void DsMonHoc::displayPostOrder() {
    cout << "\nDisplay postorder\n";
    traversePostOrder(root);
}

bool DsMonHoc::isEmpty() { return root != NULL; }

int DsMonHoc::getCount() { return getCount(root); }

void DsMonHoc::toArray(MonHoc *arr[]) {
    int index = 0;
    addNodeToArray(root, arr, index);
}

// private
int DsMonHoc::getHeight(NodeMonHoc *root) {
    if (root == NULL)
        return -1;
    else {
        return max(getHeight(root->left), getHeight(root->right)) + 1;
    }
}

int DsMonHoc::getCount(NodeMonHoc *root) {
    if (root == NULL) {
        return 0;
    }

    return 1 + getCount(root->left) + getCount(root->right);
}

bool DsMonHoc::isBalanced(NodeMonHoc *root) {
    if (root == NULL) {
        return true;
    } else {
        return abs(getHeight(root->left) - getHeight(root->right)) > 1;
    }
}

void DsMonHoc::traverseInOrder(NodeMonHoc *root) {
    if (root != NULL) {
        traverseInOrder(root->left);
        cout << root->monhoc.toString() << endl;
        if (root->left) {
            cout << "\tleft child: " << root->left->monhoc.toString() << endl;
        }
        if (root->right) {
            cout << "\tright child: " << root->right->monhoc.toString() << endl;
        }
        traverseInOrder(root->right);
    }
}

void DsMonHoc::traversePostOrder(NodeMonHoc *root) {
    if (root != NULL) {
        cout << root->monhoc.toString() << endl;
        if (root->left) {
            cout << "\tleft: " << root->left->monhoc.toString() << endl;
        }
        if (root->right) {
            cout << "\tright: " << root->right->monhoc.toString() << endl;
        }
        traversePostOrder(root->left);
        traversePostOrder(root->right);
    }
}

// set all tree's nodes to null
NodeMonHoc *DsMonHoc::emptyTree(NodeMonHoc *root) {
    if (root == NULL) return NULL;

    emptyTree(root->left);
    emptyTree(root->right);

    return NULL;
}

NodeMonHoc *DsMonHoc::search(NodeMonHoc *root, string ms) {
    if (root == NULL)
        return NULL;
    else if (ms < root->monhoc.ms)
        return search(root->left, ms);
    else if (ms > root->monhoc.ms)
        return search(root->right, ms);
    else
        return root;
}

NodeMonHoc *DsMonHoc::insertNode(NodeMonHoc *root, MonHoc &m) {
    if (root == NULL) {
        root = new NodeMonHoc;
        root->monhoc = m;
    } else if (m.ms < root->monhoc.ms)
        root->left = insertNode(root->left, m);
    else if (m.ms > root->monhoc.ms)
        root->right = insertNode(root->right, m);

    return root;
}

// NodeMonHoc *DsMonHoc::removeNode(NodeMonHoc *root, string ms) {
// NodeMonHoc *temp;

// if (root == NULL)
//     return NULL;
// else if (ms < root->monhoc.ms)
//     root->left = removeNode(root->left, ms);
// else if (ms > root->monhoc.ms)
//     root->right = removeNode(root->right, ms);
// else if (root->left && root->right) {
//     temp = findLeft(root->right);
//     root->monhoc = temp->monhoc;
//     root->right = removeNode(root->right, root->monhoc.ms);
// } else {
//     temp = root;
//     if (root->left == NULL)
//         root = root->right;
//     else if (root->right == NULL)
//         root = root->left;
//     delete temp;
// }
// return root;
// return NULL;
// }

void DsMonHoc::addNodeToArray(NodeMonHoc *root, MonHoc *array[], int &index) {
    if (root == NULL) {
        return;
    }
    addNodeToArray(root->left, array, index);

    array[index] = &root->monhoc;
    index++;
    // cout << &root->monhoc;
    addNodeToArray(root->right, array, index);
}

void testDSMH(DsMonHoc &ds) {
    ds.insert("6", "MONHOC 1", 1, 1);
    ds.insert("4", "Mon HOC 2", 1, 1);
    ds.insert("3", "TOAN", 1, 1);
    ds.insert("5", "", 1, 1);
    ds.insert("8", "", 1, 1);
    ds.insert("7", "", 1, 1);
    ds.insert("9", "", 1, 1);

    // int len = ds.getCount();
    // cout << "Number of mh: " << len << endl;

    // MonHoc *arr[len];
    // ds.toArray(arr);

    // for (unsigned i = 0; i < len; i++) {
    //     cout << arr[i]->toString() << endl;
    // }

    // arr[2]->ten = "FUCK FUCK";

    // ds.displayPostOrder();

}

#endif
