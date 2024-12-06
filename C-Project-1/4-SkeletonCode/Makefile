##########################################   OPTION SETTING ###########################################

OUTPUT = lsb_steg 
CCOMPILE = gcc

LINK = gcc

LINKOPTION += -o $(OUTPUT)
INCLUDEDIR += -I.

INCLUDEDIR += -I./Include/
INCLUDEDIR += -I./Include/Encode
INCLUDEDIR += -I./Include/Decode


OBJS += Encode/Encode.o
OBJS += Decode/Decode.o

OBJS += common.o

#########################################	OPTION END	 ##############################################


$(OUTPUT):$(OBJS) 

	$(LINK) $(LINKOPTION)  $(OBJS) 

clean: 
	rm -f $(OBJS)
	rm -f $(OUTPUT)

all: clean $(OUTPUT)
.PRECIOUS:%.cpp %.cc %.cxx %.c 
.SUFFIXES:
.SUFFIXES: .cpp .cc .cxx .c  .o

.cpp.o:
	$(CPPCOMPILE) -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR)  $*.cpp
	
.cc.o:
	$(CCOMPILE) -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR)  $*.cc

.cxx.o:
	$(CPPCOMPILE) -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR)  $*.cxx

.c.o:
	$(CCOMPILE) -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR) $*.c


#######################################################################################################	
