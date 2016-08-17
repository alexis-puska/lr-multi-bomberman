/*
 * (C) alexis-puska, 2016
 *
 * This work is licensed under the terms of the GNU GPLv2 or later.
 * See the COPYING file in the top-level directory.
 */

#define _GNU_SOURCE 1 // strcasestr
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


#include "libretro.h"
#include "../include/libretro_common.h"
#include "../include/pad.h"


#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
#ifdef _WIN32
	#include <windows.h>
#endif
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#define VOUT_MAX_WIDTH 1920
#define VOUT_MAX_HEIGHT 1080
#define VOUT_WIDTH 1920
#define VOUT_HEIGHT 1080


static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_environment_t environ_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static struct retro_rumble_interface rumble;
static bool support_no_game = true;
static struct retro_hw_render_callback hw_render;

int in_type[16] =  { 
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD
};
unsigned short in_keystate[16];








//OpenGL
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
static struct retro_hw_render_callback hw_render;

#if defined(HAVE_PSGL)
#define RARCH_GL_FRAMEBUFFER GL_FRAMEBUFFER_OES
#define RARCH_GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_OES
#define RARCH_GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_EXT
#elif defined(OSX_PPC)
#define RARCH_GL_FRAMEBUFFER GL_FRAMEBUFFER_EXT
#define RARCH_GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_EXT
#define RARCH_GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_EXT
#else
#define RARCH_GL_FRAMEBUFFER GL_FRAMEBUFFER
#define RARCH_GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE
#define RARCH_GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0
#endif



static unsigned width  = 1920;
static unsigned height = 1080;
static unsigned frame_count;

static GLuint prog;
static GLuint vbo;
static const char *vertex_shader[] = {
   "uniform mat4 uMVP;",
   "attribute vec2 aVertex;",
   "attribute vec4 aColor;",
   "varying vec4 color;",
   "void main() {",
   "  gl_Position = uMVP * vec4(aVertex, 0.0, 1.0);",
   "  color = aColor;",
   "}",
};

static const char *fragment_shader[] = {
   "#ifdef GL_ES\n",
   "precision mediump float;\n",
   "#endif\n",
   "varying vec4 color;",
   "void main() {",
   "  gl_FragColor = color;",
   "}",
};

static void compile_program(void)
{
   prog = glCreateProgram();
   GLuint vert = glCreateShader(GL_VERTEX_SHADER);
   GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

   glShaderSource(vert, ARRAY_SIZE(vertex_shader), vertex_shader, 0);
   glShaderSource(frag, ARRAY_SIZE(fragment_shader), fragment_shader, 0);
   glCompileShader(vert);
   glCompileShader(frag);

   glAttachShader(prog, vert);
   glAttachShader(prog, frag);
   glLinkProgram(prog);
   glDeleteShader(vert);
   glDeleteShader(frag);
}


static void setup_vao(void)
{
   static const GLfloat vertex_data[] = {
      -0.5, -0.5,
      0.5, -0.5,
      -0.5,  0.5,
      0.5,  0.5,
      1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 0.0, 1.0,
      0.0, 1.0, 1.0, 1.0,
      1.0, 0.0, 1.0, 1.0,
   };
   
   glUseProgram(prog);

   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glUseProgram(0);
}




static void context_reset(void)
{
   fprintf(stderr, "Context reset!\n");
   compile_program();
   setup_vao();
}

static void context_destroy(void)
{
   fprintf(stderr, "Context destroy!\n");
   glDeleteBuffers(1, &vbo);
   vbo = 0;
   glDeleteProgram(prog);
   prog = 0;
}






/* libretro */
void retro_set_environment(retro_environment_t cb){
	environ_cb = cb;
}

void retro_set_audio_sample(retro_audio_sample_t cb){
	audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb){
	audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb){
	input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb){
	input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb){
	fprintf(stderr, "refresh!\n");
	video_cb = cb;
}

unsigned retro_api_version(void){
	return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device){
}

void retro_get_system_info(struct retro_system_info *info){
	memset(info, 0, sizeof(*info));
	info->library_name = "Lr-Multi-Bomberman";
	info->library_version = "r1";
	info->valid_extensions = NULL;
	info->need_fullpath = false;
}

