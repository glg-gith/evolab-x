CXX = g++
CXXFLAGS = -g -O2 -Wall
LIBS = -lm -L./lib/libelp.so

OUT = lib/libplugins.so
ODIR = obj
SDIR = src

INC = -I. -I./inc

# Default target (the first one) : make all
all: $(OUT)


# libplugins.so
MODULES = plugins_utils.o console_temp.o 
OBJS = $(patsubst %,$(ODIR)/%,$(MODULES))

$(ODIR)/%.o: $(SDIR)/%.cpp 
	$(CXX) $(LIBS) -fPIC -c $(INC) -o $@ $< $(CXXFLAGS)

$(OUT): $(OBJS)
	$(CXX) -shared $^ -o $@


# Clean all
clean:
	rm -f $(ODIR)/*.o $(OUT)


