Initial project by Vladimir Komendantskiy

Uploaded to GitHub for backup purposes by Benoit-Pierre DEMAINE.

With greatings to ForKicks
http://www.orangepi.org/orangepibbsen/forum.php?mod=viewthread&tid=4208&pid=45047&page=1&extra=#pid45047

Tested and working by Benoit-Pierre Demaine:
- SH1106 OLED 128x64 screen
- Orange Pi Zero
- Armbian_5.35_Orangepizero_Ubuntu_xenial_next_4.13.16

############################################################### Installation

1. $ sudo armbian-config
2. In GUI: System -> Hardware -> Toggle on i2c-0 . Save, and follow instructions
3. $ sudo apt-get update && sudo apt-get install i2c-tools
4. $ i2cdetect -r 0

You will be greeted with this(Awnser yes):
```
WARNING! This program can confuse your I2C bus, cause data loss and worse!
I will probe file /dev/i2c-0 using read byte commands.
I will probe address range 0x03-0x77.
Continue? [Y/n] Y
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- 3c -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --
```

My device shows at 0x3c (not 0x78 as shown on ali). A few other addresses are possible (0x3D, 0x79).

If you get a value different from 0x3c, you will need to fix the value of OLED_I2C_ADDR in oled.h .

5. $ git clone https://github.com/doublehp/ssd1306-i2c-opiz_Demaine
6. $ cd ssd1306-i2c-opiz_Demaine
7. $ make
8. $ { for i in $(seq 6) ; do echo "$i ABCDEFGHIJLMPQabcdghijklmnopqrs" ; done ; echo -e "\0" ; } | ./oled_output /dev/i2c-0 2

########################################################## Purposes

This is a Alpha project. I do not consider this as "packageable" by distributions, but it serves good purposes for me.

I have not been able to use other python templaces and tutorials, and, I did not want to develop my whole project in C.

This is app is an "output wrapper". It reads the output from a shell script, and forwards data to the screen.

My aim is to make I2C OLED screen usable from shell script, without messing with Python or C at the top project level.

########################################################## Controls

- \0 (\x00) EOF = exit
- \n (\x0A) nelwline
- \f (\x0C) form feed = new page (blacnk page)
- \r (\x0D) CR 
- line is sent to screen after \n or \r .

- I dislike font 1
- Font 2 is easier to read
- Font 3 is denser (you need to adjust line spacing in oled_output.o)
- Can not change font tupe during run ; can not mix font types with this wrapper.
- 7 lines max with font 3, 6 lines recommended.

This wrapper only supports landscape; no portrait.

########################################################## Examples

Show hostname, date, uptime, and refresh:
while true ; do hostname ; /bin/date "+%Y-%m-%d %H-%M-%S" ; uptime ; echo -en "\f" ; sleep 1 ; done | ./oled_output /dev/i2c-0 2

Show the same thing, but refresh only the date, while reparsing the whole screen: we jump over line 1 without altering it:
{ hostname ; echo ; uptime ; echo -en "\f" ; while true ; do echo ; /bin/date "+%Y-%m-%d %H-%M-%S" ; echo -en "\f" ; sleep 1 ; done ; } | ./oled_output /dev/i2c-0 2


Show the same thing, but refresh only the date, while reparsing only the second line: we only work on line 2
{ hostname ; echo ; uptime ; echo -en "\f\n" ; while true ; do echo -ne "$(/bin/date "+%Y-%m-%d %H-%M-%S")\r" ; sleep 1 ; done ; } | ./oled_output /dev/i2c-0 2

This way, several functions in a script can refresh a specific line without altering other lines, without having to maintain a buffer within the script. This could be usefull for example if you are designing a weather monitor with will update temperature every minute, but hygrometry every 5mn. As long as outputs are not sent at the same time (aka not in parallel process-functions), each function can update any line without having to buffer the other ones.

########################################################## Requests

I would be pleased to add larger fonts, if any one can point me compatible fonts; I would like to be able to use it as 5, 4, and 3 lines.

Also portrait would be nice, maybe later.

I don't thing passing line spacing, and address as argument would worth the effort. It's very easy to edit oled_output.c and produce one binary per address required.



