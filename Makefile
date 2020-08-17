#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCE is the directory containing source code
# INCLUDES is a list of directories containing extra header files
# ASSETS is the directory of assets
#---------------------------------------------------------------------------------
TARGET    := $(shell basename $(CURDIR))
BUILD     := build
SOURCE    := source
DATA      := data
INCLUDES  := include
ASSETS    := res

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH := -mthumb -mthumb-interwork

CFLAGS := -Wall -O3
    -march=armv5te -mtune=arm946e-s -fomit-frame-pointer\
    -ffast-math \
    $(ARCH)

CFLAGS    += $(INCLUDE) -DARM9
# CXXFLAGS  := $(CFLAGS) -fno-rtti -fno-exceptions
CXXFLAGS  := $(CFLAGS) -fno-exceptions

ASFLAGS := -g $(ARCH)
LDFLAGS  = -specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project (order is important)
#---------------------------------------------------------------------------------
LIBS := -lmm9 -lnds9

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS := $(LIBNDS)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT := $(CURDIR)/$(TARGET)

export DEPSDIR := $(CURDIR)/$(BUILD)

export CFILES        := $(shell find $(SOURCE) -name *.c)
export CPPFILES      := $(shell find $(SOURCE) -name *.cpp)
export SFILES        := $(shell find $(SOURCE) -name *.s)
export BINFILES      := soundbank.bin
export SPRITE_FILES  := $(shell find $(ASSETS) -name *.png)
export AUDIO_FILES   := $(shell find $(ASSETS) -name *.wav)

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
  export LD := $(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
  export LD := $(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES := \
	$(SPRITE_FILES:.png=.o) \
  $(addsuffix .o,$(BINFILES)) \
  $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE := \
  $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
  $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
  -I$(CURDIR)/$(BUILD)/$(ASSETS)\
  -I$(CURDIR)/$(BUILD)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds

#---------------------------------------------------------------------------------
else

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

$(OUTPUT).nds : $(OUTPUT).elf
$(OUTPUT).elf : $(OFILES)

#---------------------------------------------------------------------------------
$(CPPFILES:.cpp=.o) : $(foreach file,$(CPPFILES),../$(file))
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c ../$(basename $@).cpp -o $@
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
$(SPRITE_FILES:.png=.o) : $(foreach file,$(SPRITE_FILES),../$(file))
	@mkdir -p $(dir $@)
	grit ../$(basename $@).png -ff../$(basename $@).grit -o$(basename $@)
	$(CXX) $(CFLAGS) -c $(basename $@).s -o $(basename $@).o
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
soundbank.bin soundbank.h : $(foreach file,$(AUDIO_FILES),../$(file))
	@mmutil $^ -d -osoundbank.bin -hsoundbank.h
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
%.bin.o %_bin.h : %.bin
	@echo $(notdir $<)
	@$(bin2o)
#---------------------------------------------------------------------------------

-include $(DEPSDIR)/*.d

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
