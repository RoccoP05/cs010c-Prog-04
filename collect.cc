#include <iostream>
#include <vector>
#include <string>
#include "my_search_engine.h"

int main() {
  MySearchEngine engine;

  // Starting URLs (normalized format)
  std::vector<std::string> starting_urls = {
    "edu.ucr.cs/~tyf/10c/google/mary.html"
  };

  std::cout << "Starting collection from " << starting_urls.size()
            << " URLs..." << std::endl;

  engine.Collect(starting_urls);

  std::cout << "Collection complete!" << std::endl;
  std::cout << "Indexed " << engine.GetPageCount() << " pages" << std::endl;
  std::cout << "Indexed " << engine.GetWordCount() << " words" << std::endl;

  // Save the data
  engine.SavePageDisk("pagedisk.txt");
  engine.SaveWordDisk("worddisk.txt");

  std::cout << "Data saved to pagedisk.txt and worddisk.txt" << std::endl;

  return 0;
}