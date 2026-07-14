#include <iostream>
#include "my_search_engine.h"

int main() {
  MySearchEngine engine;

  std::cout << "Loading page and word data..." << std::endl;

  engine.LoadPageDisk("pagedisk.txt");
  engine.LoadWordDisk("worddisk.txt");

  std::cout << "Loaded " << engine.GetPageCount() << " pages and "
            << engine.GetWordCount() << " words" << std::endl;

  std::cout << "Running PageRank algorithm..." << std::endl;

  engine.Rank();

  std::cout << "PageRank complete!" << std::endl;

  engine.SavePageDisk("pagedisk-ranked.txt");

  std::cout << "Ranked data saved to pagedisk-ranked.txt" << std::endl;

  return 0;
}