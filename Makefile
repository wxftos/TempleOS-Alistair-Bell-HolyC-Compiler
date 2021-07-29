CONFIG_CC_FLAGS = -Wextra -Wall -g -Werror -Wno-unused-parameter
HOLYC_SOURCES = src/main.c
HOLYC_OBJECTS = ${HOLYC_SOURCES:.c=.o}
HOLYC_TARGET  = holyc-compiler # like cc but for holyc
HOLYC_VERSION = 0.0.0

INSTALL_DIR   = /usr/bin


all: ${HOLYC_TARGET}

# source objs
src/%.o: src/%.c
	@echo "cc      $<"
	@$(CC) ${CONFIG_CC_FLAGS} -c $< -o $@ 

${HOLYC_TARGET}: ${HOLYC_OBJECTS}
	@echo "cc      $@"
	@$(CC) -o $@ $^

clean:
	@echo "rm      ${HOLYC_OBJECTS} ${HOLYC_TARGET}"
	@rm ${HOLYC_OBJECTS} ${HOLYC_TARGET}
	
install:
	@echo "cp      ${HOLYC_TARGET} -> ${INSTALL_DIR}"
	@cp ${HOLYC_TARGET} ${INSTALL_DIR}/
	@echo "finished installing ${HOLYC_TARGET}"

version:
	@echo "${HOLYC_VERSION}"

uninstall:
	@rm ${INSTALL_DIR}/${HOLYC_TARGET}
	@echo "rm      ${INSTALL_DIR}/${HOLYC_TARGET}"

.PHONY: clean install uninstall version
