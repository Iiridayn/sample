#include "WordTree.h"

void WordTree::insert(string word, string url, string title) {
  if(head == 0) {
    head = new Word(word);
    if(url != "")
      head->insert(url, title);
  }
  else {
    Word* temp = head;
    bool unfound = true;
    while(unfound) {
      if(temp->word < word){
	if(temp->left != 0)
	  temp = temp->left;
	else {
	  temp->left = new Word(word);
	  if(url != "")
	    temp->left->insert(url, title);
	  unfound = false;
	}
      }
      else if(temp->word > word) {
	if(temp->right != 0)
	  temp = temp->right;
	else {
	  temp->right = new Word(word);
	  if(url != "")
	    temp->right->insert(url, title);
	  unfound = false;
	}
      }
      else {
	if(url != "")
	  temp->insert(url, title);
	unfound = false;
      }
    }//while
  }//else
}

bool WordTree::isin(string word) {
  bool found = false;
  Word* temp = head;
  while(!found && temp != 0) {
    if(temp->word > word)
      temp = temp->right;
    else if(temp->word < word)
      temp = temp->left;
    else
      found = true;
  }
  if(temp == 0)
    return false;
  return true;
}

void WordTree::destruct(Word* temp) {
  if(temp == 0)
    return;
  destruct(temp->left);
  destruct(temp->right);
  delete temp;
}

Word* WordTree::pop() { 
  if(head == 0) return 0;
  Word* ret = pop(head);
  if(ret == head)
    head = head->left;
  return ret;
}

Word* WordTree::pop(Word* temp) {
  if(temp == 0) return 0;
  Word* ret = pop(temp->right);
  if(ret == 0)
    ret = temp;
  else if(ret == temp->right)
    temp->right = temp->right->left;
  return ret;
}
