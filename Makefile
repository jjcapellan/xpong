.PHONY: all clean

PROJECT_NAME          ?= xpong
RAYLIB_VERSION        ?= 4.2.0
RAYLIB_PATH           ?= ../raylib

# Path to raylib headers (*.h)
RAYLIB_HEADERS_PATH   ?= $(RAYLIB_PATH)/src

# Path to raylib compilations
RAYLIB_BIN_PATH       ?= /usr/local/lib
# Paths to raylib compilations
RAYLIB_DESKTOP_STATIC ?= $(RAYLIB_BIN_PATH)/libraylib.a
RAYLIB_DESKTOP_SHARED ?= $(RAYLIB_BIN_PATH)/libraylib.so.4.2.0
RAYLIB_WEB_STATIC     ?= $(RAYLIB_BIN_PATH)/libraylib_web.a

# Project paths
PROJECT_ROOT_PATH = .
PROJECT_SOURCE_PATH   ?= ./src
PROJECT_INCLUDE_PATH  ?= ./include
PROJECT_ASSETS_PATH   = ./dist/desktop/assets@/assets

# Web shell path
# Default raylib shell $(RAYLIB_PATH)/src/shell.html
# Default shell $(PROJECT_ROOT_PATH)/shell.html 
WEB_SHELL_PATH ?= $(PROJECT_ROOT_PATH)/shell.html

# Library type used for raylib: STATIC (.a) or SHARED (.so/.dll)
RAYLIB_LIBTYPE        ?= STATIC

# Build mode for project: DEBUG or RELEASE
BUILD_MODE            ?= RELEASE

# Use external GLFW library instead of rglfw module
# TODO: Review usage on Linux. Target version of choice. Switch on -lglfw or -lglfw3
USE_EXTERNAL_GLFW     ?= FALSE

# Use Wayland display server protocol on Linux desktop
# by default it uses X11 windowing system
USE_WAYLAND_DISPLAY   ?= FALSE

# Target platform: PLATFORM_DESKTOP, PLATFORM_WEB
PLATFORM              ?= PLATFORM_DESKTOP


# raylib compiled file used
ifeq ($(RAYLIB_LIBTYPE),SHARED)
	RAYLIB_LIB = $(RAYLIB_DESKTOP_SHARED)
else
	ifeq ($(PLATFORM),PLATFORM_WEB)
		RAYLIB_LIB = $(RAYLIB_WEB_STATIC)
	else
		RAYLIB_LIB = $(RAYLIB_DESKTOP_STATIC)
	endif
endif

# PLATFORM_OS 
ifeq ($(OS),Windows_NT)
	PLATFORM_OS=WINDOWS
else
	UNAMEOS=$(shell uname)
	ifeq ($(UNAMEOS),Linux)
		PLATFORM_OS=LINUX
	endif
endif


# ********** COMPILER
CC = gcc
ifeq ($(PLATFORM),PLATFORM_WEB)
	# HTML5 emscripten compiler
	CC = emcc
endif


# ********** COMPILER FLAGS

CFLAGS += -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	ifeq ($(BUILD_MODE),DEBUG)
		CFLAGS += -g -DDEBUG
	else
		CFLAGS += -s -O1
	endif
	ifeq ($(PLATFORM_OS),LINUX)
		ifeq ($(RAYLIB_LIBTYPE),SHARED)
			# Explicitly enable runtime link to libraylib.so
			CFLAGS += -Wl,-rpath,$(RAYLIB_BIN_PATH)
		endif
	endif
endif

ifeq ($(PLATFORM),PLATFORM_WEB)
	ifeq ($(BUILD_MODE),DEBUG)
		CFLAGS += -g -DDEBUG -s ASSERTIONS=1 --profiling
	else
		CFLAGS += -Os
	endif
	CFLAGS += -s LEGACY_RUNTIME=1 -s USE_GLFW=3 -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 \
	--preload-file $(PROJECT_ASSETS_PATH) --shell-file $(WEB_SHELL_PATH)
endif

