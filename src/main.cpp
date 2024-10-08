#include "binary_search.h"
#include "concurrency/concurrency.h"
#include "tree.h"
#include "utils.h"

int main()
{
  LOG("INFO", "Launching program");

  test_binary_search();
  test_tree();
  test_concurrency_one();

  LOG("INFO", "All tests run");
  return 0;
}