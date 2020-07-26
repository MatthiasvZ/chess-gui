#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS = -Wall -std=c++20 -Iinclude -fexceptions -pipe -march=native
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = -lglfw -lGLEW -lX11 -lGLU -lGL

INC_DEBUG = $(INC) -Iinclude
CFLAGS_DEBUG = $(CFLAGS) -Og -g -DDEBUG
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/chess

INC_RELEASE = $(INC) -Iinclude
CFLAGS_RELEASE = $(CFLAGS) -Ofast
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj
DEP_RELEASE = 
OUT_RELEASE = bin/chess

OBJ_DEBUG = $(OBJDIR_DEBUG)/src/Tile.o $(OBJDIR_DEBUG)/src/vendor/stb_image/stb_image.o $(OBJDIR_DEBUG)/src/other/FileManagement.o $(OBJDIR_DEBUG)/src/other/ErrorFeedback.o $(OBJDIR_DEBUG)/src/gl/Window.o $(OBJDIR_DEBUG)/src/gl/VertexBufferLayout.o $(OBJDIR_DEBUG)/src/gl/VertexBuffer.o $(OBJDIR_DEBUG)/src/gl/VertexArray.o $(OBJDIR_DEBUG)/src/gl/Texture.o $(OBJDIR_DEBUG)/src/gl/Shader.o $(OBJDIR_DEBUG)/src/gl/Renderer.o $(OBJDIR_DEBUG)/src/gl/IndexBuffer.o $(OBJDIR_DEBUG)/Main.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/src/Tile.o $(OBJDIR_RELEASE)/src/vendor/stb_image/stb_image.o $(OBJDIR_RELEASE)/src/other/FileManagement.o $(OBJDIR_RELEASE)/src/other/ErrorFeedback.o $(OBJDIR_RELEASE)/src/gl/Window.o $(OBJDIR_RELEASE)/src/gl/VertexBufferLayout.o $(OBJDIR_RELEASE)/src/gl/VertexBuffer.o $(OBJDIR_RELEASE)/src/gl/VertexArray.o $(OBJDIR_RELEASE)/src/gl/Texture.o $(OBJDIR_RELEASE)/src/gl/Shader.o $(OBJDIR_RELEASE)/src/gl/Renderer.o $(OBJDIR_RELEASE)/src/gl/IndexBuffer.o $(OBJDIR_RELEASE)/Main.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG)/src || mkdir -p $(OBJDIR_DEBUG)/src
	test -d $(OBJDIR_DEBUG)/src/vendor/stb_image || mkdir -p $(OBJDIR_DEBUG)/src/vendor/stb_image
	test -d $(OBJDIR_DEBUG)/src/other || mkdir -p $(OBJDIR_DEBUG)/src/other
	test -d $(OBJDIR_DEBUG)/src/gl || mkdir -p $(OBJDIR_DEBUG)/src/gl
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/src/Tile.o: src/Tile.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Tile.cpp -o $(OBJDIR_DEBUG)/src/Tile.o

$(OBJDIR_DEBUG)/src/vendor/stb_image/stb_image.o: src/vendor/stb_image/stb_image.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/vendor/stb_image/stb_image.cpp -o $(OBJDIR_DEBUG)/src/vendor/stb_image/stb_image.o

$(OBJDIR_DEBUG)/src/other/FileManagement.o: src/other/FileManagement.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/other/FileManagement.cpp -o $(OBJDIR_DEBUG)/src/other/FileManagement.o

$(OBJDIR_DEBUG)/src/other/ErrorFeedback.o: src/other/ErrorFeedback.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/other/ErrorFeedback.cpp -o $(OBJDIR_DEBUG)/src/other/ErrorFeedback.o

$(OBJDIR_DEBUG)/src/gl/Window.o: src/gl/Window.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gl/Window.cpp -o $(OBJDIR_DEBUG)/src/gl/Window.o

$(OBJDIR_DEBUG)/src/gl/VertexBufferLayout.o: src/gl/VertexBufferLayout.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gl/VertexBufferLayout.cpp -o $(OBJDIR_DEBUG)/src/gl/VertexBufferLayout.o

$(OBJDIR_DEBUG)/src/gl/VertexBuffer.o: src/gl/VertexBuffer.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gl/VertexBuffer.cpp -o $(OBJDIR_DEBUG)/src/gl/VertexBuffer.o

$(OBJDIR_DEBUG)/src/gl/VertexArray.o: src/gl/VertexArray.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gl/VertexArray.cpp -o $(OBJDIR_DEBUG)/src/gl/VertexArray.o

$(OBJDIR_DEBUG)/src/gl/Texture.o: src/gl/Texture.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gl/Texture.cpp -o $(OBJDIR_DEBUG)/src/gl/Texture.o

