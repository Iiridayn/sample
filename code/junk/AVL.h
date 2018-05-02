#ifndef AVL_H
#define AVL_H

typedef struct b_tree {
	char* key;
	void* data;
	struct BTree *left, *right, *parent;
	int balance;
} BTree;

BTree* head = 0;

void* lookup(char* key);
void destroy(BTree* current);
void destruct() { destroy(head); }
void insert(char* key, void* data);
void balance(BTree* node);
void rotate(BTree* node, char* type);

#endif
