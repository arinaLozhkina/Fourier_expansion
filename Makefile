EXENAME:=fourrier
SRCs:=$(wildcard *.cpp)
OBJs:=$(SRCs:.cpp=.o)
LIBS="-lginac"
CXXFLAGS="c++0x"
$(EXENAME):$(OBJs)
	@mpic++ -std=$(CXXFLAGS) -o $@ $^ $(LIBS)
%.o:%.cpp
	@mpic++ -std=$(CXXFLAGS) -o $@ -c $<$(CFLAGS)
	@echo "\033[92mCompiling $^\033[0m"
