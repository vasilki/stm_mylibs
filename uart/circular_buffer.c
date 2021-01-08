#include "stm32f4xx_hal.h"
#include <string.h>


#define K_CIRCULAR_BUFF_LENGTH 5
#define K_SEGMENT_BUFF_SIZE 200

typedef struct
{
  uint8_t data[K_SEGMENT_BUFF_SIZE];
  uint32_t data_size;
}T_BUFFER_DATA;

typedef struct
{
  T_BUFFER_DATA buffer[K_CIRCULAR_BUFF_LENGTH];
  uint32_t offset;
  uint32_t next_to_read_index;
}T_CIRCULAR_BUFFER;

static T_CIRCULAR_BUFFER GL_BUFFER;


void circular_buffer_init()
{
  memset(GL_BUFFER.buffer, 0, sizeof(GL_BUFFER.buffer));
  GL_BUFFER.offset = 0;
  GL_BUFFER.next_to_read_index = 0;

  return;
}

void circular_buffer_add(const uint8_t *par_buff, uint32_t par_size)
{
  uint32_t loc_size = 0;

  if((par_buff != 0) && (par_size > 0))
  {
    if(par_size > K_SEGMENT_BUFF_SIZE)
    {
      loc_size = K_SEGMENT_BUFF_SIZE;
    }
    else
    {
      loc_size = par_size;
    }

    memcpy(GL_BUFFER.buffer[GL_BUFFER.offset].data,par_buff,loc_size);
    GL_BUFFER.buffer[GL_BUFFER.offset].data_size = loc_size;

    GL_BUFFER.offset = (GL_BUFFER.offset + 1) % K_CIRCULAR_BUFF_LENGTH;
    GL_BUFFER.buffer[GL_BUFFER.offset].data_size = 0;
  }
  else
  {
    /*nothing to do*/
  }

  return;
}

void circular_buffer_get(uint8_t *par_buff, uint32_t *par_size)
{
  if((par_buff != 0) && (par_size != 0))
  {
    if(GL_BUFFER.buffer[GL_BUFFER.next_to_read_index].data_size != 0)
    {
      memcpy(par_buff, GL_BUFFER.buffer[GL_BUFFER.next_to_read_index].data,GL_BUFFER.buffer[GL_BUFFER.next_to_read_index].data_size);
      *par_size = GL_BUFFER.buffer[GL_BUFFER.next_to_read_index].data_size;
      GL_BUFFER.buffer[GL_BUFFER.next_to_read_index].data_size = 0;
      GL_BUFFER.next_to_read_index = (GL_BUFFER.next_to_read_index + 1) % K_CIRCULAR_BUFF_LENGTH;
    }
    else
    {
      *par_size = 0;
    }
  }
  else
  {
    /*nothing to do*/
  }

  return;
}


