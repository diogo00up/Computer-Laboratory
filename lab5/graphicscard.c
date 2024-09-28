#include <lcom/lcf.h>
#include "adresses.h"
#include "graphicscard.h"
#include <stdint.h>
#include <math.h>

#include <machine/int86.h> // /usr/src/include/arch/i386


vbe_mode_info_t vmi_p;
char *video_mem; /* frame-buffer VM address */
char *buffer;


void * (vg_init)(uint16_t mode){

  /////////////////////////////////////////////////////////////////////////
  //Faco uma call as BIOS usando o VBE para dizer que quero que o minnix use um mode grafico 

  reg86_t r;
  memset(&r, 0, sizeof(r));
  r.ax = 0x4F02;      // VBE call, function 02 -- set VBE mode
  r.bx = 1<<14|mode;  // set bit 14: linear framebuffer // sets the mode i want to use (resolution)
  r.intno = 0x10;
  if( sys_int86(&r) != OK ) {  //chamada a BIOS
    printf("set_vbe_mode: sys_int86() failed \n");
    return NULL;
  }
  
  /////////////////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////////////
  ///faz uma cal a bios de maneira a dar-nos informacao sobre o mode em que estamos neste momento colocando em vmi_p essa informacao
  vbe_get_mode_info(mode,&vmi_p);
 
  /////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////////
  //////////////Mapear memoria fisica para memoria virtual
  struct minix_mem_range mr; // physical memory range 
  unsigned int vram_base=vmi_p.PhysBasePtr; //VRAM’s physical addresss 
  unsigned int vram_size=vmi_p.XResolution*vmi_p.YResolution*(ceil(vmi_p.BitsPerPixel/8)); 
  //VRAM’s size, but you can use the frame-buffer size, instead 
  /////////////////Allocating space for my buffer 
  buffer = (char *) malloc(vram_size);
  
  // Allow memory mapping 
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)){
    panic("sys_privctl (ADD_MEM) failed");
  }

  // Map memory 
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED){
    panic("couldn’t map video memory");
  }
  return video_mem;

}

void (from_buffer)(){
  memcpy(video_mem, buffer, vmi_p.XResolution * vmi_p.YResolution * (vmi_p.BitsPerPixel/8));
  
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){

  buffer[vmi_p.XResolution * y * (vmi_p.BitsPerPixel/8) + x * (vmi_p.BitsPerPixel/8)]= color;

  //memcpy(buffer + vmi_p.XResolution * y * (vmi_p.BitsPerPixel/8) + x * (vmi_p.BitsPerPixel/8), &  //color, ceil(vmi_p.BitsPerPixel/8));  
  
  return 0;

}

int (vg_draw_hline)	(	uint16_t 	x, uint16_t 	y, uint16_t 	len, uint32_t 	color){
  for(int i = 0; i < len; i++){
    vg_draw_pixel(x + i,y,color);
  }
  return 0;
}	



int (vg_draw_rectangle)	(	uint16_t 	x, uint16_t 	y, uint16_t 	width, uint16_t 	height,uint32_t color ){

  for(int i = 0; i < height; i++){
    vg_draw_hline(x,y + i,width, color);
  } 
  
  return 0;
}

int(vg_draw_sprite)(uint8_t *sprite, xpm_image_t img, uint16_t x, uint16_t y)
{
    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            vg_draw_pixel(j + x, i + y, *(sprite + i * img.width + j));
        }
    }
    return 0;
}

