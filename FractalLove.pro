# -------------------------------------------------
# Project created by QtCreator 2009-01-10T15:38:22
# -------------------------------------------------
QT += opengl
TARGET = FractalLove
CONFIG += app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    MainWindow.cpp \
    FractalController.cpp \
    Shaders/Shader.cpp \
    Shaders/MandelbrotShaderMP.cpp \
    CompositionWidget.cpp \
    Shaders/MandelbrotInter.cpp \
    Shaders/PerlinNoiseShader.cpp \
    Shaders/JuliaOrbitalTrap.cpp \
    Shaders/OpenGLShader.cpp \
    Shaders/JuliaOrbitalTrapMP.cpp \
    Shaders/MandelbrotOrbitTrap.cpp \
    Shaders/MandelbrotImageTrap.cpp \
    ../fractallove/Shaders/CurlicueFractal.cpp \
    ../fractallove/Shaders/Mobius.cpp \
    ../fractallove/Shaders/ApollonianGasket.cpp
HEADERS += MainWindow.h \
    main.h \
    FractalController.h \
    Shaders/Shader.h \
    Shaders/MandelbrotShaderMP.h \
    CompositionWidget.h \
    Shaders/MandelbrotInter.h \
    Shaders/PerlinNoiseShader.h \
    Shaders/JuliaOrbitalTrap.h \
    Shaders/OpenGLShader.h \
    Shaders/JuliaOrbitalTrapMP.h \
    Shaders/MandelbrotOrbitTrap.h \
    Shaders/MandelbrotImageTrap.h \
    ../fractallove/Shaders/CurlicueFractal.h \
    ../fractallove/Shaders/Mobius.h \
    ../fractallove/Shaders/ApollonianGasket.h
QMAKE_CXX = i686-apple-darwin9-g++-4.2.1
QMAKE_CC = i686-apple-darwin9-gcc-4.2.1
QMAKE_LINK = i686-apple-darwin9-g++-4.2.1
QMAKE_CXXFLAGS = -fopenmp \
    -DGL_GLEXT_PROTOTYPES \
    -msse2 \
    -mfpmath=sse \
    -funroll-loops \
    -fomit-frame-pointer
QMAKE_LFLAGS = -fopenmp
