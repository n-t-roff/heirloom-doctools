/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2003 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/


/*	from OpenSolaris "n10.c	1.15	05/06/08 SMI"	*/

/*
 * Portions Copyright (c) 2005 Gunnar Ritter, Freiburg i. Br., Germany
 *
 * Sccsid @(#)n10.c	1.33 (gritter) 12/25/06
 */

/*
 * University Copyright- Copyright (c) 1982, 1986, 1988
 * The Regents of the University of California
 * All Rights Reserved
 *
 * University Acknowledgment- Portions of this document are derived from
 * software developed by the University of California, Berkeley, and its
 * contributors.
 */

/*
n10.c

Device interfaces
*/

#include <limits.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#ifdef EUC
#include <wchar.h>
#endif
#include "tdef.h"
#include "ext.h"
#include "tw.h"
#include "pt.h"

struct t t;	/* terminal characteristics */

int	dtab;
int	plotmode;
int	esct;

char	*xchname;		/* hy, em, etc. */
short	*xchtab;		/* indexes into chname[] */
char	*codestr;
char	*chname;
short	*chtab;
int	nchtab = 0;

int	*bdtab;
int	*fontlab;

int	Inch;
int	Hor;
int	Vert;
int	nfonts	= 4;	/* R, I, B, S */

/* these characters are used as various signals or values
 * in miscellaneous places.
 * values are set in specnames in t10.c
 */

int	c_hyphen;
int	c_emdash;
int	c_rule;
int	c_minus;
int	c_fi;
int	c_fl;
int	c_ff;
int	c_ffi;
int	c_ffl;
int	c_acute;
int	c_grave;
int	c_under;
int	c_rooten;
int	c_boxrule;
int	c_lefthand;
int	c_dagger;
int	c_isalnum;

int	utf8;

