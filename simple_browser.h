#ifndef SIMPLE_BROWSER_H_
#define SIMPLE_BROWSER_H_

#include <string>
#include <vector>

// A simple browser that loads web pages from local HTML files.
//
// File format:
//   - HTML files with <a href="..."> tags containing full URLs
//   - The last non-empty line (before </body></html>) contains
//     space-separated keywords
//   - Other lines are visible text (ignored for indexing)
class SimpleBrowser {
 private:
  std::vector<std::string> words_;
  std::vector<std::string> urls_;

 public:
  // Load a page from a local file.
  // The normalized URL is used directly as the local file path.
  // e.g., "edu.ucr.cs/~tyf/10c/google/mary.html" opens that path.
  // Returns true if the file was loaded successfully, false otherwise.
  bool LoadPage(const std::string& url);

  // Normalize a full URL to reversed-domain form.
  // e.g., "http://cs.ucr.edu/~tyf/10c/google/mary.html"
  //     -> "edu.ucr.cs/~tyf/10c/google/mary.html"
  static std::string NormalizeUrl(const std::string& url);

  // Get all keywords on the current page.
  std::vector<std::string> GetWords() const { return words_; }

  // Get all URLs linked from the current page.
  std::vector<std::string> GetURLs() const { return urls_; }
};

#endif  // SIMPLE_BROWSER_H_