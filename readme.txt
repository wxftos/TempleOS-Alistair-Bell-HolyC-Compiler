HolyC compiler
=============
Allows holyc to be compiled into native binaries on unix like systems.
My development excludes non free operating systems.

Building
========
GNU/Linux
`make && sudo/doas make install`

OpenBSD
`gmake && doas gmake install`

Note: 
    All code is tested on a series of GNU/Linux or BSD distributions before commit's.
    This includes:
        Ubuntu 20.04.2-lts (amd64),
        Hyperbola 0.3.1 milkyway (amd64),
        OpenBSD 6.8 (amd64),
        Arch GNU/Linux (rolling release) (amd64),

License
=======
All code licensed under the GPLv3 and should include a notice at the header of the file.

Copyright
=========
HolyC is not my property and was created by Terry Davis
