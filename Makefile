GG := g++
FLAGS := -std=c++11 -Ofast

SRCS = collisions.cpp
BUILDDIR = build
OUT = $(BUILDDIR)/collisions

all:
	@mkdir -p $(BUILDDIR)
	$(GG) $(FLAGS) $(SRCS) -o $(OUT)

clean:
	rm $(BUILDDIR)/$(OUT)
