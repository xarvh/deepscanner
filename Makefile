
OUT_DS = deepscanner
OUT_CE = chaosexplorer
OUT_DM = deepmapper
CFILES_DS = deepscanner.c io.c newton.cc pngio.cc
CFILES_CE = chaosexplorer.c io.c newton.cc
CFILES_DM = deepmapper.cc

CFLAGS = -O3 -s
LIBS = `sdl-config --libs` -lz -lpng

all: $(OUT_DS) $(OUT_CE) $(OUT_DM)

$(OUT_DS): $(CFILES_DS)
	$(CXX) $(CFLAGS) $(CFILES_DS) $(LIBS) -o $(OUT_DS)

$(OUT_CE): $(CFILES_CE)
	$(CXX) $(CFLAGS) $(CFILES_CE) $(LIBS) -o $(OUT_CE)

$(OUT_DM): $(CFILES_DM)
	$(CXX) $(CFLAGS) $(CFILES_DM) -o $(OUT_DM)
