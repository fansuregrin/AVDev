MSYS_ROOT = C:\\msys64

MKDIR = ${MSYS_ROOT}\\usr\\bin\\mkdir
RM = ${MSYS_ROOT}\\usr\\bin\\rm
CP = ${MSYS_ROOT}\\usr\\bin\\cp

CXX = g++
CFLAGS = -fdiagnostics-color=always --std=c++14 -Wall
MACRO =
INCLUDE_PATH = -I${MSYS_ROOT}\\mingw64\\include\\ffmpeg4.4
LIB_PATH = -L${MSYS_ROOT}\\mingw64\\bin
LIB = -lavcodec-58 -lavformat-58 -lavutil-56
EXE_SUFFIX = .exe