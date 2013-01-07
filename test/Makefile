CXXFLAGS = -std=c++11 -O3 \
	   -Wall -Wextra -pedantic \
	   -Winit-self \
	   -Wshadow \
	   -Wformat=2 \
	   -Wmissing-declarations \
	   -Wstrict-overflow=5 \
	   -Wcast-align \
	   -Wcast-qual \
	   -Wconversion \
	   -Wunused-macros \
	   -Wwrite-strings \
	   -Weffc++
LDFLAGS = -Wl,--as-needed -s

all: test_either test_maybe

test_either: test_either.cc ../either.hh
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ $<

test_maybe: test_maybe.cc ../maybe.hh
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ $<
