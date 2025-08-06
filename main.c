#include <stdio.h>


#include <stdint.h>

uint8_t cmd_init[6] = { 0xE0, 0x01, 0x02, 0x03, 0x04, 0x0A };

uint8_t cmd_h264[6] = { 0xE1, 0x01, 0x02, 0x03, 0x04, 0x0A  };
uint8_t cmd_aac[6] =  { 0xE1, 0x01, 0x02, 0x03, 0x04, 0x0A  };

#define CMD_INIT   		0xE0
#define CMD_AAC    		0xE1 
#define CMD_H264   		0xE2 

#define CMD_END_BYTE    0x0A 

void make_cmd()
{

}

void main()
{
	uint8_t cmd_i[6], cmd_h[6], cmd_a[6];
	uint32_t time_now = 294967295 ;
	uint32_t h264_len =  355335  ;
	uint32_t aac_len =  29934885  ;

    printf("pre time_now 0x%08X  0x%02X and %u \n", time_now, (uint8_t)((time_now >> 24)&0x000000ff), time_now);

	cmd_i[0]= CMD_INIT ;
	cmd_i[1]= (time_now >> 24);
	cmd_i[2]= (time_now >> 16); 
	cmd_i[3]= (time_now >> 8); 
	cmd_i[4]= (time_now);
	cmd_i[5]= CMD_END_BYTE ;

	printf("cmd_i : ");
	for(int i=0; i<6; i++)
	{
		printf("0x%02X ", cmd_i[i]);
	}
	printf(". \n");

	uint32_t dec_cmd_i = 0 ;
	dec_cmd_i |= (cmd_i[1]<<24 & 0xff000000);
	dec_cmd_i |= (cmd_i[2]<<16 & 0x00ff0000);
	dec_cmd_i |= (cmd_i[3]<<8 & 0x0000ff00);
	dec_cmd_i |= (cmd_i[4]    & 0x000000ff);

	printf("cmd i is 0x%08X  and  %d \n", dec_cmd_i, dec_cmd_i);
}
