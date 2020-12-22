mkfile_path	:=	$(abspath $(lastword $(MAKEFILE_LIST)))
current_dir	:=	$(THIRD_PARTY_PATH)/borealis

LIBS		:=	-lglfw3 -lEGL -lglapi -ldrm_nouveau -lm $(LIBS)

# fmt wrapper is there because dkp's base
# makefile doesn't recognize .cc files as c++

SOURCES		:=	$(SOURCES) \
				$(current_dir)/lib \
				$(current_dir)/lib/extern/glad \
				$(current_dir)/lib/extern/nanovg \
				$(current_dir)/lib/extern/libretro-common/compat \
				$(current_dir)/lib/extern/libretro-common/encodings \
				$(current_dir)/lib/extern/libretro-common/features \
				$(current_dir)/lib/extern/nxfmtwrapper

INCLUDES	:=	$(INCLUDES) \
				$(current_dir)/include \
				$(current_dir)/lib/extern/fmt/include \
				$(current_dir)/include/borealis/extern
