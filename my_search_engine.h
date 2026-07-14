#ifndef MY_SEARCH_ENGINE_H_
#define MY_SEARCH_ENGINE_H_

#include "search_engine.h"
#include "simple_browser.h"

// Your implementation of the search engine.
class MySearchEngine : public SearchEngine {
 private:
  SimpleBrowser browser_;

  // Index a new page and return its index.
  long IndexPage(const std::string& url);

  // Index a new word and return its index.
  long IndexWord(const std::string& word);

  // Run one iteration of the PageRank algorithm.
  void RankIteration();

  // Check if all elements of a vector are equal.
  bool AllEqual(const std::vector<long>& vec) const;

 public:
  MySearchEngine() = default;

  void Collect(const std::vector<std::string>& starting_urls) override;
  void Rank() override;
  std::vector<std::string> Search(
      const std::vector<std::string>& search_words,
      int num_results) override;
};

#endif  // MY_SEARCH_ENGINE_H_