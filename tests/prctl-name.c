/*
 * Check decoding of prctl PR_GET_NAME/PR_SET_NAME operations.
 *
 * Copyright (c) 2016 Eugene Syromyatnikov <evgsyr@gmail.com>
 * Copyright (c) 2016 Dmitry V. Levin <ldv@altlinux.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "tests.h"

#ifdef HAVE_PRCTL
# include <sys/prctl.h>
#endif

#if defined HAVE_PRCTL && defined PR_GET_NAME && defined PR_SET_NAME

# include <stdio.h>
# include <string.h>
# include <unistd.h>

int
main(void)
{
	static const char str[] = "0123456789abcdef";
	static const int len = sizeof(str);
	static const int len1 = len - 1;
	char *const name = tail_memdup(str, len);
	char *const name1 = name + 1;
	int i;
	int rc;

	rc = prctl(PR_SET_NAME, NULL);
	printf("prctl(PR_SET_NAME, NULL) = %s\n", sprintrc(rc));

	for (i = 0; i <= len1; ++i) {
		rc = prctl(PR_SET_NAME, name + len1 - i);
		printf("prctl(PR_SET_NAME, \"%.*s\"%s) = %s\n",
		       i < len1 - 1 ? i : len1 - 1,
		       str + len1 - i,
		       i < len1 - 1 ? "" : "...",
		       sprintrc(rc));
	}

	name[0] = -1;
	memcpy(name1, str, len1);

	for (i = 0; i <= len1; ++i) {
		rc = prctl(PR_SET_NAME, name1 + len1 - i);
		if (i < len1 - 1)
			printf("prctl(PR_SET_NAME, %p) = %s\n",
			       name1 + len1 - i, sprintrc(rc));
		else
			printf("prctl(PR_SET_NAME, \"%.*s\"...) = %s\n",
			       len1 - 1, str + len1 - i, sprintrc(rc));
	}

	rc = prctl(PR_GET_NAME, NULL);
	printf("prctl(PR_GET_NAME, NULL) = %s\n", sprintrc(rc));

	for (i = 0; i < len1; ++i) {
		rc = prctl(PR_GET_NAME, name1 + len1 - i);
		printf("prctl(PR_GET_NAME, %p) = %s\n",
		       name1 + len1 - i, sprintrc(rc));
	}

	rc = prctl(PR_GET_NAME, name1);
	if (rc)
		printf("prctl(PR_GET_NAME, %p) = %s\n",
		       name1, sprintrc(rc));
	else
		printf("prctl(PR_GET_NAME, \"%.*s\") = %s\n",
		       len1 - 1, name1, sprintrc(rc));

	puts("+++ exited with 0 +++");
	return 0;
}

#else

SKIP_MAIN_UNDEFINED("HAVE_PRCTL && PR_GET_NAME && PR_SET_NAME")

#endif