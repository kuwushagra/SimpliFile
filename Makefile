#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

#CXX = g++
#CXX = clang++

forkname = "v"
version = "1.2.0"
revision = ""
buildnumber = ${forkname}${version}${revision}
EXE = SimpliFile.application

#---------------------------------------------------------------------
# Directories
#---------------------------------------------------------------------

IMGUI_DIR = includes/imgui
SRC_DIR = src
OBJ_DIR = obj

#---------------------------------------------------------------------
# Sources
#---------------------------------------------------------------------

SOURCES = $(SRC_DIR)/be_openfunctions.cpp \
          $(SRC_DIR)/ui_styles.cpp \
          $(SRC_DIR)/ui_diskusageblock.cpp \
          $(SRC_DIR)/ui_popuprename.cpp\
          $(SRC_DIR)/ui_popupabout.cpp\
          $(SRC_DIR)/ui_popupoperations.cpp\
          $(SRC_DIR)/ui_titlebar.cpp\
          $(SRC_DIR)/ui_sidepanel.cpp\
          $(SRC_DIR)/ui_mainpanel.cpp\
          $(SRC_DIR)/main.cpp 

SOURCES += $(IMGUI_DIR)/imgui.cpp \
           $(IMGUI_DIR)/imgui_demo.cpp \
           $(IMGUI_DIR)/imgui_draw.cpp \
           $(IMGUI_DIR)/imgui_tables.cpp \
           $(IMGUI_DIR)/imgui_widgets.cpp \
           $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp \
           $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

#---------------------------------------------------------------------
# Objects
#---------------------------------------------------------------------

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

#---------------------------------------------------------------------
# Compiler and flags
#---------------------------------------------------------------------

CXX = g++
CXXFLAGS = -std=c++17 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -g -Wall -Wformat

UNAME_S := $(shell uname -s)
LIBS =

LINUX_GL_LIBS = -lGL

ifeq ($(UNAME_S), Linux)
    LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`
    CXXFLAGS += `pkg-config --cflags glfw3`
endif

ifeq ($(UNAME_S), Darwin)
    LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lglfw
    CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
endif

ifeq ($(OS), Windows_NT)
    LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32
    CXXFLAGS += `pkg-config --cflags glfw3`
endif

#---------------------------------------------------------------------
# Build rules
#---------------------------------------------------------------------

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Link
$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(EXE)
