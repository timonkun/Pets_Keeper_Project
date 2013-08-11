#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define CONFIG_FILE "control.txt"
#define URL_PATH   "http://kaffeel.org/timeonkun/control.txt"
//"http://timonkun.me/webcam/control.txt"
#define CAR_IN1    "/sys/devices/platform/qt210_gpio_ctl/gph3_0"
#define CAR_IN2    "/sys/devices/platform/qt210_gpio_ctl/gph3_1"
#define CAR_IN3    "/sys/devices/platform/qt210_gpio_ctl/gph3_2"
#define CAR_IN4    "/sys/devices/platform/qt210_gpio_ctl/gph3_3"

typedef struct{
	char pKeyword[40];
	int  iItemNum;
} Keyword_Struct;

typedef struct{
	int iCtrl_webcam;
	int iCtrl_fan;
	int iCtrl_feed;
    int iCtrl_car;
} Control_Struct;

enum ctrl_code {
CTRL_CLOSE,  //0
CTRL_OPEN,
};

enum car_ctrl_code {
    CAR_CTRL_STOP,      //0
    CAR_CTRL_FORWARD,   //1
    CAR_CTRL_BACKWARD,  //2
    CAR_CTRL_LEFT,      //3
    CAR_CTRL_RIGHT,     //4
    CAR_CTRL_CENTER_LEFT,   //5
    CAR_CTRL_CENTER_RIGHT,  //6
};

enum ctrl_mode {
MODE_WEBCAM,  //0
MODE_FAN,
MODE_FEED,
MODE_CAR,
MODE_MAX,
};

Control_Struct cur_ctrl = {
0,	// iCtrl_webcam 
0,	// iCtrl_fan
0,	// iCtrl_feed
0,  // iCtrl_car
};

Control_Struct last_ctrl = {
	0, 0, 0, 0,
};

static Keyword_Struct config_keyword[] = {
{"<CTRL_MODE_WEBCAM>", 1},  //0
{"<CTRL_MODE_FAN>",    1},
{"<CTRL_MODE_FEED>",   1},
{"<CTRL_MODE_CAR>",    1},
};

int compare_update(void);
int load_config_data(Control_Struct *tmp_ctrl);
void download_config_file(void);

int webcam_operation(int ctrl_code);
int fan_operation(int ctrl_code);
int feed_operation(int ctrl_code);
int car_operation(int ctrl_code);

int main(int argc, char **argv)
{
    char *separateur;
    char *sleep_time_string  = NULL;
	int sleep_time = 1;
    int i;
    
    for (i = 1; i < argc; i++) 
    {
        /* skip bad arguments */
		if (argv[i] == NULL || *argv[i] == 0 || *argv[i] != '-') {
		    continue;
		}
        if (strcmp(argv[i], "-s") == 0) {   // sleep
		    if (i + 1 >= argc) {
			printf("No parameter specified with -s, aborting.\n");
			exit(1);
		    }
            sleep_time_string = strdup(argv[i + 1]);
            sleep_time = strtoul(sleep_time_string, &separateur, 10);
		}
    }
    
	while(1)
	{
		download_config_file();
		compare_update();	// don't compare the entire file, compare the values.
		sleep(sleep_time);
	}

	return 0;
}

/***
void remove_config_file(void)
{
    pid_t pid;
 
    pid = fork();
    switch(pid)
    {
        case 0:
           execlp("rm", "rm", CONFIG_FILE, NULL);
           break;
        case -1:
           perror("fork error.");
           exit(1);
      default:
      break;
    }
}
***/   

void download_config_file(void)
{
#if 1
	pid_t pid;

	pid = fork();
	switch(pid)
	{
		case 0:
			execlp("wget", "wget", /*"-N",*/ "-O", CONFIG_FILE, "-q", \
					URL_PATH, NULL);
			break;
		case -1:
			perror("fork error.");
			exit(1);
		default:
			break;
	}
	pid = wait(NULL);
	//if(pid > 0)
	//	printf("wget exit success.\n");
#else
    
    system("wget -O control.txt -q http://timonkun.me/webcam/control.txt");
    printf("wget success.\n");
#endif
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
		return -1;
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
                        case MODE_CAR:
							tmp_ctrl->iCtrl_car = atoi(str_value);
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
					return -1;
				}
			
				for(i=0; i<256; i++)
					string[i] = '\0';
				for(i=0; i<16; i++)
					str_value[i] = '\0';
			}
		}
	}

	if(0 != fclose(fp))
		perror("fclose error.""");

	return 0;
}

