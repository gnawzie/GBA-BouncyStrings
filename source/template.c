
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <gba_dma.h>
#include <stdio.h>
#include <stdlib.h>

//------------------------------------
//Particle structure contains the x, y, and color of the particle
struct Point { unsigned short x; unsigned short y; signed short xdir; signed short ydir; signed short color; };
struct Point particle[1000];
short ticker = 0;
short time = 0;
//----------------------------------
void updateParticles(void){
	int i;
	int nx,ny;
	struct Point *p;
	for(i=0;i<1000;i++){
		p = &particle[i];
		nx = p->x + p->xdir;
		ny = p->y + p->ydir;
		if(nx >= 240 || nx <=0)
			p->xdir = -p->xdir;
			p->color += 0x0101;
		
		if(ny >=160 || ny <= 0)
			p->ydir = -p->ydir;
			p->color += 0x0101;
		p->x += p->xdir;
		p->y += p->ydir;
		
	}
}
void drawParticles(void){
	int i;
	short BLACK = 0;
	CpuFastSet(&BLACK, (unsigned short *)VRAM,DMA_SRC_FIXED|19200);
	for(i=0;i<1000;i++){
		((unsigned short*)VRAM)[particle[i].x+particle[i].y*240] = particle[i].color;
	}
}
//---------------------------------------------------------------------
//Write to screen
//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void) {
	int i;
	for (i=0;i<200;i++){
		particle[i].xdir = 1; particle[i].x = i; particle[i].ydir = 1; particle[i].y = i/2; particle[i].color = 0xF000 + i;
	}
	for (i=200;i<400;i++){
		particle[i].xdir = -1; particle[i].x = i/3; particle[i].ydir = -1; particle[i].y = i/3; particle[i].color = 0xF000 + i;
	}
	for (i=400;i<800;i++){
		particle[i].xdir = 1; particle[i].x = i/6; particle[i].ydir = 1; particle[i].y = i/6; particle[i].color = 0xF000 + i;
	}
	for (i=800;i<1000;i++){
		particle[i].xdir = 1; particle[i].x = i/10; particle[i].ydir = 1; particle[i].y = i/10; particle[i].color = 0xF000 + i;
	}
	irqInit();
	irqEnable(IRQ_VBLANK);
	REG_DISPCNT = MODE_3 | BG2_ENABLE;
	while(1){
	    updateParticles();
		VBlankIntrWait();
		drawParticles();
		
	}
}
