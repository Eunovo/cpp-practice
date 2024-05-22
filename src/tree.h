#include <vector>

struct Node {
  int data;
  bool visited;
  Node* parent; 
  Node* left;
  Node* right;
};

class Tree {
  Node* root;
public:
  Tree();

  void insert(int data);
  std::vector<int> ToVector();
  
  ~Tree();
};

void test_tree();
