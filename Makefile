CONFIG_CC_FLAGS = -Wextra -Wall -g -Wno-unused-parameter
HOLYC_SOURCES = src/main.c src/parser.c src/util.c
HOLYC_OBJECTS = ${HOLYC_SOURCES:.c=.o}
HOLYC_TARGET  = holyc-compiler
HOLYC_VERSION = 0.0.8

INSTALL_DIR   = /usr/bin


all: ${HOLYC_TARGET}

# source objs
src/%.o: src/%.c
	@echo "cc      $<"
	@$(CC) ${CONFIG_CC_FLAGS} -c $< -o $@ 

# final linking & compiling
${HOLYC_TARGET}: ${HOLYC_OBJECTS}
	@echo "cc      $@"
	@$(CC) -o $@ $^

clean:
	@echo "rm      ${HOLYC_OBJECTS} ${HOLYC_TARGET}"
	@rm ${HOLYC_OBJECTS} ${HOLYC_TARGET}
	
install:
	@echo "cp      ${HOLYC_TARGET} -> ${INSTALL_DIR}"
	@cp ${HOLYC_TARGET} ${INSTALL_DIR}/

version:
	@echo "${HOLYC_VERSION}"

uninstall:
	@rm ${INSTALL_DIR}/${HOLYC_TARGET}
	@echo "rm      ${INSTALL_DIR}/${HOLYC_TARGET}"

.PHONY: clean install uninstall version