static char tab_lp[] = "\
lp\n\
bset	0\n\
breset	0\n\
Hor	24\n\
Vert	40\n\
Newline	40\n\
Char	24\n\
Em	24\n\
Halfline	20\n\
Adj	24\n\
twinit	\"\"\n\
twrest	\"\"\n\
twnl	\"\\n\"\n\
hlr	\"\"\n\
hlf	\"\"\n\
flr	\"\\0337\"\n\
bdon	\"\"\n\
bdoff	\"\"\n\
iton	\"\"\n\
itoff	\"\"\n\
ploton	\"\"\n\
plotoff	\"\"\n\
up	\"\"\n\
down	\"\"\n\
right	\"\"\n\
left	\"\"\n\
\n\
charset\n\
em 1 -\n\
hy 1 -\n\
\\- 1 -\n\
rs 1 %\\134\n\
dq 1 %\\042\n\
bu 1 +\\bo\n\
sq 2 \\[]\n\
ru 1 _\n\
14 3 1/4\n\
12 3 1/2\n\
34 3 3/4\n\
fi 2 fi\n\
fl 2 fl\n\
ff 2 ff\n\
Fi 3 ffi\n\
Fl 3 ffl\n\
de 1 \\344o\\304\n\
dg 1 |\\b-\n\
fm 1 '\n\
ct 1 c\\b/\n\
rg 1 r\\bO\n\
co 1 c\\bO\n\
pl 1 +\n\
mi 1 -\n\
eq 1 =\n\
** 1 *\n\
sc 1 j\\bf\n\
aa 1 '\n\
ga 1 `\n\
ul 1 _\n\
sl 1 /\n\
*a 1 <\\ba\n\
*b 1 ,\\bB\n\
*g 1 <\\by\n\
*d 1 <\\bo\n\
*e 1 -\\bc\n\
*z 1 ,\\bL\n\
*y 1 ,\\bn\n\
*h 1 -\\b0\n\
*i 1 ,\\bi\n\
*k 1 k\n\
*l 1 \\\\\\b>\n\
*m 1 ,\\bu\n\
*n 1 ,\\bv\n\
*c 1 ,\\b3\n\
*o 1 o\n\
*p 1 -\\bn\n\
*r 1 p\n\
*s 1 -\\bo\n\
*t 1 ~\\bt\n\
*u 1 u\n\
*f 1 /\\bo\n\
*x 1 x\n\
*q 1 |\\bu\n\
*w 1 u\\bw\n\
*A 1 A\n\
*B 1 B\n\
*G 2 ~\\b|~\n\
*D 2 _\\b/_\\b\\\\\n\
*E 1 E\n\
*Z 1 Z\n\
*Y 1 H\n\
*H 1 -\\bO\n\
*I 1 I\n\
*K 1 K\n\
*L 2 /\\\\\n\
*M 1 M\n\
*N 1 N\n\
*C 1 _\\b-\\b~\n\
*O 1 O\n\
*P 2 ~\\b|~\\b|\n\
*R 1 P\n\
*S 1 ~\\b_\\b>\n\
*T 1 T\n\
*U 1 Y\n\
*F 1 |\\bO\n\
*X 1 X\n\
*Q 1 |\\bU\n\
*W 2 _\\b(_\\b)\n\
ts 1 s\n\
sr 2 \\\\/\n\
rn 1 \\0337_\\n\n\
>= 1 _\\b>\n\
<= 1 _\\b<\n\
== 1 _\\b=\n\
~= 1 ~\\b=\n\
ap 1 ~\n\
!= 1 =\\b/\n\
-> 2 ->\n\
<- 2 <-\n\
ua 1 |\\b^\n\
da 1 |\\bv\n\
mu 1 x\n\
di 1 -\\b:\n\
+- 1 +\\b_\n\
cu 1 U\n\
ca 3 (^)\n\
sb 2 (_\\b~\n\
sp 2 _\\b~)\n\
ib 2 (~\\b_\\b=\n\
ip 2 ~\\b_\\b=)\n\
if 2 oo\n\
pd 1 6\n\
gr 1 ~\\bV\n\
no 1 -\n\
is 1 '\\b,\\bI\n\
pt 2 oc\n\
es 1 /\\bO\n\
mo 1 -\\bC\n\
br 1 |\n\
dd 1 |\\b=\n\
rh 2 =>\n\
lh 2 <=\n\
or 1 |\n\
ci 1 O\n\
lt 1 (\n\
lb 1 (\n\
rt 1 )\n\
rb 1 )\n\
lk 1 |\n\
rk 1 |\n\
bv 1 |\n\
lf 1 |\n\
rf 1 |\n\
lc 1 |\n\
rc 1 |\n";

