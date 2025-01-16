#include <limlog.h>
#include "myregex.h"


int regex(const char* inputstr, const char* pattern,
			int & start, int & end, int index){
	regex_t reg;
	char regerrbuf[256] = {0};

	int c = regcomp(&reg, pattern, REG_EXTENDED);
	if(c != 0){
		regerror(c, &reg, regerrbuf, sizeof(regerrbuf));
		LOG_ERROR << regerrbuf;
		return c;
	}
	const size_t matchsz = 5;
	regmatch_t pmatch[5];

	c = regexec(&reg, inputstr, matchsz, pmatch, 0);
	if(c == REG_NOMATCH){
		return c;
	}
	else if(c == 0){
		start = pmatch[index].rm_so;
		end = pmatch[index].rm_eo;
	}
	else{
		LOG_ERROR << "REGEX MATCH ERROR;";
		regerror(c, &reg, regerrbuf, sizeof(regerrbuf));
		LOG_ERROR << regerrbuf;
		return c;
	}
	regfree(&reg);
	return REG_NOERROR;
}

int regex(const char* inputstr, const char* pattern,
			int matchsz, regmatch_t *pmatch){
	regex_t reg;
	char regerrbuf[256] = {0};

	int c = regcomp(&reg, pattern, REG_EXTENDED);
	if(c != 0){
		regerror(c, &reg, regerrbuf, sizeof(regerrbuf));
		LOG_ERROR << regerrbuf;
		return c;
	}

	c = regexec(&reg, inputstr, matchsz, pmatch, 0);
	if(c == REG_NOMATCH){
		return c;
	}
	else if(c == 0){
		return REG_NOERROR;
	}
	else{
		LOG_ERROR << "REGEX MATCH ERROR;";
		regerror(c, &reg, regerrbuf, sizeof(regerrbuf));
		LOG_ERROR << regerrbuf;
		return c;
	}
	regfree(&reg);
	return REG_NOERROR;
}

std::string regex(const char* inputstr, const char* pattern, int & ret, int index){
	int start, end;
	std::string retstr;
	ret = regex(inputstr, pattern, start, end, index);
	if(ret == REG_NOERROR){
		retstr = std::string(inputstr+start, end-start);
	}
	return retstr;
}
