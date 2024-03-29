#ifndef DSMH_H
#define DSMH_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "model/MonHoc.h"
#include "model/MonHocQueue.h"
#include "util/Helper.h"

using std::cout;
using std::endl;
using std::string;

class DsMonHoc {
public:
    DsMonHoc();
    ~DsMonHoc();

    void insert(string ms, string ten, int sltclt, int sltcth);
    void update(string msOld, string ten, int sltclt, int sltcth, string msNew);
    void remove(string ms);
    MonHoc *search(string ms);
    bool isEmpty();
    int getSize();
    void read();
    void write();
    void toArray(MonHoc *arr[], int &length);

    // debugging
    void displayPostOrder();
    void displayInOrder();
    void displayLevelOrder();

private:
    NodeMonHoc *root;
    int getHeight(NodeMonHoc *root);
    int getSize(NodeMonHoc *root);
    bool isBalanced(NodeMonHoc *root);
    int balanceFactor(NodeMonHoc *root);
    NodeMonHoc *emptyTree(NodeMonHoc *root);
    NodeMonHoc *search(NodeMonHoc *root, string ms);

    void traverseInOrder(NodeMonHoc *root);
    void traversePostOrder(NodeMonHoc *root);

    NodeMonHoc *insertNode(NodeMonHoc *root, MonHoc m);
    NodeMonHoc *findLeft(NodeMonHoc *root);
    NodeMonHoc *removeNode(NodeMonHoc *root, string ms);

    NodeMonHoc *RotateRight(NodeMonHoc *n2);
    NodeMonHoc *RotateLeft(NodeMonHoc *n2);
    NodeMonHoc *RotateLR(NodeMonHoc *n3);
    NodeMonHoc *RotateRL(NodeMonHoc *n3);

    NodeMonHoc *readFromFile(NodeMonHoc *root, std::ifstream &reader);
    void addNodeToArray(NodeMonHoc *root, MonHoc *arr[], int &length);
};

// public
DsMonHoc::DsMonHoc() {
    root = NULL;
}

DsMonHoc::~DsMonHoc() {
    root = emptyTree(root);
}

void DsMonHoc::insert(string ms, string ten, int sltclt, int sltcth) {
    MonHoc m(ms, ten, sltclt, sltcth);

    root = insertNode(root, m);
}

void DsMonHoc::update(string msOld, string ten, int sltclt, int sltcth,
                      string msNew) {
    if (msOld == msNew) {
        MonHoc *m = search(msOld);

        if (m == NULL) return;

        m->ten = ten;
        m->sltclt = sltclt;
        m->sltcth = sltcth;
    } else {
        remove(msOld);
        insert(msNew, ten, sltclt, sltcth);
    }
}

void DsMonHoc::remove(string ms) {
    root = removeNode(root, ms);
}

MonHoc *DsMonHoc::search(string ms) {
    return &search(root, ms)->monhoc;
}

void DsMonHoc::displayInOrder() {
    cout << "\n\tDisplay inorder\n";
    traverseInOrder(root);
}

void DsMonHoc::displayPostOrder() {
    cout << "\n\tDisplay postorder\n";
    traversePostOrder(root);
}

void DsMonHoc::displayLevelOrder() {
    cout << "\n\tDisplay level order\n";

    if (root == NULL) {
        return;
    }

    // create an empty queue and enqueue the root
    MonHocQueue queue;
    queue.enqueue(root);

    NodeMonHoc *curr = NULL;

    while (!queue.isEmpty()) {
        curr = queue.front();
        queue.dequeue();

        cout << curr->monhoc.toString() << endl;

        if (curr->left) {
            queue.enqueue(curr->left);
        }
        if (curr->right) {
            queue.enqueue(curr->right);
        }
    }
}

bool DsMonHoc::isEmpty() {
    return root == NULL;
}

int DsMonHoc::getSize() {
    return getSize(root);
}

void DsMonHoc::toArray(MonHoc *arr[], int &length) {
    length = 0;
    addNodeToArray(root, arr, length);
}

void DsMonHoc::read() {
    std::ifstream reader("./build/data/monhoc.csv");

    if (reader.is_open()) {
        root = readFromFile(root, reader);
    }
    reader.close();
}

void DsMonHoc::write() {
    if (root == NULL) {
        return;
    }

    std::ofstream writer("./build/data/monhoc.csv");

    if (writer.is_open()) {
        if (root == NULL) {
            return;
        }

        // create an empty queue and enqueue the root
        MonHocQueue queue;
        queue.enqueue(root);

        NodeMonHoc *curr = NULL;

        while (!queue.isEmpty()) {
            curr = queue.front();
            queue.dequeue();

            writer << curr->monhoc.toString() << endl;

            if (curr->left) {
                queue.enqueue(curr->left);
            }
            if (curr->right) {
                queue.enqueue(curr->right);
            }
        }
    }
    writer.close();
}