#ifdef	EUC
static char tab_utf8[] = "\
utf8\n\
bset	0\n\
breset	0\n\
Hor	24\n\
Vert	40\n\
Newline	40\n\
Char	24\n\
Em	24\n\
Halfline	20\n\
Adj	24\n\
twinit	\"\"\n\
twrest	\"\"\n\
twnl	\"\\n\"\n\
hlr	\"\"\n\
hlf	\"\"\n\
flr	\"\\0337\"\n\
bdon	\"\"\n\
bdoff	\"\"\n\
iton	\"\"\n\
itoff	\"\"\n\
ploton	\"\"\n\
plotoff	\"\"\n\
up	\"\"\n\
down	\"\"\n\
right	\"\"\n\
left	\"\"\n\
\n\
charset\n\
em 1 %\\342%\\200%\\224\n\
en 1 %\\342%\\200%\\223\n\
hy 1 %\\342%\\200%\\220\n\
\\- 1 -\n\
rs 1 %\\134\n\
dq 1 %\\042\n\
bu 1 %\\342%\\200%\\242\n\
sq 1 %\\342%\\226%\\241\n\
ru 1 %\\342%\\216%\\275\n\
14 1 %\\302%\\274\n\
12 1 %\\302%\\275\n\
34 1 %\\302%\\276\n\
fi 2 fi\n\
fl 2 fl\n\
ff 2 ff\n\
Fi 3 ffi\n\
Fl 3 ffl\n\
de 1 %\\302%\\260\n\
dg 1 %\\342%\\200%\\240\n\
fm 1 '\n\
ct 1 %\\302%\\242\n\
rg 1 %\\302%\\256\n\
co 1 %\\302%\\251\n\
pl 1 +\n\
mi 1 %\\342%\\210%\\222\n\
eq 1 =\n\
** 1 *\n\
sc 1 %\\302%\\247\n\
aa 1 '\n\
ga 1 `\n\
ul 1 %\\342%\\216%\\275\n\
sl 1 /\n\
*a 1 %\\316%\\261\n\
*b 1 %\\316%\\262\n\
*g 1 %\\316%\\263\n\
*d 1 %\\316%\\264\n\
*e 1 %\\316%\\265\n\
*z 1 %\\316%\\266\n\
*y 1 %\\316%\\267\n\
*h 1 %\\316%\\270\n\
*i 1 %\\316%\\271\n\
*k 1 %\\316%\\272\n\
*l 1 %\\316%\\273\n\
*m 1 %\\316%\\274\n\
*n 1 %\\316%\\275\n\
*c 1 %\\316%\\276\n\
*o 1 %\\316%\\277\n\
*p 1 %\\317%\\200\n\
*r 1 %\\317%\\201\n\
*s 1 %\\317%\\203\n\
*t 1 %\\317%\\204\n\
*u 1 %\\317%\\205\n\
*f 1 %\\317%\\206\n\
*x 1 %\\317%\\207\n\
*q 1 %\\317%\\210\n\
*w 1 %\\317%\\211\n\
*A 1 %\\316%\\221\n\
*B 1 %\\316%\\222\n\
*G 1 %\\316%\\223\n\
*D 1 %\\316%\\224\n\
*E 1 %\\316%\\225\n\
*Z 1 %\\316%\\226\n\
*Y 1 %\\316%\\227\n\
*H 1 %\\316%\\230\n\
*I 1 %\\316%\\231\n\
*K 1 %\\316%\\232\n\
*L 1 %\\316%\\233\n\
*M 1 %\\316%\\234\n\
*N 1 %\\316%\\235\n\
*C 1 %\\316%\\236\n\
*O 1 %\\316%\\237\n\
*P 1 %\\316%\\240\n\
*R 1 %\\316%\\241\n\
*S 1 %\\316%\\243\n\
*T 1 %\\316%\\244\n\
*U 1 %\\316%\\245\n\
*F 1 %\\316%\\246\n\
*X 1 %\\316%\\247\n\
*Q 1 %\\316%\\250\n\
*W 1 %\\316%\\251\n\
ts 1 %\\317%\\202\n\
sr 1 %\\342%\\210%\\232\n\
rn 1 %\\342%\\216%\\272\n\
>= 1 %\\342%\\211%\\245\n\
<= 1 %\\342%\\211%\\244\n\
== 1 %\\342%\\211%\\241\n\
~= 1 %\\342%\\211%\\203\n\
~~ 1 %\\342%\\211%\\210\n\
ap 1 %\\342%\\210%\\274\n\
!= 1 %\\342%\\211%\\223\n\
-> 1 %\\342%\\206%\\222\n\
<- 1 %\\342%\\206%\\220\n\
ua 1 %\\342%\\206%\\221\n\
da 1 %\\342%\\206%\\223\n\
mu 1 %\\303%\\227\n\
di 1 %\\303%\\267\n\
+- 1 %\\302%\\261\n\
cu 1 %\\342%\\210%\\252\n\
ca 1 %\\342%\\210%\\251\n\
sb 1 %\\342%\\212%\\202\n\
sp 1 %\\342%\\212%\\201\n\
ib 1 %\\342%\\212%\\207\n\
ip 1 %\\342%\\212%\\206\n\
if 1 %\\342%\\210%\\236\n\
pd 1 %\\342%\\210%\\202\n\
gr 1 %\\342%\\210%\\207\n\
no 1 %\\302%\\254\n\
is 1 %\\342%\\210%\\253\n\
pt 1 %\\342%\\210%\\235\n\
es 1 %\\342%\\210%\\205\n\
mo 1 %\\342%\\210%\\210\n\
br 1 %\\342%\\224%\\202\n\
dd 1 %\\342%\\200%\\241\n\
rh 1 %\\342%\\230%\\236\n\
lh 1 %\\342%\\230%\\234\n\
or 1 |\n\
ci 1 %\\342%\\227%\\213\n\
bx 1 %\\342%\\226%\\240\n\
Sl 1 %\\342%\\204%\\223\n\
fa 1 %\\342%\\210%\\200\n\
te 1 %\\342%\\210%\\203\n\
al 1 %\\342%\\204%\\265\n\
Ox 1 %\\342%\\212%\\227\n\
O+ 1 %\\342%\\212%\\225\n\
tm 1 %\\342%\\204%\\242\n\
`` 1 %\\342%\\200%\\234\n\
'' 1 %\\342%\\200%\\235\n\
-+ 1 %\\342%\\210%\\223\n\
lt 1 %\\342%\\216%\\247\n\
lb 1 %\\342%\\216%\\251\n\
rt 1 %\\342%\\216%\\253\n\
rb 1 %\\342%\\216%\\255\n\
lk 1 %\\342%\\216%\\250\n\
rk 1 %\\342%\\216%\\254\n\
bv 1 %\\342%\\216%\\252\n\
lf 1 %\\342%\\216%\\212\n\
rf 1 %\\342%\\216%\\213\n\
lc 1 %\\342%\\216%\\210\n\
rc 1 %\\342%\\216%\\211\n";
#endif	/* EUC */

