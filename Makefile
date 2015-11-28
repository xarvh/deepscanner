
OUT_DS = deepscanner
OUT_CE = chaosexplorer
CFILES_DS = deepscanner.c io.c newton.cc pngio.cc
CFILES_CE = chaosexplorer.c io.c newton.cc

CFLAGS = -O3 -s `sdl-config --cflags`
LIBS = `sdl-config --libs` -lz -lpng

all: $(OUT_DS) $(OUT_CE)

$(OUT_DS): $(CFILES_DS)
	$(CXX) $(CFILES_DS) $(LIBS) -o $(OUT_DS)

$(OUT_CE): $(CFILES_CE)
	$(CXX) $(CFILES_CE) $(LIBS) -o $(OUT_CE)
