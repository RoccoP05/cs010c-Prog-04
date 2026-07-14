#include "my_search_engine.h"
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <functional>

// Allocates a disk slot for url, stores a blank PageFile, and records the
// index in index_of_url_.  If url is already indexed, just returns its index.
long MySearchEngine::IndexPage(const std::string& url) {
  if (index_of_url_.ContainsKey(url)) {
    return index_of_url_.Get(url);
  }
  long idx = page_disk_.NewFile();
  page_disk_.Put(idx, PageFile(url));
  // Put into BTreeMap AFTER page_disk_.Put so any split in index_of_url_
  // does not invalidate the reference we just stored in page_disk_.
  index_of_url_.Put(url, idx);
  return idx;
}

// Allocates a disk slot for word if not already indexed.
long MySearchEngine::IndexWord(const std::string& word) {
  auto it = index_of_word_.find(word);
  if (it != index_of_word_.end()) {
    return it->second;
  }
  long idx = word_disk_.NewFile();
  word_disk_.Put(idx, WordFile(word));
  index_of_word_[word] = idx;
  return idx;
}

// BFS web crawl starting from the given normalized URLs.
void MySearchEngine::Collect(const std::vector<std::string>& starting_urls) {
  std::queue<std::string> bfs;
  std::unordered_set<std::string> enqueued;

  for (const auto& url : starting_urls) {
    if (!enqueued.count(url)) {
      bfs.push(url);
      enqueued.insert(url);
    }
  }

  while (!bfs.empty()) {
    std::string url = bfs.front();
    bfs.pop();

    if (!browser_.LoadPage(url)) continue;

    long page_idx = IndexPage(url);

    // Process outgoing links
    std::vector<std::string> linked_urls = browser_.GetURLs();
    for (const auto& linked_url : linked_urls) {
      // IndexPage may Put into both page_disk_ and index_of_url_, so we
      // must NOT hold a live reference to any PageFile across this call.
      long linked_idx = IndexPage(linked_url);

      // Copy the PageFile out, update, write back — avoids dangling ref.
      PageFile page = page_disk_.Get(page_idx);
      page.indices.push_back(linked_idx);
      page_disk_.Put(page_idx, page);

      if (!enqueued.count(linked_url)) {
        bfs.push(linked_url);
        enqueued.insert(linked_url);
      }
    }

    // Record this page in each word's page list
    std::vector<std::string> words = browser_.GetWords();
    for (const auto& word : words) {
      long word_idx = IndexWord(word);

      // Same copy-before-Put discipline for word_disk_.
      WordFile wf = word_disk_.Get(word_idx);
      wf.indices.push_back(page_idx);
      word_disk_.Put(word_idx, wf);
    }
  }
}

// One iteration of PageRank: distribute each page's impact evenly to all
// pages it links to, then update impacts from the temporaries.
void MySearchEngine::RankIteration() {
  std::vector<long> indices = page_disk_.GetIndices();

  // Reset temporaries
  for (long idx : indices) {
    page_disk_.Get(idx).impact_temp = 0.0;
  }

  // Distribute impact across outgoing links
  for (long idx : indices) {
    PageFile& page = page_disk_.Get(idx);
    if (!page.indices.empty()) {
      double share = page.impact / static_cast<double>(page.indices.size());
      for (long linked_idx : page.indices) {
        if (page_disk_.Contains(linked_idx)) {
          page_disk_.Get(linked_idx).impact_temp += share;
        }
      }
    }
  }

  // Commit temporaries to impact
  for (long idx : indices) {
    PageFile& page = page_disk_.Get(idx);
    page.impact = page.impact_temp;
  }
}

bool MySearchEngine::AllEqual(const std::vector<long>& vec) const {
  if (vec.empty()) return true;
  long val = vec[0];
  for (size_t i = 1; i < vec.size(); i++) {
    if (vec[i] != val) return false;
  }
  return true;
}

void MySearchEngine::Rank() {
  // Initialize all impacts to 1.0
  std::vector<long> indices = page_disk_.GetIndices();
  for (long idx : indices) {
    page_disk_.Get(idx).impact = 1.0;
  }

  for (int i = 0; i < 20; i++) {
    RankIteration();
  }
}

// Returns the URLs of the top num_results pages (by impact) that contain
// every search word.  Uses a min-heap to track the top-N efficiently.
std::vector<std::string> MySearchEngine::Search(
    const std::vector<std::string>& search_words, int num_results) {

  if (search_words.empty()) return {};

  // Start with the page list for the first word
  auto it = index_of_word_.find(search_words[0]);
  if (it == index_of_word_.end()) return {};

  std::vector<long> result_indices = word_disk_.Get(it->second).indices;

  // Intersect with each remaining word's page list
  for (size_t w = 1; w < search_words.size(); w++) {
    auto wit = index_of_word_.find(search_words[w]);
    if (wit == index_of_word_.end()) return {};

    const std::vector<long>& word_pages = word_disk_.Get(wit->second).indices;
    std::vector<long> intersection;
    for (long idx : result_indices) {
      if (std::find(word_pages.begin(), word_pages.end(), idx)
          != word_pages.end()) {
        intersection.push_back(idx);
      }
    }
    result_indices = std::move(intersection);
    if (result_indices.empty()) return {};
  }

  // Keep the top num_results by impact with a min-heap
  using Pair = std::pair<double, long>;
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> min_heap;

  for (long idx : result_indices) {
    double impact = page_disk_.Get(idx).impact;
    if (static_cast<int>(min_heap.size()) < num_results) {
      min_heap.push({impact, idx});
    } else if (impact > min_heap.top().first) {
      min_heap.pop();
      min_heap.push({impact, idx});
    }
  }

  // Drain heap into result vector (ascending order from heap; reverse for desc)
  std::vector<std::string> results;
  results.reserve(min_heap.size());
  while (!min_heap.empty()) {
    results.push_back(page_disk_.Get(min_heap.top().second).url);
    min_heap.pop();
  }
  std::reverse(results.begin(), results.end());

  return results;
}