# ************* OUTPUT FILE EXTENSION

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	ifeq ($(PLATFORM_OS), WINDOWS)
		EXT = .exe
	endif
	ifeq ($(PLATFORM_OS), LINUX)
		EXT = .out
	endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
	EXT = .html
endif

# ************* -I FLAG

INCLUDE_PATHS = -I./src -I$(PROJECT_INCLUDE_PATH) -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external

# ************* -L FLAG

LDFLAGS = -L$(RAYLIB_BIN_PATH)

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	ifeq ($(PLATFORM_OS),WINDOWS)
		# resource file contains windows executable icon and properties
		LDFLAGS += $(RAYLIB_PATH)/src/raylib.rc.data
		# -Wl,--subsystem,windows hides the console window
		ifeq ($(BUILD_MODE), RELEASE)
			LDFLAGS += -Wl,--subsystem,windows
		endif
	endif
endif

# ************* -l FLAG

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	ifeq ($(PLATFORM_OS),WINDOWS)
		LDLIBS = $(RAYLIB_LIB) -lopengl32 -lgdi32 -lwinmm
		LDLIBS += -static -lpthread
	endif
	ifeq ($(PLATFORM_OS),LINUX)
		# NOTE: Required packages: libegl1-mesa-dev
		LDLIBS = $(RAYLIB_LIB) -lGL -lm -lpthread -ldl -lrt

		# On X11 requires also below libraries
		LDLIBS += -lX11
		# NOTE: It seems additional libraries are not required any more, latest GLFW just dlopen them
		#LDLIBS += -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor

		# On Wayland windowing system, additional libraries requires
		ifeq ($(USE_WAYLAND_DISPLAY),TRUE)
			LDLIBS += -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon
		endif
		# Explicit link to libc
		ifeq ($(RAYLIB_LIBTYPE),SHARED)
			LDLIBS += -lc
		endif
	endif
	ifeq ($(USE_EXTERNAL_GLFW),TRUE)
		# NOTE: It could require additional packages installed: libglfw3-dev
		LDLIBS += -lglfw
	endif
endif

ifeq ($(PLATFORM),PLATFORM_WEB)
	LDLIBS = $(RAYLIB_LIB)
endif

# ************* DEFINE FILES USED IN COMPILATION
PROJECT_SOURCE_FILES ?= \
	./src/main.c \
	./src/scene_gameover.c \
	./src/scene_gameplay.c \
	./src/scene_title.c \
	./src/scene_transition.c \
	./src/gameplay/player.c \
	./src/gameplay/npc.c \
	./src/gameplay/ball.c \
	./src/gameplay/particles.c

OBJS = $(addprefix ./obj/,$(notdir $(patsubst %.c, %.o, $(PROJECT_SOURCE_FILES))))

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	EXE = ./dist/desktop/$(PROJECT_NAME)$(EXT)
else
	EXE = ./dist/web/$(PROJECT_NAME)$(EXT)
endif


# Default target
all:$(EXE)
	@echo All done!!

# Link
$(EXE):$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(CFLAGS) $(LDLIBS) -D$(PLATFORM) -o $(EXE)

# Compilation
./obj/%.o: ./src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE_PATHS) -D$(PLATFORM)

./obj/player.o: ./src/gameplay/player.c
	echo subdir
	$(CC) $(CFLAGS) -c $^ -o $@ $(INCLUDE_PATHS) -D$(PLATFORM)

./obj/npc.o: ./src/gameplay/npc.c
	echo subdir
	$(CC) $(CFLAGS) -c $^ -o $@ $(INCLUDE_PATHS) -D$(PLATFORM)

./obj/ball.o: ./src/gameplay/ball.c
	echo subdir
	$(CC) $(CFLAGS) -c $^ -o $@ $(INCLUDE_PATHS) -D$(PLATFORM)

./obj/particles.o: ./src/gameplay/particles.c
	echo subdir
	$(CC) $(CFLAGS) -c $^ -o $@ $(INCLUDE_PATHS) -D$(PLATFORM)

# Deletes object files
clean:
ifeq ($(PLATFORM_OS),WINDOWS)
	del obj/*.o /s
endif
ifeq ($(PLATFORM_OS),LINUX)
	rm -fv ./obj/*.o
endif



