#include <lcom/lcf.h>
#include <math.h>
#include <devices/graphicscard.h>

static char *video_mem; /* Address to which VRAM is mapped */
static char *video_buffer;

uint16_t hres; /* Frame horizontal resolution */
uint16_t vres; /* Frame vertical resolution */
static uint8_t bits_per_pixel;

int r;
struct minix_mem_range mr; /* physical memory range */
static unsigned int vram_base; /* VRAM’s physical addresss */
static unsigned int vram_size; /* VRAM’s size, but you can use the frame-buffer size, instead */

vbe_mode_info_t vmi_p;

int (vbe_get_mode_information)(uint16_t mode, vbe_mode_info_t *v){

  mmap_t mem_map;
  lm_alloc(sizeof(vbe_mode_info_t), &mem_map); //alocar memoria

  struct reg86 r86;
  memset(&r86, 0, sizeof(r86));    

  r86.ax = 0x4F01;
  r86.cx = mode;
  r86.es = PB2BASE(mem_map.phys);//endereço base
  r86.di = PB2OFF(mem_map.phys);// offset
  r86.intno = 0x10; 


  if(sys_int86(&r86) != OK ) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }

    *v = *(vbe_mode_info_t*)mem_map.virt; 


  lm_free(&mem_map);

  return 0;

}

void * (vg_init)(uint16_t mode){

  vbe_get_mode_information(mode, &vmi_p);

  hres = vmi_p.XResolution;
  vres = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;

  vram_base = vmi_p.PhysBasePtr;
  vram_size = hres * vres * (ceil(bits_per_pixel/8));

  mr.mr_base = (phys_bytes)vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK){
    panic("sys_privcctl(ADD_MEM) failed \n");
    return NULL;
  }

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED){
    panic("Couldn't map video memory\n");
    return NULL;
  }

  video_buffer = (char *) malloc(vram_size);

  struct reg86 r86;

  memset(&r86, 0, sizeof(r86));

  r86.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
  r86.bx = 1<<14 | mode; // set bit 14: linear framebuffer
  r86.intno = 0x10;

  if( sys_int86(&r86) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return NULL;
  }

  return (void *)video_mem;
}

int (color_pixel)(uint16_t x, uint16_t y, uint32_t color) {

  if(x > hres || y > vres){
    return 0;
  }

  if (bits_per_pixel == 8){
    memset(video_buffer + hres * y + x, color, 1);
  }
  else if (bits_per_pixel == 15) {
    memcpy(video_buffer + hres * y * 2 + x * 2, &color, 2);
  }
  else {
    memcpy(video_buffer + hres * y * (bits_per_pixel/8) + x * (bits_per_pixel/8), &color, (bits_per_pixel/8));
  }

  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){

  for(int i = 0; i < len; i++){
    if (color_pixel( (i + x), y, color) != 0){
      printf("Error in vg_color_pixel\n");
      return 1; 
    }
  }
  
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for(int i = 0; i < height; i++){
    if (vg_draw_hline(x, y + i, width, color) != 0){
      printf("Error in vg_draw_line()\n");
      return 1;
    }
  }

  return 0;
}

int (vg_draw_xpm)(uint16_t x, uint16_t y, xpm_image_t img, uint8_t *map){
  
  uint32_t * sprite = (uint32_t *) img.bytes;
  for (unsigned int i = 0; i < img.height; i++) {
      for (unsigned int j = 0; j < img.width; j++) {
          if(xpm_transparency_color(XPM_8_8_8_8) == *(sprite + i * img.width + j)) continue;
          color_pixel(j + x, i + y, *(sprite + i * img.width + j));
      }
  }
  return 0;
}

void (vg_clear_screen)(){

  if (bits_per_pixel == 15) {
    memset(video_buffer, 0, hres * vres * 2);
  } else {
    memset(video_buffer, 0, hres * vres * (bits_per_pixel / 8));
  }

}

void (move_buffer)(){

  if (bits_per_pixel == 15) {
    memcpy(video_mem, video_buffer, vram_size );
    memset(video_buffer,0,vram_size);
  }
  else {
    memcpy(video_mem, video_buffer, vram_size );
    memset(video_buffer,0,vram_size);
  }
}
