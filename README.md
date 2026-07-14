# Programming Assignment 4 — Disk-Based Search Engine

A simulated disk-based search engine: crawls local HTML pages, indexes them to disk with a B-tree map, ranks them with PageRank, and supports multi-keyword search — modeled on a real search engine's crawl/index/rank/query pipeline.

## Architecture

- `btree_map.h` — a from-scratch B-tree ordered map (`BTreeMap<K, V>`) used as the on-disk index.
- `hard_disk.h` — `HardDisk<T>`, simulating variable-size file storage keyed by an index, backed by `BTreeMap`.
- `page_file.h` / `page_file.cc` — `PageFile`: a page's URL, outbound links, and PageRank impact score.
- `word_file.h` / `word_file.cc` — `WordFile`: a word and the indices of pages containing it.
- `simple_browser.h` / `simple_browser.cc` — `SimpleBrowser`, loads a local HTML file, extracting its links and trailing keyword list.
- `search_engine.h` — `SearchEngine`, an abstract base defining `Collect`, `Rank`, and `Search`.
- `my_search_engine.h` / `my_search_engine.cc` — `MySearchEngine`, the concrete implementation (indexing, PageRank, querying).

## Pipeline (three executables)

1. `collect.cc` — crawls starting URLs, builds the page/word index, saves `pagedisk.txt` / `worddisk.txt`.
2. `rank.cc` — loads the collected data, runs PageRank, saves `pagedisk-ranked.txt`.
3. `search.cc` — loads the ranked data and searches for pages matching a set of keywords (results ordered by PageRank impact).

## Build & run

```
make
./collect
./rank
./search
```