$(OBJDIR_DEBUG)/src/gl/Shader.o: src/gl/Shader.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gl/Shader.cpp -o $(OBJDIR_DEBUG)/src/gl/Shader.o

$(OBJDIR_DEBUG)/src/gl/Renderer.o: src/gl/Renderer.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gl/Renderer.cpp -o $(OBJDIR_DEBUG)/src/gl/Renderer.o

$(OBJDIR_DEBUG)/src/gl/IndexBuffer.o: src/gl/IndexBuffer.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gl/IndexBuffer.cpp -o $(OBJDIR_DEBUG)/src/gl/IndexBuffer.o

$(OBJDIR_DEBUG)/Main.o: Main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c Main.cpp -o $(OBJDIR_DEBUG)/Main.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)/src
	rm -rf $(OBJDIR_DEBUG)/src/vendor/stb_image
	rm -rf $(OBJDIR_DEBUG)/src/other
	rm -rf $(OBJDIR_DEBUG)/src/gl
	rm -rf $(OBJDIR_DEBUG)

before_release: 
	test -d bin || mkdir -p bin
	test -d $(OBJDIR_RELEASE)/src || mkdir -p $(OBJDIR_RELEASE)/src
	test -d $(OBJDIR_RELEASE)/src/vendor/stb_image || mkdir -p $(OBJDIR_RELEASE)/src/vendor/stb_image
	test -d $(OBJDIR_RELEASE)/src/other || mkdir -p $(OBJDIR_RELEASE)/src/other
	test -d $(OBJDIR_RELEASE)/src/gl || mkdir -p $(OBJDIR_RELEASE)/src/gl
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/src/Tile.o: src/Tile.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Tile.cpp -o $(OBJDIR_RELEASE)/src/Tile.o

$(OBJDIR_RELEASE)/src/vendor/stb_image/stb_image.o: src/vendor/stb_image/stb_image.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/vendor/stb_image/stb_image.cpp -o $(OBJDIR_RELEASE)/src/vendor/stb_image/stb_image.o

$(OBJDIR_RELEASE)/src/other/FileManagement.o: src/other/FileManagement.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/other/FileManagement.cpp -o $(OBJDIR_RELEASE)/src/other/FileManagement.o

$(OBJDIR_RELEASE)/src/other/ErrorFeedback.o: src/other/ErrorFeedback.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/other/ErrorFeedback.cpp -o $(OBJDIR_RELEASE)/src/other/ErrorFeedback.o

$(OBJDIR_RELEASE)/src/gl/Window.o: src/gl/Window.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gl/Window.cpp -o $(OBJDIR_RELEASE)/src/gl/Window.o

$(OBJDIR_RELEASE)/src/gl/VertexBufferLayout.o: src/gl/VertexBufferLayout.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gl/VertexBufferLayout.cpp -o $(OBJDIR_RELEASE)/src/gl/VertexBufferLayout.o

$(OBJDIR_RELEASE)/src/gl/VertexBuffer.o: src/gl/VertexBuffer.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gl/VertexBuffer.cpp -o $(OBJDIR_RELEASE)/src/gl/VertexBuffer.o

$(OBJDIR_RELEASE)/src/gl/VertexArray.o: src/gl/VertexArray.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gl/VertexArray.cpp -o $(OBJDIR_RELEASE)/src/gl/VertexArray.o

$(OBJDIR_RELEASE)/src/gl/Texture.o: src/gl/Texture.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gl/Texture.cpp -o $(OBJDIR_RELEASE)/src/gl/Texture.o

$(OBJDIR_RELEASE)/src/gl/Shader.o: src/gl/Shader.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gl/Shader.cpp -o $(OBJDIR_RELEASE)/src/gl/Shader.o

$(OBJDIR_RELEASE)/src/gl/Renderer.o: src/gl/Renderer.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gl/Renderer.cpp -o $(OBJDIR_RELEASE)/src/gl/Renderer.o

$(OBJDIR_RELEASE)/src/gl/IndexBuffer.o: src/gl/IndexBuffer.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gl/IndexBuffer.cpp -o $(OBJDIR_RELEASE)/src/gl/IndexBuffer.o

$(OBJDIR_RELEASE)/Main.o: Main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c Main.cpp -o $(OBJDIR_RELEASE)/Main.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin
	rm -rf $(OBJDIR_RELEASE)/src
	rm -rf $(OBJDIR_RELEASE)/src/vendor/stb_image
	rm -rf $(OBJDIR_RELEASE)/src/other
	rm -rf $(OBJDIR_RELEASE)/src/gl
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

