mkfile_path	:=	$(abspath $(lastword $(MAKEFILE_LIST)))
current_dir	:=	./src

######## 	SRC 	########
SOURCES		:=	$(SOURCES) \
				$(current_dir)/ui

INCLUDES	:=	$(INCLUDES) \
				$(current_dir)/ui