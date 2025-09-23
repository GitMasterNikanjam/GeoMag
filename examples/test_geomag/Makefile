CXX      := g++
CXXFLAGS := -std=c++11 -O2 -Wall -Wextra

# ---- adjust to your tree ----
ROOT     := ../..                      # GeoMag.cpp / tables.cpp
MATHDIR  := ../../../Embed_Math        # Embed_Math.cpp / .h
COMMON   := ../../../Embed_Common      # Location.cpp / Embed_Common.cpp
# -----------------------------------

INCLUDES := -I$(COMMON) -I$(MATHDIR) -I$(ROOT)
OBJDIR   := build

# === LIST ONLY .cpp FILES BELOW ===
SRCS := \
  main.cpp \
  $(ROOT)/GeoMag.cpp \
  $(ROOT)/tables.cpp \
  $(COMMON)/Location.cpp \
  $(MATHDIR)/Embed_Math.cpp \
  $(COMMON)/Embed_Common.cpp

# derive object names in build/ from basenames
SRCNAMES := $(notdir $(SRCS))
OBJS     := $(addprefix $(OBJDIR)/,$(SRCNAMES:.cpp=.o))
TARGET   := geomag_test

# find sources in these dirs (no directories should appear as SRCS items)
vpath %.cpp $(ROOT) $(COMMON) $(MATHDIR) .

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR):
	@mkdir -p $(OBJDIR)

# compile rule
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

# debug helpers (optional)
print-%:
	@echo '$*=$($*)'

.PHONY: all clean print-%
