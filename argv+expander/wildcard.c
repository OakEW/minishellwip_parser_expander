#include <stdio.h>
#include <dirent.h>

int main(void)
{
	DIR				*cwd;
	struct dirent	*entry;

	cwd = opendir(".");
	if (cwd == NULL) {
		perror("opendir");
		return (1);
	}
	while ((entry = readdir(cwd)) != NULL) {
		printf("%s\n", entry->d_name);
	}
	closedir(cwd);
	return (0);
}



// no matching is not an error, just return the pattern as is
//  * does not match hidden files (those starting with .)
// .* matches . and .. and other hidden files
// handle multiple wildcards, e.g. l*x*c
// ./* matches all files in the current directory with prefix ./ NO hidden
// heandle entry with * e.g. file*name

// return 1 for matching pattern and entry, 0 for no match
int match_wildcard(const char *pat, const char *ent)
{

}


//print 1 for match
// int main(void)
// {
// 	printf("%d\n", match_wildcard("*", ".hidden"));
// 	return (0);
// }

