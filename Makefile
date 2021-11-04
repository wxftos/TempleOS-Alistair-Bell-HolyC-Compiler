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

-include config.mk

SOURCES         = src/main.c src/lexer.c src/util.c src/args.c
OBJECTS         = ${SOURCES:.c=.o}
TARGET          = holyc
INSTALL_DIR     = /usr/bin
VERSION         = 0.0.16

# Sources.
.c.o:
	@echo "cc $@"
	@$(CC) ${CFLAGS} -DHOLYC_BUILD_VERSION='"${VERSION}"' -c $< -o $@

all: libraries ${TARGET}

libraries: 
	ln -sf ${PWD}/config.mk src/hashtable/config.mk
	make -C src/hashtable all

${TARGET}: ${OBJECTS} 
	@echo "cc ${TARGET}"
	@$(CC) ${LDFLAGS} -L src/hashtable -o $@ ${OBJECTS} -lhashtable ${LDLIBS}

clean:
	rm src/*.o
	make -C src/hashtable clean
	rm ${TARGET}

install: all
	mkdir -p ${INSTALL_DIR}/
	install -s -m755 ${TARGET} ${INSTALL_DIR}

uninstall:
	$(RM) ${INSTALL_DIR}/${TARGET}

version:
	@echo ${VERSION}

info:
	@echo "CC      -> $(CC)"
	@echo "AR      -> $(AR)"
	@echo "CFLAGS  -> ${CFLAGS}"
	@echo "LDFLAGS -> ${LDFLAGS}"
	@echo "LDLIBS  -> ${LDLIBS}"

.PHONY: clean install uninstall version build_info
