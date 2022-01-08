CC = ../../simgrid-3.28/install/bin/smpicc
CXX = ../../simgrid-3.28/install/bin/smpicxx
vgg-df: 
	$(CC) VGG16.c ../collective-operations/2lvfc.c -o VGG16.run

vgg-cmb:
	$(CXX) -c VGG16.c
	$(CXX) -std=c++0x -c ../collective-operations/kautz-cmb.cpp
	$(CXX) VGG16.o kautz-cmb.o -o vgg-cmb.run
 
vgg-mttree:
	$(CXX) -c VGG16.c
	$(CXX) -std=c++0x -c ../collective-operations/kautz-mttree.cpp
	$(CXX) VGG16.o kautz-cmb.o -o vgg-mttree.run

vgg-conges:
	$(CXX) -c VGG16.c
	$(CXX) -std=c++0x -c ../collective-operations/kautz-conges.cpp
	$(CXX) VGG16.o kautz-cmb.o -o vgg-conges.run

resnet: 
	../../simgrid-3.28/install/bin/smpicc RESNET50.c -o RESNET50.run
	
clean:
	/bin/rm -f *.run
	/bin/rm -f *.log*
	/bin/rm -f smpitmp*
	/bin/rm -f *.o
