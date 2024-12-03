MSYS_ROOT = C:\\msys64

MKDIR = ${MSYS_ROOT}\\usr\\bin\\mkdir
RM = ${MSYS_ROOT}\\usr\\bin\\rm
CP = ${MSYS_ROOT}\\usr\\bin\\cp

CXX = g++
CFLAGS = -fdiagnostics-color=always --std=c++17 -Wall
MACRO =
INCLUDE_PATH = -I${MSYS_ROOT}\\mingw64\\include\\ffmpeg4.4 \
	-I${MSYS_ROOT}\\mingw64\\include
LIB_PATH =
LIB =
EXE_SUFFIX = .exe