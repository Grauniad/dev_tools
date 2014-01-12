$(HOME)/.dev_tools/.vimrc: vimrcTemplate ../../../makefile.include $(PROJECT_ROOT_DIR)/.gdb_breakpoints
	mkdir -p $(HOME)/.dev_tools/
	cat vimrcTemplate | sed -e "s/__PROJECT_DIR__/$$(echo $(PROJECT_ROOT_DIR) | sed -e 's/\//\\\//g')/g" \
	                        -e "s/__DEV_TOOLS__/$$(echo $(DEV_TOOLS_DIR) | sed -e 's/\//\\\//g')/g" > $(HOME)/.dev_tools/.vimrc
$(PROJECT_ROOT_DIR)/.gdb_breakpoints:
	touch $(PROJECT_ROOT_DIR)/.gdb_breakpoints

MODE=CPP

include ../../../makefile.include
