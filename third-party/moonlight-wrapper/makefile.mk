mkfile_path	:=	$(abspath $(lastword $(MAKEFILE_LIST)))
current_dir	:=	$(THIRD_PARTY_PATH)/moonlight-wrapper

SOURCES		:=	$(SOURCES) \
				$(current_dir) \
				$(current_dir)/utils \
				$(current_dir)/switch
			
INCLUDES	:=	$(INCLUDES) \
				$(current_dir) \
				$(current_dir)/utils \
				$(current_dir)/switch

###########  UI  ###########

SOURCES		:=	$(SOURCES) \
				$(current_dir)/ui \
				$(current_dir)/layer

INCLUDES	:=	$(INCLUDES) \
				$(current_dir)/ui \
				$(current_dir)/layer

###########  Crypto  ###########

SOURCES		:=	$(SOURCES) \
				$(current_dir)/crypto

INCLUDES	:=	$(INCLUDES) \
				$(current_dir)/crypto

###########  LibGameStream  ###########

SOURCES		:=	$(SOURCES) \
				$(current_dir)/libgamestream

INCLUDES	:=	$(INCLUDES) \
				$(current_dir)/libgamestream

###########  Streaming  ###########

SOURCES		:=	$(SOURCES) \
				$(current_dir)/streaming \
				$(current_dir)/streaming/audio \
				$(current_dir)/streaming/ffmpeg \
				$(current_dir)/streaming/video 

INCLUDES	:=	$(INCLUDES) \
				$(current_dir)/streaming \
				$(current_dir)/streaming/audio \
				$(current_dir)/streaming/ffmpeg \
				$(current_dir)/streaming/video 
