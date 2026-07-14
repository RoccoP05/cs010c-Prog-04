CXX = g++
CXXFLAGS = -Wall -Werror -std=c++17

SRCS = my_search_engine.cc page_file.cc word_file.cc simple_browser.cc

all: collect rank search

collect: collect.cc $(SRCS)
	$(CXX) $(CXXFLAGS) -o collect collect.cc $(SRCS)

rank: rank.cc $(SRCS)
	$(CXX) $(CXXFLAGS) -o rank rank.cc $(SRCS)

search: search.cc $(SRCS)
	$(CXX) $(CXXFLAGS) -o search search.cc $(SRCS)

clean:
	rm -f collect rank search pagedisk.txt worddisk.txt pagedisk-ranked.txt
