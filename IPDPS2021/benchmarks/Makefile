CC = ../../simgrid-3.28/install/bin/smpicc
CXX = ../../simgrid-3.28/install/bin/smpicxx
vgg-df: 
	$(CC) VGG16.c ../collective-operations/2lvfc.c -o VGG16.o

vgg-mttree:
	
resnet: 
	../../simgrid-3.28/install/bin/smpicc RESNET50.c -o RESNET50.run
	
clean:
	/bin/rm -f *.run
	/bin/rm -f *.log*
	/bin/rm -f smpitmp*

