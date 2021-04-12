<!-- Referenced links -->
[bugs]:           https://github.com/n-t-roff/heirloom-doctools/issues
[groff-ml]:       https://lists.gnu.org/mailman/listinfo/groff
[homepage]:       http://n-t-roff.github.io/heirloom/doctools.html
[releases]:       https://github.com/n-t-roff/heirloom-doctools/releases
[Release-badge]:  https://img.shields.io/github/v/release/n-t-roff/heirloom-doctools?label=latest%20release&color=brightGreen&sort=semver
[Release-link]:   https://github.com/n-t-roff/heirloom-doctools/releases/latest
[Snapshot-badge]: https://img.shields.io/badge/download-ZIP%20archive-green
[Snapshot-link]:  https://github.com/n-t-roff/heirloom-doctools/archive/refs/heads/master.zip
[HEAD-badge]:     https://img.shields.io/github/last-commit/n-t-roff/heirloom-doctools?color=blue
[HEAD-link]:      https://github.com/n-t-roff/heirloom-doctools/tree/HEAD

# Heirloom Doctools

[![Latest release][Release-badge]][Release-link]
[![Download ZIP][Snapshot-badge]][Snapshot-link]
[![Last commit][HEAD-badge]][HEAD-link]

For information about the Heirloom doctools please visit the project [homepage][].

Stable source-code versions are available from the [releases page][releases].

Development snapshots can be downloaded as a [ZIP archive][Snapshot-link] or with the command

<!-- XXX: HTML is used so readers can right-click the
hyperlink in their browsers and select "Copy URL" -->
<pre><code class="language-bash">git clone <a href=
"https://github.com/n-t-roff/heirloom-doctools.git"
>https://github.com/n-t-roff/heirloom-doctools.git</a>
</code></pre>

Installation paths and compiler settings are adjusted in `mk.config`.
The software is built with
```bash
./configure
make
```
and installed with
```bash
make install
```
To remove build artefacts and other generated files, run
```bash
make mrproper
```

Bugs can be reported at the [issue list][bugs].
Please wait before creating a patch or a pull-request,
since someone else could already be working on this issue or has a solution.


All &lowast;roff users are encouraged to subscribe to the GNU roff (groff)
[mailing list][groff-ml] for discussions and Heirloom doctools announcements.
Topics related to Heirloom should have a subject beginning with `[Heirloom]`.
However, please *don't* use the mailing-list to submit Heirloom bug-reports.
Instead, use the [issue tracker][bugs] mentioned above.

If there are any problems, please send an e-mail to:
<code>&lt; <ins><b>troff</b></ins> [at] <ins><b>arcor</b></ins> [dot] <ins><b>de</b></ins> &gt;</code>.
