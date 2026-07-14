#ifndef WORD_FILE_H_
#define WORD_FILE_H_

#include <string>
#include <vector>
#include <fstream>

// Represents information stored about a word.
struct WordFile {
  std::string word;
  std::vector<long> indices;  // Indices of pages containing this word

  WordFile() : word("") {}

  explicit WordFile(const std::string& word) : word(word) {}

  // Read a WordFile from an input stream.
  // Format:
  //   <word>
  //   <number_of_pages>
  //   <page_index_1>
  //   <page_index_2>
  //   ...
  static WordFile Read(std::ifstream& in);

  // Write this WordFile to an output stream.
  void Write(std::ofstream& out) const;
};

#endif //WORD_FILE_H_