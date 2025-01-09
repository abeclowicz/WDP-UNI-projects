// autor: Michał Szydło
// code review: Adam Bęcłowicz

#include <bits/stdc++.h>
#include "prev.h"
using namespace std;

struct Node {
	Node *left_son, *right_son;
	int l, r, value; // [l, r], max value
	
	Node(int l_seg, int r_seg) {
		left_son = right_son = nullptr;
		
		l = l_seg;
		r = r_seg;
		
		value = -1;
	}
};

vector<Node *> roots, allocated_nodes;
static int ile_elementow = 0;

// max value of two sons
int maksi(Node *left_son, Node *right_son) {
	if(left_son == nullptr) {
		return right_son->value;
	}
	else if(right_son == nullptr) {
		return left_son->value;
	}

	return max(left_son->value, right_son->value);
}

void insert(Node *current, Node *previous, int value) {
	allocated_nodes.push_back(current);

	if(current->l == current->r) {
		current->value = ile_elementow;
		return;
	}

	int mid = current->l + (int)(((long long)current->r - current->l) / 2);

	if(value <= mid) {
		current->left_son = new Node(current->l, mid);

		if(previous != nullptr) {
			current->right_son = previous->right_son;
			previous = previous->left_son;
		}
		else {
			current->right_son = nullptr;
		}

		insert(current->left_son, previous, value);
	}
	else {
		current -> right_son = new Node(mid + 1, current->r);

		if(previous != nullptr) {
			current->left_son = previous->left_son;
			previous = previous->right_son;
		}
		else {
			current->left_son = nullptr;
		}

		insert(current->right_son, previous, value);
	}

	current->value = maksi(current->left_son, current->right_son);
}

// adding new root and new path
void pushBack(int value) {
	Node *previous_root = roots.empty() ? nullptr : roots.back();
	Node *current = new Node(INT_MIN, INT_MAX);

	roots.push_back(current);
	insert(current, previous_root, value);

	ile_elementow++;
}

void init(const std::vector<int> &seq) {
	for(int value : seq) {
		pushBack(value);
	}
}

// persistent segment tree query
int query(Node *v, int lo, int hi) {
	if(v == nullptr || v->l > hi || v->r < lo){
		return -1;
	}

	if(v->l >= lo && v->r <= hi) {
		return v->value;
	}

	return max(query(v->left_son, lo, hi), query(v->right_son, lo, hi));
}

// max{ j : 0 <= j <= i && seq[j] \in [lo..hi] } or -1
int prevInRange(int i, int lo, int hi) {
	return query(roots[i], lo, hi);
}

void done() {
	for(Node *node : allocated_nodes) {
        delete node;
    }
}

