The Heirloom Documentation Tools
================================

The Heirloom Documentation Tools package provides troff, nroff, and related
utilities to format manual pages and other documents for output on terminals
and printers.
They are portable and enhanced versions of the utilities released by Sun as
part of OpenSolaris, which are a variant of ditroff, which, in turn, descends
to the historical Unix troff that generated output for the C/A/T
phototypesetter.

nroff is most prominently used to format Unix manual pages for viewing them on
a terminal.
This implementation consumes relatively few system resources and is thus
suitable for small system distributions.
It has been enhanced to generate UTF-8 output.

troff generates PostScript output from the same document source code as nroff.
Thus a special use of it is to prepare manual pages for printing.
troff is a general-purpose typesetting processor, though.
Significant features that have been added in this implementation are:

* "Plug-and-Play" font handling:
  troff can access PostScript Type 1, OpenType, and TrueType fonts directly,
  that is, it can read font metrics from AFM, OpenType, or TrueType files, and
  can instruct its dpost post-processor to include glyph data from PFB, PFA,
  OpenType, and TrueType files into the output it generates.
* Justification improvements include paragraph-at-once adjustment using a
  variant of the Knuth-Plass algorithm originally developed for TeX, and
  options to decrease word spaces and to dynamically change the letter spacing
  as well as the glyph widths (microtypography).
* Pairwise kerning of characters and letter space tracking are supported.
  troff includes a request to construct kerning pairs for characters from
  different fonts.
* Hanging characters can be used, for example, hyphens can partially or
  entirely hang over the right margin of a document.
* Mechanisms for typesetting small capitals, old-style numerals, and arbitrary
  ligatures e.g. using Type 1 "expert" or OpenType fonts are provided.
* Letters can be of arbitrary size, including fractional point sizes.
  Individual fonts can be zoomed.
* Text input is processed according to the LC\_CTYPE environment variable.
  With a UTF-8 locale, a large set of characters is available without the need
  to type troff escape sequences.
* Hyphenation of international languages is supported using the OpenOffice
  variant of Raph Levien's LibHnj, which is in turn based on Liang's TeX
  algorithm.
* International paper sizes such as A4 can be configured.
* The dpost post-processor can generate instructions for the PDF distiller for
  the inclusion of bookmarks, links, titles, and authorship information.
* Most groff extensions, like long names for requests, strings, and number
  registers, are supported. A special groff compatibility mode is also
  provided.
* Other troff language extensions include a string search request, output line
  traps, floating-point registers, and local variables per macro instance.

The distribution contains a script to convert OpenDocument files to troff
input.
This allows troff to act as typesetting application in a production scheme
where authors hand in office documents.

The text above had been written by Gunnar Ritter who had developed the
heirloom-doctools on base of source code from OpenSolaris and Plan9.
This had been at a time when 64 bit hardware was not very common.
Since Gunnar is not maintaining the project at the moment this fork had been
created.
The main focus of this fork is not further development but rather keeping all
funktionality and fixing bugs.

Bugs can be reported at the
[issue list](https://github.com/n-t-roff/heirloom-doctools/issues).

If there are any problems mail can be send to
[troff at arcor dot de](mailto:troff@arcor.de).
