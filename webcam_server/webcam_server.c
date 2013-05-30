#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define URL_PATH  "kaffeel.org/timeonkun/address.txt"
#define CONFIG_FILE "address.txt"


typedef struct{
	char pKeyword[40];
	int  iItemNum;
} keyword_struct;

typedef struct{
	int iCtrlValue;
} control_struct;


enum ctrl_mode {
MODE_WEBCAM,
MODE_FAN,
MODE_FEED,
MODE_MAX,
};

control_struct cur_ctrl[MODE_MAX] = {
0,	// MODE_WEBCAM 
0,	// MODE_FAN
0,	// MODE_FEED
};

control_struct last_ctrl[MODE_MAX] = {
	0, 0, 0,
};
static keyword_struct config_keyword[] = {
{"<CTRL_MODE_WEBCAM>", 1},  //0
{"<CTRL_MODE_FAN>",    1},
{"<CTRL_MODE_FEED>",   1},
};

int compare_update();
int load_config_data(control_struct tmp_ctrl[]);
void download_config_file(void);

int main(int argc, char **argv)
{
	

	while(1)
	{
		//download_config_file();
		compare_update();	// don't compare the entire file, compare the values.
		sleep(3);
	}

	return 0;
}

void download_config_file(void)
{
	pid_t pid;

	pid = fork();
	switch(pid)
	{
		case 0: // "-q"
			execlp("wget", "wget", "-N", "-q", \
					URL_PATH, NULL);
			break;
		case -1:
			perror("fork error.");
			exit(1);
		default:
			break;
	}
}

int load_config_data(control_struct tmp_ctrl[])
{
	int i, j, string_len;
	char string[256];
	char str_value[16];
	int  ctrl_value;

	FILE *fp = fopen(CONFIG_FILE, "r");
	if(NULL == fp)
	{
		perror("Can't open CONFIG_FILE");
		exit(1);
	}


	while(fgets(string, 256, fp))
	{
		for(j = MODE_WEBCAM; j < MODE_MAX; j++)
		{
			string_len = strlen(config_keyword[j].pKeyword);
			if(0 == strncmp(config_keyword[j].pKeyword, string, string_len))
			{
				if(NULL != fgets(str_value, 16, fp))
				{
					cur_ctrl[j].iCtrlValue = atoi(str_value);
					printf("%s: %d\n%d\n", string, \
						config_keyword[j].iItemNum, cur_ctrl[j].iCtrlValue);
				}
				else
				{
					perror("fgets str_value error.");
					exit(1);
				}
			
				for(i=0; i<256; i++)
					string[i] = '\0';
				for(i=0; i<16; i++)
					str_value[i] = '\0';
			}
		}
	}

	return 0;
}

// read config file, compare the ctrl values.
int compare_update()
{
	load_config_data(cur_ctrl);
}
