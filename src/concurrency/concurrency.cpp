#include <chrono>
#include <filesystem>
#include <fstream>
#include <semaphore>
#include <string>
#include <thread>
#include <vector>

#include "../tree.h"
#include "../utils.h"

#define ASSERT_FILES_HAVE_SAME_CONTENT(expFile, actFile) { \
  std::ifstream inExpFile (expFile); \
  std::ifstream inActFile (actFile); \
  std::string line1, line2; \
  while (getline(inExpFile, line1)) { \
    IS_TRUE(getline(inActFile, line2)); \
    IS_TRUE(line1 == line2); \
  } \
}

struct Timer {
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  Timer() {
    start = std::chrono::high_resolution_clock::now();
  }

  int duration() {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  }

  void reset() {
    start = std::chrono::high_resolution_clock::now();
  }
};

bool serial(std::vector<std::string> files, std::string output_file, std::string& error) {
  Tree tree;
  std::string line;
  for (const auto& file : files) {
    std::ifstream infile (file);
    if (infile.is_open()) {
      while (getline(infile,line)) {
        tree.insert(std::stoi(line));
      }
      infile.close();
    } else {
      error = "Could not open file: " + file;
      return false;
    }
  }
  std::vector<int> result = tree.ToVector();
  std::ofstream outfile (output_file);
  if (outfile.is_open()) {
    for (const int& data : result) {
      outfile << data << "\n";
    }
    outfile.close();
  } else {
    error = "Could not open file: " + output_file;
    return false;
  }
  return true;
}

std::binary_semaphore smphSignalThreadToThread{1};
std::counting_semaphore smphSignalThreadToMain{3};

void read_file(std::string file, Tree& tree) {
  std::string line;
  std::ifstream infile (file);
  std::vector<int> data;

  if (infile.is_open()) {
    while (getline(infile, line)) {
      data.push_back(std::stoi(line));
    }
    infile.close();
  }

  smphSignalThreadToThread.acquire();
  for (const int& d : data) {
    tree.insert(d);
  }
  smphSignalThreadToThread.release();
  smphSignalThreadToMain.release();
}


bool concurrency_one(std::vector<std::string> files, std::string output_file, std::string& error) {
  std::vector<std::thread> threads;
  Tree tree;
  for (const std::string& file : files) {
    smphSignalThreadToMain.acquire();
    std::thread t(read_file, file, std::ref(tree));
    t.detach();
  }

  smphSignalThreadToMain.acquire();
  smphSignalThreadToMain.acquire();
  smphSignalThreadToMain.acquire();

  std::vector<int> result = tree.ToVector();  
  std::ofstream outfile (output_file);
  if (outfile.is_open()) {
    for (const int& r : result) {
      outfile << r << "\n";
    }
    outfile.close();
  } else {
    error = "Could not open file: " + output_file;
    return false;
  }
  return true;
}

void test_concurrency_one() {
  LOG("INFO", "Testing Concurrency One");
  
  std::filesystem::path cwd = std::filesystem::current_path();
  std::filesystem::path input_dir = cwd / "src" / "concurrency" / "inputs";
  std::filesystem::path serial_output_file = cwd / "src" / "concurrency" / "outputs" / "serial.txt";
  std::filesystem::path output_file = cwd / "src" / "concurrency" / "outputs" / "case_1.txt";
  std::vector<std::string> files;
  for (const auto& entry : std::filesystem::directory_iterator(input_dir)) {
    files.push_back(entry.path().string());
  }

  std::string error;
  bool success;
  Timer timer;
  success = serial(files, serial_output_file.string(), error);
  int serial_duration = timer.duration();
  if (!success) {
    LOG("ERROR", "Failed with error: %s", error.c_str());
  }
  LOG("INFO", "serial execution took %ims", serial_duration);

  timer.reset();
  success = concurrency_one(files, output_file.string(), error);
  int duration = timer.duration();
  if (!success) {
    LOG("ERROR", "Failed with error: %s", error.c_str());
  }
  LOG("INFO", "concurrent execution took %ims", duration);

  ASSERT_FILES_HAVE_SAME_CONTENT(serial_output_file, output_file);
  IS_TRUE(duration < serial_duration);
}
