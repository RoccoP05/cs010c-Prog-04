#include "simple_browser.h"
#include <fstream>
#include <sstream>
#include <algorithm>

std::string SimpleBrowser::NormalizeUrl(const std::string& url) {
  // Strip protocol prefix (http://, https://, etc.)
  std::string rest = url;
  size_t protocol_end = url.find("://");
  if (protocol_end != std::string::npos) {
    rest = url.substr(protocol_end + 3);
  }

  // Split into domain and path at first '/'
  size_t slash_pos = rest.find('/');
  std::string domain, path;
  if (slash_pos != std::string::npos) {
    domain = rest.substr(0, slash_pos);
    path = rest.substr(slash_pos + 1);
  } else {
    domain = rest;
    path = "";
  }

  // Reverse domain parts (e.g. "cs.ucr.edu" -> "edu.ucr.cs")
  std::vector<std::string> parts;
  std::istringstream ss(domain);
  std::string part;
  while (std::getline(ss, part, '.')) {
    parts.push_back(part);
  }
  std::reverse(parts.begin(), parts.end());

  std::string reversed_domain;
  for (size_t i = 0; i < parts.size(); i++) {
    if (i > 0) reversed_domain += '.';
    reversed_domain += parts[i];
  }

  if (!path.empty()) {
    return reversed_domain + '/' + path;
  }
  return reversed_domain;
}

bool SimpleBrowser::LoadPage(const std::string& url) {
  words_.clear();
  urls_.clear();

  std::ifstream file(url);
  if (!file.is_open()) return false;

  std::string line;
  std::string last_text_line;

  while (std::getline(file, line)) {
    // Stop at closing body/html tags
    if (line.find("</body>") != std::string::npos ||
        line.find("</html>") != std::string::npos) {
      break;
    }

    // Extract all href="..." URLs on this line
    size_t pos = 0;
    while (true) {
      size_t href_pos = line.find("href=\"", pos);
      if (href_pos == std::string::npos) break;
      size_t url_start = href_pos + 6;
      size_t url_end = line.find('"', url_start);
      if (url_end == std::string::npos) break;
      std::string href = line.substr(url_start, url_end - url_start);
      if (!href.empty()) {
        urls_.push_back(NormalizeUrl(href));
      }
      pos = url_end + 1;
    }

    // Track last non-empty, non-tag line as the keywords line
    std::string trimmed = line;
    size_t first = trimmed.find_first_not_of(" \t\r\n");
    if (first != std::string::npos) {
      trimmed = trimmed.substr(first);
      size_t last = trimmed.find_last_not_of(" \t\r\n");
      if (last != std::string::npos) trimmed = trimmed.substr(0, last + 1);
      if (!trimmed.empty() && trimmed[0] != '<') {
        last_text_line = trimmed;
      }
    }
  }

  // Parse space-separated keywords from the last text line
  std::istringstream ss(last_text_line);
  std::string word;
  while (ss >> word) {
    words_.push_back(word);
  }

  return true;
}
