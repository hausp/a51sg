# Created by aszdrick graf <aszdrick@gmail.com>
# Compiler
CXX       :=gcc
LDLIBS    :=-lstdc++ -lm
LDFLAGS   :=
CXXFLAGS  :=`pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -std=c++11 -Wall
# Source directory
SRCDIR    :=src
# Headers directory
HDRDIR    :=include
# Build directory
BUILDIR   :=build
# Binaries directory
BINDIR    :=bin
#Include flag
INCLUDE   :=-I$(HDRDIR) -I$(HDRDIR)/shapes
# Sources
SRC       :=$(shell find $(SRCDIR) -name '*.cpp')
# Dependencies
DEP       :=$(SRC:.cpp=.d) 
# Objects
OBJ       :=$(patsubst $(SRCDIR)/%.cpp,$(BUILDIR)/%.o,$(SRC))
# Program executable
EXEC      :=$(BINDIR)/shapes

.PHONY: all makedir clean clean_all

all: makedir $(EXEC)

$(EXEC): $(OBJ)
	@echo "[linking] $(EXEC)"
	@$(CXX) $(OBJ) -o $@ $(LDLIBS) $(LDFLAGS) $(CXXFLAGS)

$(BUILDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "[  $(CXX)  ] $< -> .o"
	@mkdir -p $(BUILDIR)/$(*D)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

makedir: | $(BUILDIR) $(BINDIR)

$(BINDIR) $(BUILDIR):
	@echo "[ mkdir ] Creating directory '$@'"
	@mkdir -p $@

# For each .cpp file, creates a .d file with all dependencies of .cpp,
# including .d as target (to ensure updated dependencies, in case of
# adding a new include or nested include)
$(SRCDIR)/%.d: $(SRCDIR)/%.cpp
	@echo "[makedep] $< -> .d"
	@$(CXX) -MM -MP -MT "$(BUILDIR)/$*.o $@" -MF "$@" $< $(INCLUDE) $(CXXFLAGS)

# Only remove object files
clean:
	@$(RM) -r $(BUILDIR)

# Remove object, binary and dependency files
clean_all:
	@$(RM) -r $(BUILDIR)
	@$(RM) -r $(BINDIR)
	@$(RM) -r $(DEP)

# Do not include list of dependencies if make was called with target clean_all
ifneq ($(MAKECMDGOALS), clean_all)
-include $(DEP)
endif