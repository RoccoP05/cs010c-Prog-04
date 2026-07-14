#ifndef HARD_DISK_H_
#define HARD_DISK_H_

#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include "btree_map.h"

// Simulates a hard disk that maps file indices to files.
// T must have static Read(ifstream&) and Write(ofstream&) methods.
template <typename T>
class HardDisk {
 private:
  BTreeMap<long, T> data_;
  long next_index_;

 public:
  HardDisk() : data_(4), next_index_(0) {}

  // Get a new file index (simulates variable-size file allocation).
  long NewFile() {
    long index = next_index_;
    next_index_ += 1 + (rand() % 4);
    return index;
  }

  // Store a file at the given index.
  void Put(long index, const T& file) {
    data_.Put(index, file);
  }

  // Get a mutable reference to a file by index.
  T& Get(long index) {
    return data_.Get(index);
  }

  // Get a const reference to a file by index.
  const T& Get(long index) const {
    return data_.Get(index);
  }

  // Check if an index exists.
  bool Contains(long index) const {
    return data_.ContainsKey(index);
  }

  // Get the number of files.
  size_t Size() const {
    return data_.Size();
  }

  // Get all indices in sorted order.
  std::vector<long> GetIndices() const {
    return data_.Keys();
  }

  // Write all files to disk.
  void Write(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
      throw std::runtime_error("Could not open file: " + filename);
    }
    std::vector<long> keys = data_.Keys();
    for (long key : keys) {
      out << key << std::endl;
      data_.Get(key).Write(out);
    }
    out.close();
  }

  // Read all files from disk.
  void Read(const std::string& filename) {
    data_ = BTreeMap<long, T>(4);
    std::ifstream in(filename);
    if (!in.is_open()) {
      throw std::runtime_error("Could not open file: " + filename);
    }
    while (in.peek() != EOF) {
      long index;
      in >> index;
      in.ignore();
      T file = T::Read(in);
      data_.Put(index, file);
    }
    in.close();
  }
};

#endif  // HARD_DISK_H_