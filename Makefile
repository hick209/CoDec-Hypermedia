CC = g++
C_FLAGS =
CPP_FLAGS = -I. -Wall --pedantic-errors
OPTIMIZATION_FLAGS = -O3
LD_FLAGS = -L.
MAKE = make
AR = ar

PROGRAM_NAME = codecFuckingFodaCodec

DEFINES = -DPROGRAM_NAME=\"$(PROGRAM_NAME)\"

bin = main

algorithms = \
        WaveReader \
        WaveWriter \
        FUF \
        FormatoUltraFodaData \
        Huffman \
        Differencial \
        Transform \
        Utils

LIB_CODEC = Codec

LIB = -l$(LIB_CODEC)

all: $(bin)

$(algorithms) : lib$(LIB_CODEC).a

all_bin_lo = $(bin:%=%.lo)

# General rules

$(bin) : % : %.o lib$(LIB_CODEC).a
	$(CC) $(DEFINES) $(LD_FLAGS) $(LDFLAGS) $(OPTIMIZATION_FLAGS) $(filter %.o, $^) -o $@ $(LIB)
	mv $@ $(PROGRAM_NAME)

%.o : %.cpp
	$(CC) $(DEFINES) $(CPP_FLAGS) $(CPPFLAGS) $(C_FLAGS) $(CFLAGS) $(OPTIMIZATION_FLAGS) -c  $< -o $@

%.lo : %.cpp
	$(CC) $(DEFINES) $(CPP_FLAGS) $(CPPFLAGS) -MM -MT '$(<:.cpp=.o) $@' $< > $@

include $(all_bin_lo)

# Build static library

lib$(LIB_CODEC).a : $(algorithms:%=%.o)
	$(AR) rcs $@ $^

# Clean up the mess

.PHONY: clean

clean: 
	rm -f *.o *.lo *.a $(bin) $(PROGRAM_NAME)
	rm -f *~ \#* 

