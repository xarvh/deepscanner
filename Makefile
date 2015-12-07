all: deepscanner stats

%: %.cc
	$(CXX) -O3 $< -o $@
