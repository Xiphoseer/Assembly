# Assembly
This library is intended as a c++ implementation of common functions
and datastructures for recreating a server software for the game
LEGO Universe. It aims to provide these in such a way that it can
be used in a variety of different contexts and within projects
with different design decisions already in place. The individual
structures are largely independant of each other so that any software
may decide to only use a subset of the provided options.

## Operating Systems
While this library is written for and has been developed on a Linux
distribution, it is inteded to be platform independant. I'm open to
contributions that allow this library to be used on other systems,
provided that it does not break existing setups (too much).

## Building & Installation
Currently the repository is optimized to be built by the GNU autotools.
I'm happy to include other mechanisms such as CMake if someone is
willing to set that up.

For building with the autotools, I recommend the following procedure
```
# autoreconf --install
# mkdir build
# cd build
# ../configure
# make
# sudo make install
```

## License

Currently, I have not decided on a license for this project,
though as this is available publicly on github, I plan on choosing
a fairly standard open source one. This means that for the meantime
I tolerate other people using this software, but I still own the
IP rights to all the code I wrote.

## Notes

This codebase may contain some code snippets from stackoverflow
or similar sites. I try to mark all occurences appropriately within
the codebase. If you feel like I missed the attribution somewhere,
feel free to contact me and I will sort that out.

In case you found this, having never heard of any LU restoration
projects before, but are curious as to what has happend in the past years,
check out [Darkflame Universe](https://darkflameuniverse.org) or
[lcdr Universe](https://lcdruniverse.org).
