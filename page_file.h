#ifndef PAGE_FILE_H_
#define PAGE_FILE_H_

#include <string>
#include <vector>
#include <fstream>

// Represents information stored about a web page.
struct PageFile {
  std::string url;
  std::vector<long> indices;  // Indices of pages this page links to
  double impact;
  double impact_temp;         // Temporary storage for PageRank computation

  PageFile() : url(""), impact(0.0), impact_temp(0.0) {}

  explicit PageFile(const std::string& url)
      : url(url), impact(0.0), impact_temp(0.0) {}

  // Read a PageFile from an input stream.
  // Format:
  //   <url>
  //   <impact>
  //   <number_of_links>
  //   <link_index_1>
  //   <link_index_2>
  //   ...
  static PageFile Read(std::ifstream& in);

  // Write this PageFile to an output stream.
  void Write(std::ofstream& out) const;
};

#endif  // PAGE_FILE_H_