// private
// get height
int DsMonHoc::getHeight(NodeMonHoc *root) {
    if (root == NULL)
        return -1;
    else {
        return std::max(getHeight(root->left), getHeight(root->right)) + 1;
    }
}

// get node count
int DsMonHoc::getSize(NodeMonHoc *root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + getSize(root->left) + getSize(root->right);
}

bool DsMonHoc::isBalanced(NodeMonHoc *root) {
    if (root == NULL) {
        return true;
    } else {
        return abs(getHeight(root->left) - getHeight(root->right)) > 1;
    }
}

int DsMonHoc::balanceFactor(NodeMonHoc *root) {
    return getHeight(root->left) - getHeight(root->right);
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

    delete root;
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

NodeMonHoc *DsMonHoc::insertNode(NodeMonHoc *root, MonHoc m) {
    if (root == NULL) {
        root = new NodeMonHoc;
        root->monhoc = m;
    } else if (m.ms < root->monhoc.ms) {
        root->left = insertNode(root->left, m);
    } else if (m.ms > root->monhoc.ms) {
        root->right = insertNode(root->right, m);
    }

    int balance = balanceFactor(root);

    if (balance > 1) {
        // left heavy
        if (m.ms < root->left->monhoc.ms) {
            root = RotateRight(root);
        } else {
            root = RotateLR(root);
        }
    } else if (balance < -1) {
        // right heavy
        if (m.ms < root->right->monhoc.ms) {
            root = RotateRL(root);
        } else {
            root = RotateLeft(root);
        }
    }
    return root;
}

NodeMonHoc *DsMonHoc::findLeft(NodeMonHoc *root) {
    if (root == NULL) {
        return NULL;
    } else if (root->left == NULL) {
        return root;
    } else {
        return findLeft(root->left);
    }
}

NodeMonHoc *DsMonHoc::removeNode(NodeMonHoc *root, string ms) {
    NodeMonHoc *temp;

    if (root == NULL) {
        return NULL;
    } else if (ms < root->monhoc.ms) {
        root->left = removeNode(root->left, ms);
    } else if (ms > root->monhoc.ms) {
        root->right = removeNode(root->right, ms);
    } else if (root->left && root->right) {
        // node has 2 child
        temp = findLeft(root->right);
        root->monhoc = temp->monhoc;
        root->right = removeNode(root->right, root->monhoc.ms);
    } else {
        // node is leaf or has only 1 child
        temp = root;
        if (root->left == NULL) {
            root = root->right;
        } else {
            root = root->left;
        }
        delete temp;
        return root;
    }

    // balancing
    int balance = balanceFactor(root);

    if (balance > 1) {
        if (balanceFactor(root->left) >= 0) {
            // left left
            root = RotateRight(root);
        } else {
            // left right
            root = RotateLR(root);
        }
    } else if (balance < -1) {
        if (balanceFactor(root->right) <= 0) {
            // right right
            root = RotateLeft(root);
        } else {
            // right left
            root = RotateRL(root);
        }
    }
    return root;
}

NodeMonHoc *DsMonHoc::RotateLeft(NodeMonHoc *n1) {
    NodeMonHoc *n2 = n1->right;
    n1->right = n2->left;
    n2->left = n1;
    return n2;
}

NodeMonHoc *DsMonHoc::RotateRight(NodeMonHoc *n3) {
    NodeMonHoc *n2 = n3->left;
    n3->left = n2->right;
    n2->right = n3;
    return n2;
}

NodeMonHoc *DsMonHoc::RotateLR(NodeMonHoc *n3) {
    n3->left = RotateLeft(n3->left);
    return RotateRight(n3);
}

NodeMonHoc *DsMonHoc::RotateRL(NodeMonHoc *n3) {
    n3->right = RotateRight(n3->right);
    return RotateLeft(n3);
}

NodeMonHoc *DsMonHoc::readFromFile(NodeMonHoc *root, std::ifstream &reader) {
    string line;
    string delim = "|";

    while (getline(reader, line)) {
        string data[4] = {""};

        int index = 0;
        size_t start = 0;
        size_t end = line.find(delim, start);

        while (end != string::npos) {
            data[index] = line.substr(start, end - start);
            start = end + delim.length();
            end = line.find(delim, start);
            index++;
        }

        data[index] = line.substr(start, end);
        MonHoc m(data[0], data[1], stoi(data[2]), stoi(data[3]));

        root = insertNode(root, m);
    }
    return root;
}

void DsMonHoc::addNodeToArray(NodeMonHoc *root, MonHoc *array[], int &length) {
    if (root == NULL) {
        return;
    }
    addNodeToArray(root->left, array, length);

    array[length++] = &root->monhoc;

    addNodeToArray(root->right, array, length);
}

void testDSMH(DsMonHoc &ds) {
    ds.read();
    // ds.write();
    ds.displayLevelOrder();

    ds.remove("INT9");
    ds.displayLevelOrder();

    ds.write();
}

#endif
