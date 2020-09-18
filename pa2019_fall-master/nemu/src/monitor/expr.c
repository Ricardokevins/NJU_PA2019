#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

CPU_STATE cpu;

enum
{
	NOTYPE = 256,
	EQ,
	NUM,
	REG,
	SYMB,
	NE,
	AND,
	OR,
	DEREF,
	HEX,
	NEG

	/* TODO: Add more token types */

};

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE}, // white space
	{"\\+", '+'},
	{"\\(",'('},
	{"\\*",'*'},
	{"\\)",')'},
	{"/",'/'},
	{"==",EQ},
	{"!=",NE},
	{"\\|\\|",OR},
	{"!",'!'},
	{"&&",AND},
	{"\\-",'-'},
	{"[a-zA-Z_]+",SYMB},
	{"[0-9]+",NUM},
	{"[0-9a-fA-F]+",HEX},
	{"\\$e[a,b,c,d]x",REG},
	{"\\$eip",REG},
	{"\\$e[s,d]i",REG},
	{"\\$e[s,b]p",REG}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

uint32_t eval(int p, int q, bool *valid);
uint32_t look_up_symtab(char *, bool *);

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token
{
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

uint32_t Addr_SYMB(int p)
{
    bool success = true;
    return look_up_symtab(tokens[p].str, &success);
}

static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//printf("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

				switch (rules[i].token_type)
				{
					case NOTYPE:
						break;
					case NUM:
					case HEX:
					case REG:
					case SYMB:
						for(int i=0;i<substr_len;i++)
							tokens[nr_token].str[i]=substr_start[i];
					default:
						tokens[nr_token].type = rules[i].token_type;
						nr_token++;
						break;
				}

				break;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

int which_operand(int i){
	return tokens[i].type!=NOTYPE && tokens[i].type!=NUM && tokens[i].type!=REG && tokens[i].type!=SYMB && tokens[i].type!=HEX && tokens[i].type!='(' && tokens[i].type!=')';
}

uint32_t expr(char *e, bool *success)
{
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}

	for(int i=0;i<nr_token;i++)
	{
		if(tokens[i].type == '*' && (i == 0 || which_operand(i-1)))
			tokens[i].type = DEREF;
		if(tokens[i].type == '-' && (i == 0 || which_operand(i-1)))
			tokens[i].type = NEG;
	}
	bool valid = true;
    uint32_t my_result = eval(0, nr_token - 1, &valid);
    if (valid == true)
    {
        *success = true;
        return my_result;
    }
    else
    {
        *success = false;
        return 0;
    }
	return 0;
}



uint32_t get_Value_REG(int p)
{
	int select_code=6;
	if(!strcmp(tokens[p].str, "$eax"))
		select_code=0;
	if (!strcmp(tokens[p].str, "$ebx"))
		select_code=3;
	if (!strcmp(tokens[p].str, "$ebp"))
		select_code=5;
	if (!strcmp(tokens[p].str, "$ecx"))
		select_code=1;
	if (!strcmp(tokens[p].str, "$edx"))
		select_code=2;
	if (!strcmp(tokens[p].str, "$edi"))
		select_code=7;
	if (!strcmp(tokens[p].str, "$eip"))
		return cpu.eip;
	if (!strcmp(tokens[p].str, "$esp"))
		select_code=4;
    return cpu.gpr[select_code].val;
}



uint32_t eval(int p, int q, bool *valid)
{
    if (valid == false)
        return 0;
    if (p > q)
    {
        valid = false;
        return 0;
    }
    else if (p == q)
    {
        uint32_t num;
        if (tokens[p].type == NUM)
        {
            sscanf(tokens[p].str, "%d", &num);
        }
        else if (tokens[p].type == HEX)
        {
            sscanf(tokens[p].str, "%x", &num);
        }
        else if (tokens[p].type == SYMB)
        {
            num = Addr_SYMB(p);
        }
        else if (tokens[p].type == REG)
        {
        	num = get_Value_REG(p);
        }
        return num;
    }
    return 0;
}


