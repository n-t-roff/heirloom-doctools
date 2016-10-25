#include "hyphen.h"
#include <stdio.h>
#include <string.h>
#include "global.h"

int
main(int argc __unused, char **argv)
{
	HyphenDict	*hd;
	char	buf[512], hbuf[512];
	int	i, j;

	hd = hnj_hyphen_load(argv[1]);
	while (fgets(buf, sizeof buf, stdin) != NULL) {
		j = strlen(buf);
		buf[--j] = '\0';
		hnj_hyphen_hyphenate(hd, buf, j, hbuf);
		for (i = 0; i < j; i++)
			if ((hbuf[i] - '0') & 1)
				printf("%c %.*s-%s\n", hbuf[i],
						i+1, buf, &buf[i+1]);
	}
	return 0;
}