void retro_get_system_av_info(struct retro_system_av_info *info){
	memset(info, 0, sizeof(*info));
	info->timing.fps            = 60;
	info->timing.sample_rate    = 44100;
	info->geometry.base_width   = VOUT_WIDTH;
	info->geometry.base_height  = VOUT_HEIGHT;
	info->geometry.max_width    = VOUT_MAX_WIDTH;
	info->geometry.max_height   = VOUT_MAX_HEIGHT;
	info->geometry.aspect_ratio = 16.0 / 9.0;
}

/* savestates */
size_t retro_serialize_size(void){
	// it's currently 4380651-4397047 bytes,
	// but have some reserved for future
	return 0x440000;
}

bool retro_serialize(void *data, size_t size){
	return false;
}

bool retro_unserialize(const void *data, size_t size){
	return false;
}

/* cheats */
void retro_cheat_reset(void){
}

void retro_cheat_set(unsigned index, bool enabled, const char *code){
}


static bool retro_init_hw_context(void)
{
    fprintf(stderr, "Context initialisation\n");
#ifdef GLES
    hw_render.context_type = RETRO_HW_CONTEXT_OPENGLES2;
    fprintf(stderr, "RETRO_HW_CONTEXT_OPENGLES2!\n");
#else
    hw_render.context_type = RETRO_HW_CONTEXT_OPENGL;
    fprintf(stderr, "RETRO_HW_CONTEXT_OPENGL!\n");
#endif
    hw_render.context_reset = context_reset;
    hw_render.context_destroy = context_destroy;
    hw_render.depth = true;
    hw_render.stencil = true;
    hw_render.bottom_left_origin = true;
    if (!environ_cb(RETRO_ENVIRONMENT_SET_HW_RENDER, &hw_render)){
        fprintf(stderr, "false!\n");
        return false;
    }
    return true;
    fprintf(stderr, "true!\n");
}

bool retro_load_game(const struct retro_game_info *info){
	struct retro_input_descriptor desc[] = {
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "Cross" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "Circle" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Triangle" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Square" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L1" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "L2" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R1" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "R2" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,"Select" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
		
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "Cross" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "Circle" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Triangle" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Square" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L1" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "L2" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R1" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "R2" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,"Select" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
		
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "Cross" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "Circle" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Triangle" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Square" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L1" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "L2" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R1" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "R2" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,"Select" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
		
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "Cross" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "Circle" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Triangle" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Square" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L1" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "L2" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R1" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "R2" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,"Select" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
		
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "Cross" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "Circle" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Triangle" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Square" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L1" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "L2" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R1" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "R2" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,"Select" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
		
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "Cross" },
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "Circle" },
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Triangle" },
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Square" },
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L1" },
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "L2" },
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R1" },
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "R2" },
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,"Select" },
		{ 5, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
		
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "Cross" },
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "Circle" },
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Triangle" },
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Square" },
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L1" },
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "L2" },
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R1" },
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "R2" },
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,"Select" },
		{ 6, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
		
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "Cross" },
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "Circle" },
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Triangle" },
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Square" },
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L1" },
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "L2" },
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R1" },
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "R2" },
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,"Select" },
		{ 7, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
		{ 0 },
	};
	
	environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);
	
	enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
	if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt)){
		fprintf(stderr, "XRGB8888 is not supported.\n");
		return false;
	}
    
    if (!retro_init_hw_context())
    {
        fprintf(stderr, "HW Context could not be initialized, exiting...\n");
        return false;
    }
	
	return true;
}







