# Copyright (c) 2021 Alistair Bell <alistair@alistairbell.xyz> 
#
# HolyC compiler is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

include config.mk

SOURCES         = src/main.c src/lexer.c src/util.c src/args.c
OBJECTS         = ${SOURCES:.c=.o}
TARGET          = holyc
INSTALL_DIR     = /usr/bin

# Project Version.
VERSION         = 0.0.16

# Append the version as a macro.
CONFIG_CC_FLAGS := ${CONFIG_CC_FLAGS} -DHOLYC_BUILD_VERSION='"${VERSION}"'

# Sources.
.c.o:
	@echo "cc $@"
	@$(CC) $(CONFIG_CC_FLAGS) -c $< -o $@

# The 'all' rule.
all: prepare libraries ${TARGET}

# Prepares the submodules
prepare:
	ln -sf $(shell pwd)/config.mk src/hashtable/config.mk

libraries: 
	make -C src/hashtable all

# Final linking.
${TARGET}: ${OBJECTS} 
	@echo "cc ${TARGET}"
	@$(CC) ${CONFIG_LD_FLAGS} -L src/hashtable -o $@ ${OBJECTS} -lhashtable ${CONFIG_LD_LIBS}

# Handy rules. 
clean:
	rm src/*.o
	make -C src/hashtable clean
	rm ${TARGET}

install: all
	mkdir -p ${INSTALL_DIR}/
	install -s -m755 ${TARGET} ${INSTALL_DIR}

uninstall:
	rm ${INSTALL_DIR}/${TARGET}

version:
	@echo ${VERSION}


.PHONY: clean install uninstall version
