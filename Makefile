BUILD_DIR = ./satoriya-shiori/satoriya/satori

all:
	cd $(BUILD_DIR) && $(MAKE) -f makefile.emscripten
	cp $(BUILD_DIR)/libsatori.js .

clean:
	cd $(BUILD_DIR) && $(MAKE) -f makefile.emscripten clean
