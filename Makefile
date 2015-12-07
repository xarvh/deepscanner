all: deepscanner stats colorise

%: %.cc
	$(CXX) $< -o $@
