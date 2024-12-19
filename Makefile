.PHONY: debug release all clean init run run_release

BUILD_DIR = build
DEBUG_DIR = $(BUILD_DIR)/debug
RELEASE_DIR = $(BUILD_DIR)/release
TARGETS = aoc

debug:
	@echo "DEBUG: $(DEBUG)"
	@mkdir -p "$(DEBUG_DIR)"
	@cd "$(DEBUG_DIR)" && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=debug ../..
	@ln -sf "$(DEBUG_DIR)/compile_commands.json" .
	@cd "$(DEBUG_DIR)" && make $(TARGETS)

run: debug
	./$(DEBUG_DIR)/aoc

run_release: release
	./$(RELEASE_DIR)/aoc

all: debug release

init:
	@git submodule update --init --recursive --force

release:
	@echo "RELEASE: $(RELEASE)"
	@mkdir -p "$(RELEASE_DIR)"
	@cd "$(RELEASE_DIR)" && cmake -DCMAKE_BUILD_TYPE=release ../..
	# @cd "$(RELEASE_DIR)" && cmake -DCMAKE_BUILD_TYPE=release ../..
	@cd "$(RELEASE_DIR)" && make $(TARGETS)

clean:
	rm -rf "$(BUILD_DIR)"

