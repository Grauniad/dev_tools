
../../../makefile.auto.include: 
	cd ../../../; \
	cat DEV_TOOLS/Config/Makefiles/makefileTemplate | sed -e "s/__PROJECT_DIR__/$$(echo $$(pwd) | sed -e 's/\//\\\//g')/g" > makefile.include

