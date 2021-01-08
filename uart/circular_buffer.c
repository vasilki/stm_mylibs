#include <string.h>

#define K_CIRCULAR_BUFF_LENGTH 5
#define K_SEGMENT_BUFF_SIZE 200

typedef struct
{
  uint8_t buffer[K_CIRCULAR_BUFF_LENGTH*K_SEGMENT_BUFF_SIZE];
  uint32_t data_size[K_CIRCULAR_BUFF_LENGTH];
  uint8_t *offset;
  uint32_t segment_size;
  uint32_t circular_length;
  uint32_t get_index;
}T_CIRCULAR_BUFFER;

static T_CIRCULAR_BUFFER GL_BUFFER;

void circular_buffer_init()
{
  GL_BUFFER.segment_size = K_SEGMENT_BUFF_SIZE;
  GL_BUFFER.circular_length = K_CIRCULAR_BUFF_LENGTH;
  memset(GL_BUFFER.buffer, 0, sizeof(GL_BUFFER.buffer));
  memset(GL_BUFFER.data_size, 0, sizeof(GL_BUFFER.data_size));
  GL_BUFFER.offset = &GL_BUFFER.buffer[0];
  GL_BUFFER.get_index = 0;

  return;
}

void circular_buffer_add(const uint8_t *par_buff, uint32_t par_size)
{
  uint32_t loc_size = 0;

  if((par_buff != 0) && (par_size > 0))
  {
    if(par_size > GL_BUFFER.segment_size)
    {
      loc_size = GL_BUFFER.segment_size;
    }
    else
    {
      loc_size = par_size;
    }

    memcpy(GL_BUFFER.offset,par_buff,loc_size);
    GL_BUFFER.offset += GL_BUFFER.segment_size;
    *GL_BUFFER.data_size = loc_size;
    GL_BUFFER.data_size++;


    if(GL_BUFFER.offset > &GL_BUFFER.buffer[sizeof(GL_BUFFER.buffer)])
    {
      GL_BUFFER.offset = &GL_BUFFER.buffer[0];
      GL_BUFFER.data_size = &GL_BUFFER.data_size[0];
    }
    else
    {
      /*nothing to do*/
    }
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
    if(GL_BUFFER.data_size[GL_BUFFER.get_index] != 0)
    {
      memcpy(par_buff, &GL_BUFFER.buffer[GL_BUFFER.get_index * GL_BUFFER.segment_size],GL_BUFFER.data_size[GL_BUFFER.get_index]);
      *par_size = GL_BUFFER.data_size[GL_BUFFER.get_index];
      GL_BUFFER.data_size[GL_BUFFER.get_index] = 0;
      GL_BUFFER.get_index = (GL_BUFFER.get_index + 1) % GL_BUFFER.circular_length;
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


