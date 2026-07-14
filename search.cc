#include <iostream>
#include <vector>
#include <string>
#include "my_search_engine.h"

int main() {
  MySearchEngine engine;

  std::cout << "Loading ranked page and word data..." << std::endl;

  engine.LoadPageDisk("pagedisk-ranked.txt");
  engine.LoadWordDisk("worddisk.txt");

  std::cout << "Loaded " << engine.GetPageCount() << " pages and "
            << engine.GetWordCount() << " words" << std::endl;

  // Search for pages containing all these words
  std::vector<std::string> search_words = {"mary", "jack", "jill"};

  std::cout << "Searching for: ";
  for (size_t i = 0; i < search_words.size(); i++) {
    if (i > 0) std::cout << ", ";
    std::cout << search_words[i];
  }
  std::cout << std::endl;

  std::vector<std::string> results = engine.Search(search_words, 5);

  std::cout << "Found " << results.size() << " results:" << std::endl;
  for (const std::string& url : results) {
    std::cout << url << std::endl;
  }

  return 0;
}