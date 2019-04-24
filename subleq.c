#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#if DEBUG
#include <err.h>
#include <limits.h>
#endif

static void
subleq(int_fast64_t *restrict const ops)
{
	/* instruction pointers */
	static int_fast64_t ip;
	static int_fast64_t nip;

	/* subleq operands */
	static int_fast64_t a;
	static int_fast64_t b;
	static int_fast64_t c;

	ip = 0;

	while (ip >= 0) {
		nip = ip + 3;
		a = ops[ip];
		b = ops[ip + 1];
		c = ops[ip + 2];

		if (a == -1) {
			ops[b] = getchar();
#if DEBUG
			if (ops[b] == EOF) {
				warnx("ip: %"PRIdFAST64" (arg b): read val "
					"'%"PRIdFAST64"' with EOF", ip + 1,
					ops[b]);
			}
#endif
		} else if (b == -1) {
#if DEBUG
			if (ops[a] > UCHAR_MAX) {
				warnx("ip: %"PRIdFAST64" (arg a): val "
					"'%"PRIdFAST64"' may be printed wrong",
					ip, ops[a]);
			}

			if (putchar((int)ops[a]) == EOF) {
				warnx("ip: %"PRIdFAST64" (arg a): val "
					"'%"PRIdFAST64"' printed with EOF", ip,
					ops[a]);
			}
#else
			putchar(ops[a]);
#endif
		} else {
			ops[b] -= ops[a];

			if (ops[b] <= 0) {
				nip = c;
			}
		}

		ip = nip;
	}
}

int
main(int argc, char *argv[])
{
	FILE *in;
	size_t i;
	size_t n;
	int_fast64_t *ops;
	int ret;

	in = stdin;
	ops = NULL;
	ret = 0;

	(void)argc;
	(void)*argv++;

	do {
		if (*argv) {
#ifdef CAREFREE
			in = fopen(*argv, "r");
#else
			if (*argv[0] == '-' && *argv[1] == '\0') {
				in = stdin;
			} else if ((in = fopen(*argv, "r")) == NULL) {
				ret = 1;
			}
#endif

			(void)*argv++;
		}

		fscanf(in, "%zu", &n);

		ops = realloc(ops, n * sizeof(int_fast64_t));

#ifndef CAREFREE
		if (ops == NULL && n != 0) {
			ret = 1;
		}
#endif

		for (i = 0; i < n; ++i) {
			fscanf(in, "%"PRIdFAST64, &ops[i]);
		}

#ifndef CAREFREE
		if (in != stdin && fclose(in) == EOF) {
			ret = 1;
		}
#endif

		subleq(ops);
	} while (*argv);

	if (ops != NULL) {
		free(ops);
	}

	return ret;
}