void
ptinit(void)
{
	register int i;
	register char *p, *cp;
	char *tt;
	int nread, fd;
	struct stat stbuf;
	char check[50];
	extern int initbdtab[], initfontlab[];

	t.codetab = calloc(NROFFCHARS-_SPECCHAR_ST, sizeof *t.codetab);
	t.width = calloc(NROFFCHARS, sizeof *t.width);
	xchname = calloc(4 * (NROFFCHARS-_SPECCHAR_ST), sizeof *xchname);
	xchtab = calloc(NROFFCHARS-_SPECCHAR_ST, sizeof *xchtab);
	chname = xchname;
	chtab = xchtab;
	bdtab = initbdtab;
	fontlab = initfontlab;
	tt = malloc(strlen(termtab) + strlen(devname) + 1);
	strcpy(tt, termtab);
	strcat(tt, devname);
	termtab = tt;
	if (strcmp(devname, "locale") == 0) {
#ifdef	EUC
		wchar_t	wc;
		if (mb_cur_max > 1 &&
				mbtowc(&wc, "\303\266", 2) == 2 &&
					wc == 0xF6 &&
				mbtowc(&wc, "\342\202\254", 3) == 3 &&
					wc == 0x20AC) {
			codestr = tab_utf8;
			nread = sizeof tab_utf8 - 1;
			csi_width[0] = 0;
			utf8 = 1;
		} else
#endif	/* EUC */
		{
			codestr = tab_lp;
			nread = sizeof tab_lp - 1;
		}
	} else if ((fd = open(termtab, O_RDONLY)) < 0) {
		if (strcmp(devname, "lp")) {
			errprint("cannot open %s", termtab);
			exit(-1);
		}
		codestr = tab_lp;
		nread = sizeof tab_lp - 1;
	} else {
		fstat(fd, &stbuf);
		codestr = setbrk((int) stbuf.st_size);

		nread = read(fd, codestr, (int) stbuf.st_size);
		close(fd);
	}

	p = codestr;
	p = skipstr(p);		/* skip over type, could check */
	p = skipstr(p); p = getint(p, &t.bset);
	p = skipstr(p); p = getint(p, &t.breset);
	p = skipstr(p); p = getint(p, &t.Hor);
	p = skipstr(p); p = getint(p, &t.Vert);
	p = skipstr(p); p = getint(p, &t.Newline);
	p = skipstr(p); p = getint(p, &t.Char);
	p = skipstr(p); p = getint(p, &t.Em);
	p = skipstr(p); p = getint(p, &t.Halfline);
	p = skipstr(p); p = getint(p, &t.Adj);
	p = skipstr(p); p = getstr(p, t.twinit = p);
	p = skipstr(p); p = getstr(p, t.twrest = p);
	p = skipstr(p); p = getstr(p, t.twnl = p);
	p = skipstr(p); p = getstr(p, t.hlr = p);
	p = skipstr(p); p = getstr(p, t.hlf = p);
	p = skipstr(p); p = getstr(p, t.flr = p);
	p = skipstr(p); p = getstr(p, t.bdon = p);
	p = skipstr(p); p = getstr(p, t.bdoff = p);
	p = skipstr(p); p = getstr(p, t.iton = p);
	p = skipstr(p); p = getstr(p, t.itoff = p);
	p = skipstr(p); p = getstr(p, t.ploton = p);
	p = skipstr(p); p = getstr(p, t.plotoff = p);
	p = skipstr(p); p = getstr(p, t.up = p);
	p = skipstr(p); p = getstr(p, t.down = p);
	p = skipstr(p); p = getstr(p, t.right = p);
	p = skipstr(p); p = getstr(p, t.left = p);

	getstr(p, check);
	if (strcmp(check, "charset") != 0) {
		errprint("device table apparently curdled");
		exit(1);
	}

	for (i = 0; i < _SPECCHAR_ST; i++)
		t.width[i] = 1;	/* default widths */

	i = 0;
/* this ought to be a pointer array and in place in codestr */
	cp = chname + 1;	/* bug if starts at 0, in setch */
	while (p < codestr + nread) {
		while (*p == ' ' || *p == '\t' || *p == '\n')
			p++;
		if (i + _SPECCHAR_ST >= NROFFCHARS) {
			errprint("too many names in charset for %s", termtab);
			exit(1);
		}
		chtab[i] = cp - chname;	/* index, not pointer */
		*cp++ = *p++;	/* 2-char names */
		*cp++ = *p++;
		*cp++ = '\0';
		while (*p == ' ' || *p == '\t')
			p++;
		t.width[i+_SPECCHAR_ST] = *p++ - '0';
		while (*p == ' ' || *p == '\t')
			p++;
		t.codetab[i] = p;
		p = getstr(p, p);	/* compress string */
		p++;
		i++;
		nchtab++;
	}

	sps = EM;
	ses = EM;
	ics = EM * 2;
	dtab = 8 * t.Em;
	for (i = 0; i < 16; i++)
		tabtab[i] = dtab * (i + 1);
	pl = 11 * INCH;
	po = PO;
	spacesz = SS;
	sesspsz = SSS;
	lss = lss1 = VS;
	ll = ll1 = lt = lt1 = LL;
	smnt = nfonts = 5;	/* R I B BI S */
	specnames();	/* install names like "hyphen", etc. */
	if (eqflg)
		t.Adj = t.Hor;
}

