NULL =

CXX ?= g++
CPPFLAGS += -I src
CXXFLAGS ?= -g -O2
CXXFLAGS += -std=c++11

BLAS_LIBS ?= -lopenblas
LIBS += $(BLAS_LIBS)

SRCS =				\
	src/cg.cpp		\
	src/main.cpp		\
	src/matrix/heat.cpp	\
	src/matrix/id.cpp	\
	src/matrix/int.cpp	\
	src/matrix/intgrad.cpp	\
	src/matrix.cpp		\
	src/mesh.cpp		\
	src/vector.cpp		\
	$(NULL)

OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

all:

mesh.msh: ff++/mesh.edp
	FreeFem++ ff++/mesh.edp

%.o: %.cpp
	$(CXX) -c -MMD -MP $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

main: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

all: mesh.msh main
.PHONY: all

control-c++.dat heat-c++.dat: c++.intermediate
c++.intermediate: mesh.msh main
	./main
.INTERMEDIATE: c++.intermediate

control-ff++.dat heat-ff++.dat: ff++.intermediate
ff++.intermediate: ff++/main.edp
	FreeFem++ ff++/main.edp
.INTERMEDIATE: ff++.intermediate

%-err.dat: %-c++.dat %-ff++.dat python/err.py
	python/err.py $*-c++.dat $*-ff++.dat > $@

control-c++.gif: control-c++.dat gnuplot/plot.gnu
	gnuplot -e "basename='control-c++'" -e "legend='Optimal Control (C++)'" gnuplot/plot.gnu 2> /dev/null

heat-c++.gif: heat-c++.dat gnuplot/plot.gnu
	gnuplot -e "basename='heat-c++'" -e "legend='Heat (C++)'" gnuplot/plot.gnu 2> /dev/null

control-ff++.gif: control-ff++.dat gnuplot/plot.gnu
	gnuplot -e "basename='control-ff++'" -e "legend='Optimal Control (FreeFem++)'" gnuplot/plot.gnu 2> /dev/null

heat-ff++.gif: heat-ff++.dat gnuplot/plot.gnu
	gnuplot -e "basename='heat-ff++'" -e "legend='Heat (FreeFem++)'" gnuplot/plot.gnu 2> /dev/null

control-err.gif: control-err.dat gnuplot/plot.gnu
	gnuplot -e "basename='control-err'" -e "legend='Optimal Control (Error)'" gnuplot/plot.gnu 2> /dev/null

heat-err.gif: heat-err.dat gnuplot/plot.gnu
	gnuplot -e "basename='heat-err'" -e "legend='Heat (Error)'" gnuplot/plot.gnu 2> /dev/null

plot-c++: control-c++.gif heat-c++.gif
	@echo "Now open \`control-c++.gif' and \`heat-c++.gif'."
.PHONY: plot-c++

plot-ff++: control-ff++.gif heat-ff++.gif
	@echo "Now open \`control-ff++.gif' and \`heat-ff++.gif'."
.PHONY: plot-ff++

plot-err: control-err.gif heat-err.gif
	@echo "Now open \`control-err.gif' and \`heat-err.gif'."
.PHONY: plot-err

clean:
	-rm -rf main
	-rm -rf src/*.[do]
	-rm -rf src/matrix/*.[do]
	-rm -rf mesh.msh
	-rm -rf *.dat
	-rm -rf *.gif
.PHONY: clean

-include $(DEPS)
