#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define CONFIG_FILE "control.txt"
#define URL_PATH  "kaffeel.org/timeonkun/control.txt"

typedef struct{
	char pKeyword[40];
	int  iItemNum;
} Keyword_Struct;

typedef struct{
	int iCtrl_webcam;
	int iCtrl_fan;
	int iCtrl_feed;
} Control_Struct;

enum ctrl_code {
CTRL_CLOSE,  //0
CTRL_OPEN,

};

enum ctrl_mode {
MODE_WEBCAM,  //0
MODE_FAN,
MODE_FEED,
MODE_MAX,
};

Control_Struct cur_ctrl = {
0,	// iCtrl_webcam 
0,	// iCtrl_fan
0,	// iCtrl_feed
};

Control_Struct last_ctrl = {
	0, 0, 0,
};

static Keyword_Struct config_keyword[] = {
{"<CTRL_MODE_WEBCAM>", 1},  //0
{"<CTRL_MODE_FAN>",    1},
{"<CTRL_MODE_FEED>",   1},
};

int compare_update(void);
int load_config_data(Control_Struct *tmp_ctrl);
void download_config_file(void);

int webcam_operation(int ctrl_code);
int fan_operation(int ctrl_code);
int feed_operation(int ctrl_code);

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
		case 0:
			execlp("wget", "wget", /*"-N",*/ "-q", \
					URL_PATH, NULL);
			break;
		case -1:
			perror("fork error.");
			exit(1);
		default:
			break;
	}
}

int load_config_data(Control_Struct *tmp_ctrl)
{
	int i, index, string_len;
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
		for(index = MODE_WEBCAM; index < MODE_MAX; index++)
		{
			string_len = strlen(config_keyword[index].pKeyword);
			if(0 == strncmp(config_keyword[index].pKeyword, string, string_len))
			{
				if(NULL != fgets(str_value, 16, fp))
				{
					switch(index)
					{
						case MODE_WEBCAM:
							tmp_ctrl->iCtrl_webcam = atoi(str_value);
							break;
						case MODE_FAN:
							tmp_ctrl->iCtrl_fan = atoi(str_value);
							break;
						case MODE_FEED:
							tmp_ctrl->iCtrl_feed = atoi(str_value);
							break;
						default:
							printf("[%s] Error. index=%d", __func__, index);
							exit(1);
					}
					//printf("%s: %d\n%d\n", string, \
						config_keyword[index].iItemNum, atoi(str_value));
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

int webcam_operation(int ctrl_code)
{
	printf("%s: ctrl_code=%d\n", __func__, ctrl_code);
	return 0;
}

int fan_operation(int ctrl_code)
{

	printf("%s: ctrl_code=%d\n", __func__, ctrl_code);
	return 0;
}

int feed_operation(int ctrl_code)
{
	printf("%s: ctrl_code=%d\n", __func__, ctrl_code);
	return 0;
}

// read config file, compare the ctrl values.
int compare_update(void)
{
	load_config_data(&cur_ctrl);
	//printf("webcam=%d, fan=%d, feed=%d\n", cur_ctrl.iCtrl_webcam, \
			cur_ctrl.iCtrl_fan, cur_ctrl.iCtrl_feed);

	if(cur_ctrl.iCtrl_webcam != last_ctrl.iCtrl_webcam)
	{
		//printf("last_ctrl.webcam=%d, cur_ctrl.webcam=%d\n", \
				last_ctrl.iCtrl_webcam, cur_ctrl.iCtrl_webcam);
		webcam_operation(cur_ctrl.iCtrl_webcam);
		last_ctrl.iCtrl_webcam = cur_ctrl.iCtrl_webcam;
	}

	if(cur_ctrl.iCtrl_fan != last_ctrl.iCtrl_fan)
	{
		//printf("last_ctrl.fan=%d, cur_ctrl.fan=%d\n", \
				last_ctrl.iCtrl_fan, cur_ctrl.iCtrl_fan);
		fan_operation(cur_ctrl.iCtrl_fan);
		last_ctrl.iCtrl_fan = cur_ctrl.iCtrl_fan;
	}

	if(cur_ctrl.iCtrl_feed != last_ctrl.iCtrl_feed)
	{
		//printf("last_ctrl.feed=%d, cur_ctrl.feed=%d\n", \
				last_ctrl.iCtrl_feed, cur_ctrl.iCtrl_feed);
		feed_operation(cur_ctrl.iCtrl_feed);
		last_ctrl.iCtrl_feed = cur_ctrl.iCtrl_feed;
	}

	return 0;
}