char *
skipstr (	/* skip over leading space plus string */
    char *s
)
{
	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;
	while (*s != ' ' && *s != '\t' && *s != '\n')
		if (*s++ == '\\')
			s++;
	return s;
}

char *
getstr (	/* find next string in s, copy to t */
    char *s,
    char *t
)
{
	int quote = 0;

	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;
	if (*s == '"') {
		s++;
		quote = 1;
	}
	for (;;) {
		if (quote && *s == '"') {
			s++;
			break;
		}
		if (!quote && (*s == ' ' || *s == '\t' || *s == '\n'))
			break;
		if (*s != '\\')
			*t++ = *s++;
		else {
			s++;	/* skip \\ */
			if (isdigit((unsigned char)s[0]) &&
			    isdigit((unsigned char)s[1]) &&
			    isdigit((unsigned char)s[2])) {
				*t++ = (s[0]-'0')<<6 | (s[1]-'0')<<3 | s[2]-'0';
				s += 2;
			} else if (isdigit((unsigned char)s[0])) {
				*t++ = *s - '0';
			} else if (*s == 'b') {
				*t++ = '\b';
			} else if (*s == 'n') {
				*t++ = '\n';
			} else if (*s == 'r') {
				*t++ = '\r';
			} else if (*s == 't') {
				*t++ = '\t';
			} else {
				*t++ = *s;
			}
			s++;
		}
	}
	*t = '\0';
	return s;
}

