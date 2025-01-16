#ifndef __MYREGEX_H__
#define __MYREGEX_H__

#include <regex.h>
#include <string>

int regex(const char* inputstr, const char* pattern,
			int & start, int & end, int index = 0);
		
int regex(const char* inputstr, const char* pattern,
			int matchsz, regmatch_t *pmatch);

std::string regex(const char* inputstr, const char* pattern, int & ret, int index = 0);

#endif