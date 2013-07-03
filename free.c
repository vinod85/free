/*
 * free.c - Display FreeBSD memory information
 * Wed Nov 26 19:34:54 IST 2008 vinod <vinod@segfault.in>
 * License: http://opensource.org/licenses/BSD-2-Clause
 */

#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const char *version = "$Id: free,v 0.1.3 2008/11/26 19:34:54 IST vinod $";

int
get_sysctl(char *name)
{
	int mib[4], value, i;
	size_t len, miblen = 1;

	for(i = 0; name[i] != '\0'; i++) 
		if(name[i] == '.')
			miblen++;
	len = miblen;
	sysctlnametomib(name, mib, &len);
	len = sizeof(value);
	sysctl(mib, miblen, &value, &len, NULL, 0);

	return value;
}

void
usage(void)
{
	fprintf(stderr, "usage: free [-b|-k|-m|-g] [-t] [-v]\n" \
	"  -b,-k,-m,-g show output in bytes, KB, MB, or GB\n" \
	"  -t display logical summary for RAM\n" \
	"  -v display version information and exit\n");
}

int
main(int argc, char *argv[])
{
	int c, vflag = 0, tflag = 0;
	int factor = 1;
	long int physmem, realmem;
	long int vmactive, vminactive, vmfree, vmcache, vmpage, vmwire;
	long int memfree, memused;
	long int pagesize;

	opterr = 0;

	while ((c = getopt(argc, argv, "bghkmtv")) != -1) {
		switch (c) {
			case 'b':
				factor = 1;
				break;
			case 'g':
				factor = 1024*1024*1024;

			case 'h':
				usage();
				exit(EXIT_SUCCESS);
			case 'k':
				factor = 1024;
				break;
			case 'm':
				factor = 1024*1024;
				break;
			case 't':
				tflag = 1;
				break;
			case 'v':
				vflag = 1;
				break;
			case '?':
			default:
				fprintf(stderr, "%s: invalid option -- %c\n", argv[0], optopt);
				usage();
				exit(EXIT_FAILURE);
		}
	}

	argc -= optind;
	argv += optind;

	if(vflag) {
		fprintf(stderr, "%s\nbuilt %s %s\n", version,
				__DATE__, __TIME__);
		exit(EXIT_SUCCESS);
	}

	physmem    = labs(get_sysctl("hw.physmem"));
	realmem    = labs(get_sysctl("hw.realmem"));
	pagesize   = labs(get_sysctl("hw.pagesize"));

	vmpage     = labs(get_sysctl("vm.stats.vm.v_page_count") * pagesize);
	vmwire     = labs(get_sysctl("vm.stats.vm.v_wire_count") * pagesize);
	vmactive   = labs(get_sysctl("vm.stats.vm.v_active_count") * pagesize);
	vminactive = labs(get_sysctl("vm.stats.vm.v_inactive_count") * pagesize);
	vmcache    = labs(get_sysctl("vm.stats.vm.v_cache_count") * pagesize);
	vmfree     = labs(get_sysctl("vm.stats.vm.v_free_count") * pagesize);

	printf("         %15s %15s %15s %15s %15s %15s\n", "total", "active", "free", "inactive", "wire", "cached");
	printf("Memory:  %15ld %15ld %15ld %15ld %15ld %15ld\n",
			realmem/factor,
			vmactive/factor,
			vmfree/factor,
			vminactive/factor,
			vmwire/factor,
			vmcache/factor);

	/*
	 * logical summary
	 */
	if(tflag) {
		memfree = vminactive + vmfree + vmcache;
		memused	= realmem - memfree;

		printf("Summary: %15ld %15ld %15ld\n",
				realmem/factor,
				memused/factor,
				memfree/factor);
	}

	return (EXIT_SUCCESS);
}