char *
getint (	/* find integer at s */
    char *s,
    int *pn
)
{
	int base;

	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;
	base = (*s == '0') ? 8 : 10;
	*pn = 0;
	while (isdigit((unsigned char)*s))
		*pn = base * *pn + *s++ - '0';
	return s;
}

void
specnames(void)
{
	static struct {
		int	*n;
		char	*v;
	} spnames[] = {
		{ &c_hyphen, "hy"	},
		{ &c_emdash, "em"	},
		{ &c_rule, "ru"		},
		{ &c_minus, "\\-"	},
		{ &c_fi, "fi"		},
		{ &c_fl, "fl"		},
		{ &c_ff, "ff"		},
		{ &c_ffi, "Fi"		},
		{ &c_ffl, "Fl"		},
		{ &c_acute, "aa"	},
		{ &c_grave, "ga"	},
		{ &c_under, "ul"	},
		{ &c_rooten, "rn"	},
		{ &c_boxrule, "br"	},
		{ &c_lefthand, "lh"	},
		{ &c_isalnum, "__"	},
		{ 0, 0 }
	};
	int	i;

	for (i = 0; spnames[i].n; i++)
		*spnames[i].n = findch(spnames[i].v);
	if (c_isalnum == 0)
		c_isalnum = NROFFCHARS;
}


int 
findch (	/* find char s in chname */
    register char *s
)
{
	register int	i;

	for (i = 0; chtab[i] != 0; i++)
		if (strcmp(s, &chname[chtab[i]]) == 0)
			return(i + _SPECCHAR_ST);
	return(0);
}

void
twdone(void)
{
	int waitf;

	obufp = obuf;
	if (t.twrest)		/* has ptinit() been done yet? */
		oputs(t.twrest);
	flusho();
	if (pipeflg != -1) {
		close(ptid);
		waitpid(pipeflg, &waitf, 0);
	}
	restore_tty();
}


void
ptout(tchar i)
{
	if (cbits(i) == FILLER)
		return;
	if (isadjspc(i))
		return;
	if (olinep >= &oline[olinesz]) {
		tchar	*k;
		olinesz += 100;
		k = realloc(oline, olinesz * sizeof *oline);
		olinep = (tchar *)((char *)olinep + ((char *)k-(char *)oline));
		oline = k;
	}
	*olinep++ = i;
	if (cbits(i) != '\n')
		return;
	olinep--;
	lead += dip->blss + lss - t.Newline;
	dip->blss = 0;
	esct = esc = 0;
	if (olinep > oline) {
		move();
		ptout1();
		oputs(t.twnl);
	} else {
		lead += t.Newline;
		move();
	}
	lead += dip->alss;
	dip->alss = 0;
	olinep = oline;
}


