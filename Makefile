NVCC=nvcc 
CXX=g++

CXXFLAGS=-I./include -std=c++14 -Wall -g -fPIC
CXXFLAGS_CPU=-I./include -std=c++14 -Wall -g -fPIC -pthread -fopenmp

OBJDIR_GPU=objs/gpu
OBJDIR_CPU=objs/cpu

SRCDIR=src


CUDA_LINK_FLAGS =  -rdc=true -gencode=arch=compute_61,code=sm_61 -Xcompiler '-fPIC'
CUDA_COMPILE_FLAGS = -I./include --device-c -gencode=arch=compute_61,code=sm_61 -Xcompiler '-fPIC' -g -O3


cpu: bfs_cpu
gpu: bfs_gpu

OBJS_CPU= $(OBJDIR_CPU)/board.o $(OBJDIR_CPU)/cell.o $(OBJDIR_CPU)/cpusolver.o $(OBJDIR_CPU)/kruskal.o $(OBJDIR_CPU)/maze.o $(OBJDIR_CPU)/wall.o $(OBJDIR_CPU)/main_cpu.o
OBJS_GPU= $(OBJDIR_GPU)/board.o $(OBJDIR_GPU)/cell.o $(OBJDIR_GPU)/gpusolver.o $(OBJDIR_GPU)/kernel.o $(OBJDIR_GPU)/kruskal.o $(OBJDIR_GPU)/maze.o $(OBJDIR_GPU)/wall.o $(OBJDIR_GPU)/main_gpu.o


bfs_cpu: $(OBJS_CPU)
	$(CXX) $(CXXFLAGS_CPU) -o $@ $(OBJS_CPU)

$(OBJDIR_CPU)/%.o: %.cpp
	$(CXX) $< $(CXXFLAGS_CPU) -c -o $@

$(OBJDIR_CPU)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $< $(CXXFLAGS_CPU) -c -o $@

clean_cpu:
	rm -rf $(OBJDIR_CPU)/*.o bfs_cpu






bfs_gpu: $(OBJS_GPU)
	$(NVCC) $(CUDA_LINK_FLAGS) -o $@ $(OBJS_GPU)


$(OBJDIR_GPU)/%.o: %.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

$(OBJDIR_GPU)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

$(OBJDIR_GPU)/kernel.o: $(SRCDIR)/kernel.cu
	${NVCC} ${CUDA_COMPILE_FLAGS} -c $(SRCDIR)/kernel.cu -o $@

clean_gpu:
	rm -rf  $(OBJDIR_GPU)/*.o bfs_gpu;
