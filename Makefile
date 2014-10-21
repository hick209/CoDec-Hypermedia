CC = g++
C_FLAGS =
CPP_FLAGS = -I. -Wall --pedantic-errors 
LD_FLAGS = -L.
MAKE = make
AR = ar
LIB = 

PROGRAM_NAME = codecFuckingFodaCodec

DEFINES = -DPROGRAM_NAME=\"$(PROGRAM_NAME)\"

bin = main

all: $(bin)


all_bin_d = $(bin:%=%.lo)

# General rules

$(bin) : % : %.o
	$(CC) $(DEFINES) $(LD_FLAGS) $(LDFLAGS) $(filter %.o, $^) -o $@ $(LIB)
	mv $@ $(PROGRAM_NAME)

%.o : %.cpp
	$(CC) $(DEFINES) $(CPP_FLAGS) $(CPPFLAGS) $(C_FLAGS) $(CFLAGS) -c  $< -o $@

%.lo : %.cpp
	$(CC) $(DEFINES) $(CPP_FLAGS) $(CPPFLAGS) -MM -MT '$(<:.cpp=.o) $@' $< > $@

include $(all_bin_d) $(all_aux_bin_d) $(all_static_lib_d) $(all_lib_shared_d)

# Build static library

#$(LIB): lib%.a : %.o
#	$(AR) rcs $@ $^

# Clean up the mess

.PHONY: clean

clean: 
	rm -f *.o *.lo *.a $(bin) $(PROGRAM_NAME)
	rm -f *~ \#* 