void
ptout1(void)
{
	register int k;
	register char	*codep;
	char	*savep;
#ifdef EUC
	register char *qq;
#endif /* EUC */
	int	w, j, phyw;
#ifdef EUC
	int jj;
#endif /* EUC */
	tchar * q, i;
	static int oxfont = FT;	/* start off in roman */

	for (q = oline; q < olinep; q++) {
		i = *q;
		if (ismot(i)) {
			j = absmot(i);
			if (isnmot(i))
				j = -j;
			if (isvmot(i))
				lead += j;
			else 
				esc += j;
			continue;
		}
		if ((k = cbits(i)) <= 040) {
			switch (k) {
			case ' ': /*space*/
				esc += t.Char;
				break;
			case '\033':
			case '\007':
			case '\016':
			case '\017':
				oput(k);
				break;
			}
			continue;
		}
#ifdef EUC
		if (multi_locale && (k >= nchtab + _SPECCHAR_ST)) {
			jj = tr2un(k, fbits(i));
			if ((jj = wcwidth(jj)) < 0)
				jj = 0;
			phyw = w = t.Char * csi_width[jj];
		} else {
#endif /* EUC */
		phyw = w = t.Char * t.width[k];
		if (iszbit(i))
			w = 0;
#ifdef EUC
		}
#endif /* EUC */
		if (esc || lead)
			move();
		esct += w;
		xfont = fbits(i);
		if (xfont != oxfont) {
			if (oxfont == ulfont || oxfont == BIFONT)
				oputs(t.itoff);
			if (bdtab[oxfont])
				oputs(t.bdoff);
			if (xfont == ulfont || xfont == BIFONT)
				oputs(t.iton);
			if (bdtab[xfont])
				oputs(t.bdon);
			oxfont = xfont;
		}
		if ((xfont == ulfont || xfont == BIFONT) && !(*t.iton & 0377)) {
			for (j = w / t.Char; j > 0; j--)
				oput('_');
			for (j = w / t.Char; j > 0; j--)
				oput('\b');
		}
		if ((j = bdtab[xfont]) && !(*t.bdon & 0377))
			j++;
		else
			j = 1;	/* number of overstrikes for bold */
#ifdef	EUC
		if (k == '-' && utf8) {
			/*
			 * With -Tlocale and a UTF-8 locale, "-" is replaced
			 * by a UTF-8 hyphen, and "\-" remains the ASCII
			 * hyphen-minus character. This is because in manual
			 * pages, "\-" represents the ASCII option
			 * introduction character, and converting it to a
			 * UTF-8 minus character would make it impossible
			 * to copy-and-paste option descriptions.
			 */
			savep = "%\342%\200%\220";
			goto loop;
		} else if (k == '`' && utf8) {
			/*
			 * Similar considerations apply to ` ' vs. \` \'.
			 * The former are typographic single quotes, while
			 * the latter are commonly used for the ASCII syntax
			 * quotes in manual pages.
			 */
			savep = "%\342%\200%\230";
			goto loop;
		} else if (k == '\'' && utf8) {
			savep = "%\342%\200%\231";
			goto loop;
		} else
#endif
			if (k < 128) {	/* ordinary ascii */
			oput(k);
			while (--j > 0) {
				oput('\b');
				oput(k);
			}
#ifdef EUC
		} else if (multi_locale && (k >= nchtab + _SPECCHAR_ST)) {
			int	n;
			char	mb[MB_LEN_MAX+1];
			jj = tr2un(k, fbits(i));
			if ((n = wctomb(mb, jj)) > 0) {
				for (qq = mb; qq < &mb[n];)
					oput(*qq++);
				while (--j > 0) {
					for (jj = w / t.Char; jj > 0; jj--)
						oput('\b');
					for (qq = mb; qq < &mb[n];)
						oput(*qq++);
				}
			}
		} else if (k < 256) {
			/*
			 * Invalid character for C locale or
			 * non-printable 8-bit single byte
			 * character such as <no-break-sp>
			 * in ISO-8859-1
			 */
			oput(k);
			while (--j > 0) {
				oput('\b');
				oput(k);
			}
#endif /* EUC */
		} else if (k >= nchtab + _SPECCHAR_ST) {
			oput(k - nchtab - _SPECCHAR_ST);
		} else {
			int oj, isesc, allesc;
			savep = t.codetab[k-_SPECCHAR_ST];
		loop:	codep = savep;
			allesc = 1;
			oj = j;
			while (*codep != 0) {
				if (*codep & 0200) {
					codep = plot(codep);
					oput(' ');
					allesc = 0;
				} else {
					if (isesc = *codep=='%') /* escape */
						codep++;
					else
						allesc = 0;
					oput(*codep);
					if (*codep == '\033')
						oput(*++codep);
					else if (*codep != '\b' && !isesc)
						for (j = oj; --j > 0; ) {
							oput('\b');
							oput(*codep);
						}
					codep++;
				}
			}
			if (allesc && --j > 0) {
				oput('\b');
				goto loop;
			}
		}
		if (!w)
			for (j = phyw / t.Char; j > 0; j--)
				oput('\b');
	}
}


