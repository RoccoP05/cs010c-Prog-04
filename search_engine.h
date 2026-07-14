#ifndef SEARCH_ENGINE_H_
#define SEARCH_ENGINE_H_

#include <string>
#include <vector>
#include <vector>
#include <unordered_map>
#include "btree_map.h"
#include "hard_disk.h"
#include "page_file.h"
#include "word_file.h"

// Abstract base class for a search engine.
class SearchEngine {
 protected:
  HardDisk<PageFile> page_disk_;
  HardDisk<WordFile> word_disk_;
  BTreeMap<std::string, long> index_of_url_;
  std::unordered_map<std::string, long> index_of_word_;

 public:
  SearchEngine() : index_of_url_(4) {}
  virtual ~SearchEngine() = default;

  // Collect web pages starting from initial URLs.
  virtual void Collect(const std::vector<std::string>& starting_urls) = 0;

  // Rank pages using the PageRank algorithm.
  virtual void Rank() = 0;

  // Search for pages containing all search words.
  // Returns URLs of top num_results pages ranked by impact.
  virtual std::vector<std::string> Search(
      const std::vector<std::string>& search_words,
      int num_results) = 0;

  // Save page disk to file.
  void SavePageDisk(const std::string& filename) {
    page_disk_.Write(filename);
  }

  // Save word disk to file.
  void SaveWordDisk(const std::string& filename) {
    word_disk_.Write(filename);
  }

  // Load page disk from file and rebuild URL index.
  void LoadPageDisk(const std::string& filename) {
    page_disk_.Read(filename);
    std::vector<long> indices = page_disk_.GetIndices();
    for (long idx : indices) {
      index_of_url_.Put(page_disk_.Get(idx).url, idx);
    }
  }

  // Load word disk from file and rebuild word index.
  void LoadWordDisk(const std::string& filename) {
    word_disk_.Read(filename);
    std::vector<long> indices = word_disk_.GetIndices();
    for (long idx : indices) {
      index_of_word_[word_disk_.Get(idx).word] = idx;
    }
  }

  // Get the number of indexed pages.
  size_t GetPageCount() const { return page_disk_.Size(); }

  // Get the number of indexed words.
  size_t GetWordCount() const { return word_disk_.Size(); }
};

#endif  // SEARCH_ENGINE_H_