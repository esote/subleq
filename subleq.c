#include <err.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int
subleq(int_fast64_t *const ops)
{
	/* instruction pointers */
	register int_fast64_t ip, nip;

	/* subleq operands */
	register int_fast64_t a, b, c;

	ip = 0;

	while (ip >= 0) {
		nip = ip + 3;
		a = ops[ip];
		b = ops[ip + 1];
		c = ops[ip + 2];

		if (a == -1) {
			if ((ops[b] = getchar()) == EOF) {
				return -1;
			}
		} else if (b == -1) {
			if (putchar((int)ops[a]) == EOF) {
				return -1;
			}
		} else {
			ops[b] -= ops[a];
			if (ops[b] <= 0) {
				nip = c;
			}
		}

		ip = nip;
	}

	return 0;
}

int
main(int argc, char *argv[])
{
	FILE *in;
	size_t i, n, maxn;
	int_fast64_t *ops;

	(void)argc;
	argv++;

	in = stdin;
	ops = NULL;
	maxn = 0;

	do {
		if (*argv != NULL) {
			if (strcmp(*argv, "-") == 0) {
				in = stdin;
			} else if ((in = fopen(*argv, "r")) == NULL) {
				err(1, "fopen");
			}

			argv++;
		}

		if (fscanf(in, "%zu", &n) == EOF) {
			err(1, "fscanf");
		}

		if (n > maxn) {
			maxn = n;

			if ((ops = realloc(ops, maxn * sizeof(*ops))) == NULL) {
				err(1, "realloc");
			}
		} else {
			memset(ops + n, 0, (maxn - n) * sizeof(*ops));
		}

		for (i = 0; i < n; ++i) {
			if (fscanf(in, "%"PRIdFAST64, &ops[i]) == EOF) {
				err(1, "fscanf");
			}
		}

		if (in != stdin && fclose(in) == EOF) {
			err(1, "fclose");
		}

		if (subleq(ops) == -1) {
			err(1, "subleq");
		}
	} while (*argv != NULL);

	if (ops != NULL) {
		free(ops);
	}

	return 0;
}
