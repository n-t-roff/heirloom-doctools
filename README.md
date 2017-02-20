This project is a test bed for improved paragraph composition and letter
adjustment ("microtypograpy") methods on n-t-roff.  These adjustments make
it easier to work with difficult source material and allow modification
of paragraph appearance.  Although the core functions have been tested
extensively, the code should be considered experimental.

Documentation is located in the doc/typo/PDF directory:

. typodoc.pdf - Describes the new requests, their functions, and gives usage examples.

. usermanual.pdf - The current nroff/troff Users's Manual reformatted using the new functions.

. demopage.pdf - Gunnar Ritter's demonstration page warmed over with libre fonts.

The code should be compiled the same way as n-t-roff, although I suggest
installing it to a separate test directory.

Please report any bugs under the "Issues" tab above.

--------

For information about the Heirloom doctools please visit the project
[webpage](http://n-t-roff.github.io/heirloom/doctools.html).

Installation paths and compiler settings are adjusted in `mk.config`.
The software is build with `make` and installed with `make install`.
Generated files are removed with `make mrproper` afterwards.
A git repository can be kept up-to-date with
`git pull`.

