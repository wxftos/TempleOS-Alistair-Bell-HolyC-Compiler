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

SOURCES       = src/main.c src/args.c src/parser.c src/util.c src/lexer.c src/expr.c
OBJECTS		  = ${SOURCES:.c=.o}
TARGET        = holyc
INSTALL_DIR   = /usr/bin

# Project Version
VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_PATCH = 13
VERSION       = ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}

# Append the version as a macro
CONFIG_CC_FLAGS     := ${CONFIG_CC_FLAGS} -DHOLYC_BUILD_VERSION='"${VERSION}"'

all: ${TARGET}

# source objs
src/%.o: src/%.c
	@echo "cc      $<"
	@$(CC) ${CONFIG_CC_FLAGS} -c $< -o $@ 

# final linking & compiling
${TARGET}: ${OBJECTS}
	@echo "cc      $@"
	@$(CC) -o $@ $^

clean:
	@echo "rm      ${OBJECTS} ${TARGET}"
	@rm ${OBJECTS} ${TARGET}
	
install:
	@echo "cp      ${TARGET} -> ${INSTALL_DIR}"
	@cp ${TARGET} ${INSTALL_DIR}/

version:
	@echo "${VERSION}"

uninstall:
	@rm ${INSTALL_DIR}/${TARGET}
	@echo "rm      ${INSTALL_DIR}/${TARGET}"

.PHONY: clean install uninstall version
