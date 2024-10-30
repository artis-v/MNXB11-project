CXX := g++
CXXWARNINGS := -Wall -Wextra -Werror
CXXOPT := -O3
CXXSTD := -std=c++17
INCLUDES := -I include
CXXFLAGS := $(CXXWARNINGS) -fPIC $(CXXSTD) $(CXXOPT) $(INCLUDES)
LDFLAGS :=

.PHONY: all clean run

all: AnnualData.so run

run: AnnualData.so
	root -l plotData.C

AnnualData.so: src/AnnualData.o plotData.o
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LDFLAGS)

src/AnnualData.o: src/AnnualData.cxx
	$(CXX) $(CXXFLAGS) $^ -c -o $@

plotData.o: plotData.C
	$(CXX) $(CXXFLAGS) $^ -c -o $@

clean:
	rm -v src/AnnualData.o plotData.o AnnualData.so

	
