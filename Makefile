all: deepscanner stats maximiseSpread colorise

%: %.cc
	$(CXX) $< -o $@
