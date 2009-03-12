# -------------------------------------------------
# Project created by QtCreator 2009-01-10T15:38:22
# -------------------------------------------------
QT += opengl
TARGET = FractalLove
CONFIG += app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    MainWindow.cpp \
    GLWidget.cpp \
    FractalController.cpp \
    Shaders/Shader.cpp \
    Shaders/MandelbrotVelocity.cpp \
    Shaders/NebulabrotShaderMP.cpp \
    Shaders/MandelbrotShaderMP.cpp \
    Shaders/MandelbrotBoostShaderMP.cpp \
    Shaders/MandelbrotAvgShader.cpp \
    Shaders/BuddhaJuliaShaderMP.cpp \
    Shaders/BuddhabrotShaderMP.cpp \
    CompositionWidget.cpp \
    Shaders/MandelbrotInter.cpp
HEADERS += MainWindow.h \
    main.h \
    GLWidget.h \
    FractalController.h \
    Shaders/Shader.h \
    \ \ # Shaders/MandelbrotVelocity.h \
# Shaders/NebulabrotShaderMP.h \
    Shaders/MandelbrotShaderMP.h \
    \ \ # Shaders/MandelbrotBoostShaderMP.h \
# Shaders/MandelbrotAvgShader.h \
# Shaders/BuddhaJuliaShaderMP.h \
# Shaders/BuddhabrotShaderMP.h \
    CompositionWidget.h \
    Shaders/MandelbrotInter.h

# QMAKE_CXX = i686-apple-darwin9-g++-4.2.1
# QMAKE_CC = i686-apple-darwin9-gcc-4.2.1
QMAKE_CXXFLAGS = -fopenmp
QMAKE_LFLAGS = -fopenmp
