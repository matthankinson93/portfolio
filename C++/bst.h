#ifndef BST_H
#define BST_H

#include <string>
#include <iostream>

using namespace std;

class TreeNode
{
public:
   void insert_node(TreeNode* new_node);
   void print_nodes() const;
   bool find(string value) const;
private:
   string data;
   TreeNode* left;
   TreeNode* right;
friend class BinarySearchTree;
};

class BinarySearchTree
{
public:
   BinarySearchTree();
   void insert(string data);
   void erase(string data);
   int count(string data) const;
   void print() const;
private:
   TreeNode* root;
};

#endif
