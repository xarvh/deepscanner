
OUT_CE = ../chaosexplorer
CFILES_CE = chaosexplorer.c io.c newton.cc

CFLAGS = -O3 -s
LIBS = `sdl-config --libs` -lz -lpng

all: $(OUT_CE)

$(OUT_CE): $(CFILES_CE)
	$(CXX) $(CFLAGS) $(CFILES_CE) $(LIBS) -o $(OUT_CE)
