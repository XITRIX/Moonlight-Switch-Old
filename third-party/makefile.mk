######## 	Includes 	########
include $(current_dir)/borealis/borealis.mk

mkfile_path	:=	$(abspath $(lastword $(MAKEFILE_LIST)))
current_dir	:=	$(THIRD_PARTY_PATH)

######## 	Moonlight 	########
SOURCES		:=	$(SOURCES) \
				$(current_dir)/moonlight-common-c/enet \
				$(current_dir)/moonlight-common-c/reedsolomon \
				$(current_dir)/moonlight-common-c/src

INCLUDES	:=	$(INCLUDES) \
				$(current_dir)/moonlight-common-c/enet/include \
				$(current_dir)/moonlight-common-c/reedsolomon \
				$(current_dir)/moonlight-common-c/src 