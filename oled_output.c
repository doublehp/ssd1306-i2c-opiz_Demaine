/*
 * Copyright (c) 2015, Vladimir Komendantskiy
 * MIT License
 *
 * SSD1306 demo of block and font drawing.
 */

//
// fixed for OrangePiZero by HypHop
//

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

#include "oled.h"
#include "font.h"

int oled_output(struct display_info *disp) {
	
// syntax :
// \0 (\x00) EOF = exit
// \n (\x0A) nelwline
// \f (\x0C) form feed = new page (blacnk page)
// \r (\x0D) CR	
// line is sent to screen after \n .

// I dislike font 1
// Font 2 is easier to read
// Font 3 is denser
// Can not change font tupe during run ; can not mix font types.
// 7 lines max

// { for i in $(seq 7) ; do echo "$i ABCDEFGHIJLMPQabcdghijklmnopqrs" ; done ; echo -e "\0" ; } | ./oled_output /dev/i2c-0 2

	int vpos;
	int vinc;
	int vstart;
	int vmax;
	char inputstr[9999];
	int c;
	int strpos;

	// 7 lines: 0 / 9 / 56	( font 3 only)
	// 6 lines: 3 / 10 / 53
	// 6 lines: 0 / 11 / 55
	// 5 lines: 3 / 12 / 51

	vstart=0;
	vinc=11;
	vmax=55;

	strpos=0;
	
	vpos=vstart;

	for (;;) {
		
		for(strpos=0;(c=getchar())>16;strpos++) {	
			inputstr[strpos]=c;
			if(strpos>9990) strpos=9990;
		}
		if (c<0) {
			usleep(1000);
			continue;
		}
		if(strpos!=0) { // do not sent buffer to disp if received control on empty line
			inputstr[strpos]='\0';	// terminate line
// DEBUG :
//		printf("'%s' %i / %c . p:%i . c:%i .\n",inputstr,c,c,strpos,inputstr[strpos-1]);
			strcat(inputstr,"                        "); // clear rest of old line
			inputstr[42]='\0';	// prevent bug about looping line on itself
			oled_putstrto(disp, 0, vpos, inputstr);
			oled_send_buffer(disp);
		}
		if(c==0) return (0);	// \0
		if(c==10) vpos+=vinc;	// \n
		if(c==12) vpos=vstart;	// \f
		if(c==13) vpos+=0;	// \r
		if (vpos>vmax) vpos=vmax;
		inputstr[0]=0;		// flush line
	}
	return 0;
}



void show_error(int err, int add) {
	//const gchar* errmsg;
	//errmsg = g_strerror(errno);
	printf("\nERROR: %i, %i\n\n", err, add);
	//printf("\nERROR\n");
}

void show_usage(char *progname) {
	printf("\nUsage:\n%s <I2C bus device node> [<font n (1|2|3)>]\n", progname);
}

int main(int argc, char **argv) {
	char filename[32];
	struct display_info disp;

	if (argc < 2) {
		show_usage(argv[0]);
		return -1;
	}

	memset(&disp, 0, sizeof(disp));

	sprintf(filename, "%s", argv[1]);
	disp.address = OLED_I2C_ADDR;
	disp.font = font2;

	//	if (oled_open(&disp, filename) < 0 ||
	//	    oled_init(&disp)           < 0 ||
	//	    oled_demo(&disp)           < 0)
	//	{

	int e;

	e = oled_open(&disp, filename);

	if (e < 0) {
		show_error(1, e);
	} else {
		e = oled_init(&disp);
		if (e < 0) {
			show_error(2, e);
		} else {

			if(argc==3) {
				if((char)argv[2][0]=='1') disp.font = font1;
				if((char)argv[2][0]=='2') disp.font = font2;
				if((char)argv[2][0]=='3') disp.font = font3;
			}
			if (oled_output(&disp) < 0) {
				show_error(3, 777);
			}
		}
	}

	return 0;
}
