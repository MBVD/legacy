#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define BUF_MAX 256

typedef struct TreeNode {
  int data;
  struct TreeNode* left;
  struct TreeNode* right;
} TreeNode;

TreeNode* newTreeNode(int data) {
  TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
  node->data = data;
  node->left = NULL;
  node->right = NULL;
  return node;
}

TreeNode* insertTreeNode(TreeNode* node, int data) {
  if (node == NULL) return newTreeNode(data);
  if (data < node->data)
    node->left = insertTreeNode(node->left, data);
  else if (data > node->data)
    node->right = insertTreeNode(node->right, data);
  return node;
}

void print_tree(TreeNode* tree, int pos, int k){
  if (tree == NULL) {
    return;
  }
  if (pos == k){
    printf("%d ", tree->data);
    return;
  }  
  print_tree(tree->left, pos+1, k);
  print_tree(tree->right, pos+1, k);
}

int main(){
  TreeNode* root = NULL;
  root = insertTreeNode(root, 50);
  root = insertTreeNode(root, 30);
  root = insertTreeNode(root, 20);
  root = insertTreeNode(root, 60);
  root = insertTreeNode(root, 70);
  root = insertTreeNode(root, 55);
  print_tree(root, 1, 3);
}