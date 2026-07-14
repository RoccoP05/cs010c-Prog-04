#include "word_file.h"

// Format written/read (index line is handled by HardDisk, not here):
//   <word>
//   <number_of_pages>
//   <page_index_1>
//   ...
WordFile WordFile::Read(std::ifstream& in) {
  WordFile wf;
  std::getline(in, wf.word);
  long count;
  in >> count;
  in.ignore();
  wf.indices.resize(count);
  for (long i = 0; i < count; i++) {
    in >> wf.indices[i];
    in.ignore();
  }
  return wf;
}

void WordFile::Write(std::ofstream& out) const {
  out << word << "\n";
  out << indices.size() << "\n";
  for (long idx : indices) {
    out << idx << "\n";
  }
}
