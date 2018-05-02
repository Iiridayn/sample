#include <string.h>
#include "AVL.h"

void* lookup(char* key) {
	int cmpres = 0;
 	BTree* current = head;
	while(current != 0) {
		cmpres = strcmp(key, current->key);
		if(cmpres < 0)
			current = current->left;
		else if(cmpres > 0)
			current = current->right;
		else
			return current->data;
	}
	return 0;
}

void destroy(BTree* current) {
	destroy(current->left);
	destroy(current->right);
	free(current);
}

void insert(char* key, void* data) {
	int cmpres = 0;
	BTree* temp;
 	BTree* current = head;
 	
 	if(head == 0) {
 		temp = (BTree *)malloc(sizeof(BTree));
		temp->key = key;
		temp->data = data;
		temp->balance = 0;
		temp->parent = 0;
		temp->left = 0;
		temp->right = 0;
		head = temp;
		return; // no need to rebalance
 	}
 	
	while(current != 0) {
		cmpres = strcmp(key, current->key);
		if(cmpres < 0)
			if(current->left == 0) {
				temp = (BTree *)malloc(sizeof(BTree));
				temp->key = key;
				temp->data = data;
				current->left = temp;
				temp->parent = current;
				temp->balance = 0;
				temp->left = 0;
				temp->right = 0;
				//temp->parent->balance += -1;
				balance(temp);
				return;
			}
			else current = current->left;
		else if(cmpres > 0)
			if(current->right == 0) {
				temp = (BTree *)malloc(sizeof(BTree));
				temp->key = key;
				temp->data = data;
				current->right = temp;
				temp->parent = current;
				temp->balance = 0;
				temp->left = 0;
				temp->right = 0;
				//temp->parent->balance += 1;
				balance(temp);
				return;
			}
			else current = current->right;
		else return;
	}
	return;
}

void balance(BTree* node) {
	while(node != 0) {
		// recalc balance
		node->balance = 0;
		if(node->left != 0) {
  			node->balance -= -1;
  			if(node->left->balance != 0) node->balance -= 1;
		}
		if(node->right != 0) {
  			node->balance += 1;
  			if(node->right->balance != 0) node->balance += 1;
		}
	
		if(node->balance == -2)
			if(node->right->balance == -1) rotate(node, "RR");
			else rotate(node, "RL");
		else if(node->balance == 2)
			if(node->left->balance == 1) rotate(node, "LL");
			else rotate(node, "LR");
		
		node = node->parent;
	}
}

void rotate(BTree* node, char* type) {
	BTree* temp;

	switch(type) {
		case "RR":
			temp = node->right;
			node->right = temp->left;
			node->right->parent = node;
			temp->left = node;
			temp->parent = node->parent;
			if(!temp->parent) head = temp;
			else if(node->parent->left == node) temp->parent->left = temp;
			else temp->parent->right = temp;
			node->parent = temp;
			node->balance = 0;
			temp->balance = 0;
			break;
		case "LL":
			temp = node->left;
			node->left = temp->right;
			node->left->parent = node;
			temp->right = node;
			temp->parent = node->parent;
			if(!temp->parent) head = temp;
			else if(node->parent->left == node) temp->parent->left = temp;
			else temp->parent->right = temp;
			node->parent = temp;
			node->balance = 0;
			temp->balance = 0;
			break;
		case "LR":
			temp = node->left->right;
			node->left->right = temp->left;
			node->left->right->parent = node->left->right;
			temp->left = node->left;
			node->left->parent = temp;
			temp->parent = node->parent;
			if(!temp->parent) head = temp;
			else if(node->parent->left == node) node->parent->left = temp;
			else node->parent->right = temp;
			node->left = temp->right;
			node->left->parent = node;
			temp->right = node;
			node->parent = temp;
			temp->balance = 0;
			temp->left->balance = 0;
			temp->right->balance = 0;
			break;
		case "RL":
			temp = node->right->left;
			node->right->left = temp->right;
			node->right->left->parent = node->right->left;
			temp->right = node->right;
			node->right->parent = temp;
			temp->parent = node->parent;
			if(!temp->parent) head = temp;
			else if(node->parent->left == node) node->parent->left = temp;
			else node->parent->right = temp;
			node->right = temp->left;
			node->right->parent = node;
			temp->left = node;
			node->parent = temp;
			temp->balance = 0;
			temp->left->balance = 0;
			temp->right->balance = 0;
			break;
	}
}