char *
plot(char *x)
{
	register int	i;
	register char	*j, *k;

	oputs(t.ploton);
	k = x;
	if ((*k & 0377) == 0200)
		k++;
	for (; *k; k++) {
		if (*k == '%') {	/* quote char within plot mode */
			oput(*++k);
		} else if (*k & 0200) {
			if (*k & 0100) {
				if (*k & 040)
					j = t.up; 
				else 
					j = t.down;
			} else {
				if (*k & 040)
					j = t.left; 
				else 
					j = t.right;
			}
			if ((i = *k & 037) == 0) {	/* 2nd 0200 turns it off */
				++k;
				break;
			}
			while (i--)
				oputs(j);
		} else 
			oput(*k);
	}
	oputs(t.plotoff);
	return(k);
}


void
move(void)
{
	register int k;
	register char	*i, *j;
	char	*p, *q;
	int	iesct, dt;

	iesct = esct;
	if (esct += esc)
		i = "\0"; 
	else 
		i = "\n\0";
	j = t.hlf;
	p = t.right;
	q = t.down;
	if (lead) {
		if (lead < 0) {
			lead = -lead;
			i = t.flr;
			/*	if(!esct)i = t.flr; else i = "\0";*/
			j = t.hlr;
			q = t.up;
		}
		if (*i & 0377) {
			k = lead / t.Newline;
			lead = lead % t.Newline;
			while (k--)
				oputs(i);
		}
		if (*j & 0377) {
			k = lead / t.Halfline;
			lead = lead % t.Halfline;
			while (k--)
				oputs(j);
		} else { /* no half-line forward, not at line begining */
			k = lead / t.Newline;
			lead = lead % t.Newline;
			if (k > 0) 
				esc = esct;
			i = "\n";
			while (k--) 
				oputs(i);
		}
	}
	if (esc) {
		if (esc < 0) {
			esc = -esc;
			j = "\b";
			p = t.left;
		} else {
			j = " ";
			if (hflg)
				while ((dt = dtab - (iesct % dtab)) <= esc) {
					if (dt % t.Em)
						break;
					oput(TAB);
					esc -= dt;
					iesct += dt;
				}
		}
		k = esc / t.Em;
		esc = esc % t.Em;
		while (k--)
			oputs(j);
	}
	if ((*t.ploton & 0377) && (esc || lead)) {
		oputs(t.ploton);
		esc /= t.Hor;
		lead /= t.Vert;
		while (esc--)
			oputs(p);
		while (lead--)
			oputs(q);
		oputs(t.plotoff);
	}
	esc = lead = 0;
}


void
ptlead(void)
{
	move();
}


void
dostop(void)
{
	char	junk;

	flusho();
	read(2, &junk, 1);
}


/*ARGSUSED*/
void
newpage(int unused)
{
	realpage++;
}

void
pttrailer(void){;}