void start_webcam(void)
{
	system("sh /samples/mjpg-streamer/start.sh &");
	printf("%s\n", __func__);
}

void stop_webcam()
{
	system("sh /samples/pkill.sh mjpg_streamer");
	printf("%s\n", __func__);
}

int webcam_operation(int ctrl_code)
{
	printf("%s: ctrl_code=%d\n", __func__, ctrl_code);

	switch(ctrl_code)
	{
		case 0:
			stop_webcam();
			break;
		case 1:
			start_webcam();
			break;
		default:
            printf("[%s] ERR: out of range. ctrl_code=%d\n", __func__, ctrl_code);
			break;
	}
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

/***
 * #define CAR_IN1    "/sys/devices/platform/qt210_gpio_ctl/gph3_0"
 * #define CAR_IN2    "/sys/devices/platform/qt210_gpio_ctl/gph3_1"
 * #define CAR_IN3    "/sys/devices/platform/qt210_gpio_ctl/gph3_2"
 * #define CAR_IN4    "/sys/devices/platform/qt210_gpio_ctl/gph3_3"
 ***/
int car_operation(int ctrl_code)
{
    
    switch(ctrl_code)
    {
        case CAR_CTRL_STOP: //0
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_0");
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_1");
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_2");
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_3");
            printf("%s: CAR_CTRL_STOP\n", __func__);
            break;
        case CAR_CTRL_FORWARD:  //1
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_0");
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_1");
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_2");
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_3");
            printf("%s: CAR_CTRL_FORWARD\n", __func__);
            break;
        case CAR_CTRL_BACKWARD: //2
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_0");
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_1");
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_2");
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_3");
            printf("%s: CAR_CTRL_BACKWARD\n", __func__);
            break;
        case CAR_CTRL_LEFT: //3
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_0");
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_1");
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_2");
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_3");
            printf("%s: CAR_CTRL_LEFT\n", __func__);
            break;
        case CAR_CTRL_RIGHT:    //4
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_0");
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_1");
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_2");
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_3");
            printf("%s: CAR_CTRL_RIGHT\n", __func__);
            break;
        case CAR_CTRL_CENTER_LEFT: //5
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_0");
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_1");
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_2");
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_3");
            printf("%s: CAR_CTRL_CENTER_LEFT\n", __func__);
            break;
        case CAR_CTRL_CENTER_RIGHT:    //6
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_0");
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_1");
            system("echo 1 > /sys/devices/platform/qt210_gpio_ctl/gph3_2");
            system("echo 0 > /sys/devices/platform/qt210_gpio_ctl/gph3_3");
            printf("%s: CAR_CTRL_CENTER_RIGHT\n", __func__);
            break;
        default:
            printf("[%s] ERR: out of range. ctrl_code=%d\n", __func__, ctrl_code);
            break;
    }
    
	return 0;
}

// read config file, compare the ctrl values.
int compare_update(void)
{
	load_config_data(&cur_ctrl);
	printf("webcam=%d, fan=%d, feed=%d, car=%d\n", cur_ctrl.iCtrl_webcam, \
			cur_ctrl.iCtrl_fan, cur_ctrl.iCtrl_feed, cur_ctrl.iCtrl_car);

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
    
    if(cur_ctrl.iCtrl_car != last_ctrl.iCtrl_car)
	{
		//printf("last_ctrl.car=%d, cur_ctrl.car=%d\n", 
		//		last_ctrl.iCtrl_car, cur_ctrl.iCtrl_car);
		car_operation(cur_ctrl.iCtrl_car);
		last_ctrl.iCtrl_car = cur_ctrl.iCtrl_car;
	}

	return 0;
}