static const unsigned short retro_psx_map[] = {
	[RETRO_DEVICE_ID_JOYPAD_B]	= 1 << DKEY_CROSS,
	[RETRO_DEVICE_ID_JOYPAD_Y]	= 1 << DKEY_SQUARE,
	[RETRO_DEVICE_ID_JOYPAD_SELECT]	= 1 << DKEY_SELECT,
	[RETRO_DEVICE_ID_JOYPAD_START]	= 1 << DKEY_START,
	[RETRO_DEVICE_ID_JOYPAD_UP]	= 1 << DKEY_UP,
	[RETRO_DEVICE_ID_JOYPAD_DOWN]	= 1 << DKEY_DOWN,
	[RETRO_DEVICE_ID_JOYPAD_LEFT]	= 1 << DKEY_LEFT,
	[RETRO_DEVICE_ID_JOYPAD_RIGHT]	= 1 << DKEY_RIGHT,
	[RETRO_DEVICE_ID_JOYPAD_A]	= 1 << DKEY_CIRCLE,
	[RETRO_DEVICE_ID_JOYPAD_X]	= 1 << DKEY_TRIANGLE,
	[RETRO_DEVICE_ID_JOYPAD_L]	= 1 << DKEY_L1,
	[RETRO_DEVICE_ID_JOYPAD_R]	= 1 << DKEY_R1,
	[RETRO_DEVICE_ID_JOYPAD_L2]	= 1 << DKEY_L2,
	[RETRO_DEVICE_ID_JOYPAD_R2]	= 1 << DKEY_R2,
};
#define RETRO_PSX_MAP_LEN (sizeof(retro_psx_map) / sizeof(retro_psx_map[0]))

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info){
	return false;
}

void retro_unload_game(void){
}

unsigned retro_get_region(void){
	return RETRO_REGION_PAL;
}

void *retro_get_memory_data(unsigned id){
	return NULL;
}

size_t retro_get_memory_size(unsigned id){
	return 0;
}

void retro_reset(void){

}




void retro_run(void){
	// reset all keystate, query libretro for keystate
	int i;
	int j;
	for(i = 0; i < 8; i++) {
		in_keystate[i] = 0;
		// query libretro for keystate
		for (j = 0; j < RETRO_PSX_MAP_LEN; j++){
			if (input_state_cb(i, RETRO_DEVICE_JOYPAD, 0, j)){
				in_keystate[i] |= retro_psx_map[j];
			}
		}
	}

	
    char *image_path = "./resource/test2.png";
    SDL_Surface *image = IMG_Load( image_path );
    
    if(image != NULL){
    	fprintf(stderr, "image ->w : %i",image->w);
    }
	
	
    
    
   frame_count++;
   glBindFramebuffer(RARCH_GL_FRAMEBUFFER, hw_render.get_current_framebuffer());

   glClearColor(0.3, 0.4, 0.5, 1.0);
   glViewport(0, 0, width, height);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);





   glUseProgram(prog);

   glEnable(GL_DEPTH_TEST);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   int vloc = glGetAttribLocation(prog, "aVertex");
   glVertexAttribPointer(vloc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
   glEnableVertexAttribArray(vloc);
   int cloc = glGetAttribLocation(prog, "aColor");
   glVertexAttribPointer(cloc, 4, GL_FLOAT, GL_FALSE, 0, (void*)(8 * sizeof(GLfloat)));
   glEnableVertexAttribArray(cloc);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   int loc = glGetUniformLocation(prog, "uMVP");

   float angle = frame_count / 100.0;
   float cos_angle = cos(angle);
   float sin_angle = sin(angle);

   const GLfloat mvp[] = {
      cos_angle, -sin_angle, 0, 0,
      sin_angle, cos_angle, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1,
   };
   glUniformMatrix4fv(loc, 1, GL_FALSE, mvp);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

   cos_angle *= 0.9;
   sin_angle *= 0.9;
   const GLfloat mvp2[] = {
      cos_angle, -sin_angle, 0, 0.0,
      sin_angle, cos_angle, 0, 0.0,
      0, 0, 1, 0,
      0.4, 0.4, 0.2, 1,
   };

		
 
    

   glUniformMatrix4fv(loc, 1, GL_FALSE, mvp2);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   glDisableVertexAttribArray(vloc);
   glDisableVertexAttribArray(cloc);
	glUseProgram(0); 

    

	
    
    video_cb(RETRO_HW_FRAME_BUFFER_VALID, VOUT_WIDTH, VOUT_HEIGHT, 0);
	fprintf(stderr, "retro_run!\n");
}



void retro_init(void){
	video_out_buf = malloc(VOUT_MAX_WIDTH * VOUT_MAX_HEIGHT);
	environ_cb(RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE, &rumble);
	unsigned level = 6;
	environ_cb(RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL, &level);
    environ_cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &support_no_game);
    fprintf(stderr, "Loaded game!\n");
}

void retro_deinit(void){
	
}









