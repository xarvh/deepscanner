all: deepscanner stats maximiseSpread colorise antialias

%: %.cc
	$(CXX) $< -o $@
