NVCC=nvcc 
CXX=g++
CXXFLAGS=-I./include -std=c++14 -Wall -g -fPIC
CVCXXFLAGS=`pkg-config --cflags --libs opencv4`
OBJDIR=objs
SRCDIR=src

CUDA_LINK_FLAGS =  -rdc=true -gencode=arch=compute_61,code=sm_61 -Xcompiler '-fPIC'
CUDA_COMPILE_FLAGS = --device-c -gencode=arch=compute_61,code=sm_61 -Xcompiler '-fPIC' -g -O3

# g++ -std=c++14 -ggdb main.cpp ./src/*.cpp -o main -I./include `pkg-config --cflags --libs opencv4`



default: bfs_boosting

OBJS= $(OBJDIR)/board.o $(OBJDIR)/cell.o $(OBJDIR)/cpusolver.o $(OBJDIR)/gpusolver.o $(OBJDIR)/kernel.o $(OBJDIR)/kruskal.o $(OBJDIR)/maze.o $(OBJDIR)/wall.o $(OBJDIR)/main.o

bfs_boosting: $(OBJS)
	$(NVCC) $(CUDA_LINK_FLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

# $(OBJDIR)/render.o: $(SRCDIR)/render.cpp
# 	$(CXX) $< $(CXXFLAGS) $(CVCXXFLAGS) -c -o $@

$(OBJDIR)/kernel.o: $(SRCDIR)/kernel.cu
	${NVCC} ${CUDA_COMPILE_FLAGS} -c $(SRCDIR)/kernel.cu -o $@


# compile_main: main.cpp $(OBJDIR)
# 	$(CXX) main.cpp $(CXXFLAGS) -c -o $(OBJDIR)/main.o

# compile_cpp: $(SRCDIR)/%.cpp
# 	$(CXX) $< $(CXXFLAGS) -c -o $(OBJDIR)/$@

clean:
	rm -rf  $(OBJDIR)*.o bfs_boosting;