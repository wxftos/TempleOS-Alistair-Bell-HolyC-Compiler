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
