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

IMGUI_DIR = includes/imgui
SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(SRC_DIR)/be_openfunctions.cpp \
          $(SRC_DIR)/ui_styles.cpp \
          $(SRC_DIR)/ui_diskusageblock.cpp \
          $(SRC_DIR)/ui_popuprename.cpp \
          $(SRC_DIR)/ui_popupabout.cpp \
          $(SRC_DIR)/ui_popupoperations.cpp \
          $(SRC_DIR)/ui_titlebar.cpp \
          $(SRC_DIR)/ui_sidepanel.cpp \
          $(SRC_DIR)/ui_mainpanel.cpp \
          $(SRC_DIR)/main.cpp

SOURCES += $(IMGUI_DIR)/imgui.cpp \
           $(IMGUI_DIR)/imgui_demo.cpp \
           $(IMGUI_DIR)/imgui_draw.cpp \
           $(IMGUI_DIR)/imgui_tables.cpp \
           $(IMGUI_DIR)/imgui_widgets.cpp \
           $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp \
           $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

CXX = g++
CXXFLAGS = -std=c++17 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -Iincludes -g -Wall -Wformat
LDFLAGS =
LIBS =

UNAME_S := $(shell uname -s)
EXE = SimpliFile.application

ifeq ($(OS), Windows_NT)
    EXE = SimpliFile.exe
    CXXFLAGS += -I"$(GLFW_DIR)/include" -Iincludes/imgui -Iincludes/imgui/backends
    LDFLAGS  += -L"$(GLFW_DIR)/lib-mingw-w64" -lglfw3 -lgdi32 -lopengl32 -limm32
endif

ifeq ($(UNAME_S), Linux)
    LIBS += -lGL `pkg-config --static --libs glfw3`
    CXXFLAGS += `pkg-config --cflags glfw3`
endif

ifeq ($(UNAME_S), Darwin)
    EXE = SimpliFile
    CXX = clang++

    BREW_PREFIX := $(shell brew --prefix glfw)

    CXXFLAGS += \
        -std=c++17 \
        -DGL_SILENCE_DEPRECATION \
        -I$(BREW_PREFIX)/include \
        -I$(IMGUI_DIR) \
        -I$(IMGUI_DIR)/backends

    LDFLAGS += \
        -L$(BREW_PREFIX)/lib \
        -lglfw \
        -framework OpenGL \
        -framework Cocoa \
        -framework IOKit \
        -framework CoreVideo
endif

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

ifeq ($(UNAME_S), Darwin)

bundle-macos: $(EXE)
	@rm -rf SimpliFile.app
	@mkdir -p SimpliFile.app/Contents/MacOS
	@cp SimpliFile SimpliFile.app/Contents/MacOS/SimpliFile
	@cp -r includes/ SimpliFile.app/Contents/MacOS/includes/
	@mkdir -p SimpliFile.app/Contents

	@printf '%s\n' \
'<?xml version="1.0" encoding="UTF-8"?>' \
'<plist version="1.0">' \
'<dict>' \
'    <key>CFBundleName</key>' \
'    <string>SimpliFile</string>' \
'    <key>CFBundleExecutable</key>' \
'    <string>SimpliFile</string>' \
'    <key>CFBundleIdentifier</key>' \
'    <string>com.example.simplifile</string>' \
'    <key>CFBundleVersion</key>' \
'    <string>1.0</string>' \
'    <key>CFBundlePackageType</key>' \
'    <string>APPL</string>' \
'</dict>' \
'</plist>' \
> SimpliFile.app/Contents/Info.plist

endif

clean:
	rm -rf $(OBJ_DIR) SimpliFile.application SimpliFile.app SimpliFile.exe
