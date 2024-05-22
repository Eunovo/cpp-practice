#include "tree.h"
#include "utils.h"

Tree::Tree() {
  this->root = nullptr;
}

void Tree::insert(int data) {
  if (this->root == nullptr) {
    this->root = new Node{data, false, nullptr, nullptr, nullptr};
    return;
  }

  Node* currentNode = this->root;
  Node* parent;
  while(currentNode != nullptr) {
    // LOG("DEBUG", "Current node data: %i", currentNode->data);
    parent = currentNode;
    if (data > currentNode->data) {
      currentNode = currentNode->right;
      if (currentNode == nullptr) {
        parent->right = new Node{data, false, parent, nullptr, nullptr};
        // LOG("DEBUG", "Inserted %i to the right of %i", data, parent->data);
      }
      continue;
    }
    currentNode = currentNode->left;
    if (currentNode == nullptr) {
      parent->left = new Node{data, false, parent, nullptr, nullptr};
      // LOG("DEBUG", "Inserted %i to the left of %i", data, parent->data);
    }
  }
}

std::vector<int> Tree::ToVector() {
  std::vector<int> result;
  
  Node* currentNode = this->root;
  while(currentNode != nullptr) {
    if (currentNode->visited) {
      currentNode = currentNode->parent;
      continue;
    }

    if (currentNode->left != nullptr && currentNode->left->visited == false) {
      currentNode = currentNode->left;
      continue;
    }
    result.push_back(currentNode->data);
    currentNode->visited = true;
    if (currentNode->right != nullptr) {
      currentNode = currentNode->right;
      continue;
    }
    currentNode = currentNode->parent;
  }

  return result;
}

Tree::~Tree() {
  Node* currentNode = this->root;
  Node* parent;
  while(currentNode != nullptr) {
    if (currentNode->left != nullptr) {
      currentNode = currentNode->left;
      continue;
    }

    if (currentNode->right != nullptr) {
      currentNode = currentNode->right;
      continue;
    }

    if (currentNode->parent == nullptr) {
      delete currentNode;
      this->root = nullptr;
      parent = nullptr;
      currentNode = nullptr;
      break;
    }

    parent = currentNode->parent;
    if (parent->left != nullptr && parent->left == currentNode) {
      parent->left = nullptr;
    } else {
      parent->right = nullptr;
    }
    delete currentNode;
    currentNode = parent;
  }
}

void test_tree() {
  LOG("INFO", "Testing simple binary tree");

  {
    Tree tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);
    tree.insert(6);

    std::vector<int> expected = {2, 3, 4, 5, 6, 7};
    std::vector<int> actual = tree.ToVector();

    IS_TRUE(expected == actual);
  }

  LOG("INFO", "Testing empty tree");
  {
    Tree tree;
    std::vector<int> expected = {};
    std::vector<int> actual = tree.ToVector();
    IS_TRUE(expected == actual);
  }

  LOG("INFO", "Testing tree with one node");
  {
    Tree tree;
    tree.insert(5);
    std::vector<int> expected = {5};
    std::vector<int> actual = tree.ToVector();
    IS_TRUE(expected == actual);
  }

  LOG("INFO", "Testing tree with duplicate values");
  {
    Tree tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(3);
    tree.insert(7);
    std::vector<int> expected = {3, 3, 5, 7, 7};
    std::vector<int> actual = tree.ToVector();
    IS_TRUE(expected == actual);
  }

  LOG("INFO", "Testing tree with negative values");
  {
    Tree tree;
    tree.insert(-5);
    tree.insert(-3);
    tree.insert(-7);
    tree.insert(-2);
    tree.insert(-4);
    tree.insert(-6);
    std::vector<int> expected = {-7, -6, -5, -4, -3, -2};
    std::vector<int> actual = tree.ToVector();
    IS_TRUE(expected == actual);
  }

  LOG("INFO", "Simple binary tree test passed");
}
