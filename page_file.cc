#include "page_file.h"

// Format written/read (index line is handled by HardDisk, not here):
//   <url>
//   <impact>
//   <number_of_links>
//   <link_index_1>
//   ...
PageFile PageFile::Read(std::ifstream& in) {
  PageFile pf;
  std::getline(in, pf.url);
  in >> pf.impact;
  in.ignore();
  long count;
  in >> count;
  in.ignore();
  pf.indices.resize(count);
  for (long i = 0; i < count; i++) {
    in >> pf.indices[i];
    in.ignore();
  }
  return pf;
}

void PageFile::Write(std::ofstream& out) const {
  out << url << "\n";
  out << impact << "\n";
  out << indices.size() << "\n";
  for (long idx : indices) {
    out << idx << "\n";
  }
}
