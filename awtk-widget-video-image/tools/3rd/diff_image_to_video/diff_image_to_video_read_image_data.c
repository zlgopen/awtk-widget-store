#include <stdlib.h>
#include <string.h>

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "../lz4/lz4_assets.h"
#include "diff_image_to_video_types.h"
#include "diff_image_to_video_read_image_data.h"

typedef void *(*memcpy_fun_t)(void *dst, void *src, uint32_t size);

static memcpy_fun_t memcpy_fun = NULL;

static unsigned char *g_max_data_decompress = NULL;

static int diff_image_to_video_create_image_data_for_align(
    unsigned char *data_for_image_diff_data_list_lz4_dec,
    unsigned int image_data_length, unsigned char *image_data,
    unsigned int image_width, unsigned int channels) {

  int is_su = -1;
  unsigned int y = 0;
  unsigned int data_length = 0;
  unsigned int width_length = 0;

  image_diff_rect_t diff_rect;
  unsigned int image_data_list_point = 0;

  if (memcpy_fun == NULL) {
    return is_su;
  }

  do {
    unsigned int image_x_point = 0;
    unsigned int image_y_point = 0;
    unsigned int image_line_length = 0;
    unsigned int tmp_image_data_length = 0;

    memcpy(&diff_rect, data_for_image_diff_data_list_lz4_dec + image_data_list_point, sizeof(image_diff_rect_t));
    image_data_list_point += sizeof(image_diff_rect_t);

    data_length = diff_rect.width * diff_rect.height * channels;

    width_length = channels * image_width;
    image_x_point = diff_rect.x * channels;
    image_y_point = diff_rect.y * width_length;
    image_line_length = channels * diff_rect.width;

    for (y = 0; y < diff_rect.height; y++) {

      memcpy_fun(image_data + image_x_point + image_y_point,
                 data_for_image_diff_data_list_lz4_dec + image_data_list_point +
                     tmp_image_data_length,
                 diff_rect.width);

      image_y_point += width_length;
      tmp_image_data_length += image_line_length;
    }

    image_data_list_point += data_length;

    is_su = 0;
  } while (image_data_list_point < image_data_length);

  return is_su;
}

static int diff_image_to_video_create_image_data_for_noalign(
    unsigned char *data_for_image_diff_data_list_lz4_dec,
    unsigned int image_data_length, unsigned char *image_data,
    unsigned int image_width, unsigned int channels) {
  int is_su = -1;
  unsigned int y = 0;
  unsigned int data_length = 0;
  unsigned int width_length = 0;
  unsigned int data_width_length = 0;

  image_diff_rect_t diff_rect;
  unsigned int image_data_list_point = 0;

  do {
    unsigned int image_x_point = 0;
    unsigned int image_y_point = 0;
    unsigned int tmp_image_data_length = 0;

    memcpy(&diff_rect, data_for_image_diff_data_list_lz4_dec + image_data_list_point, sizeof(image_diff_rect_t));
    image_data_list_point += sizeof(image_diff_rect_t);

    data_length = diff_rect.width * diff_rect.height * channels;

    width_length = channels * image_width;
    image_x_point = diff_rect.x * channels;
    image_y_point = diff_rect.y * width_length;
    data_width_length = channels * diff_rect.width;

    for (y = 0; y < diff_rect.height; y++) {
      memcpy(image_data + image_x_point + image_y_point,
             data_for_image_diff_data_list_lz4_dec + image_data_list_point +
                 tmp_image_data_length,
             data_width_length);

      image_y_point += width_length;
      tmp_image_data_length += data_width_length;
    }

    image_data_list_point += data_length;

    is_su = 0;
  } while (image_data_list_point < image_data_length);
  return is_su;
}

static int diff_image_to_video_get_lz4_data_to_diff_image_data(
    unsigned char *lz4_data, unsigned int lz4_length,
    unsigned int image_data_length, unsigned char *image_data,
    unsigned int image_width, unsigned int channels) {

  int is_su = -1;

  if (g_max_data_decompress == NULL) {
    return is_su;
  }

  lz4_decompress_doubleBufferr_buffer(g_max_data_decompress,
                                      image_data_length, lz4_data, lz4_length);

  if (image_data_length > 0) {
    if (memcpy_fun != NULL) {
      is_su = diff_image_to_video_create_image_data_for_align(
          g_max_data_decompress, image_data_length, image_data,
          image_width, channels);
    } else {
      is_su = diff_image_to_video_create_image_data_for_noalign(
          g_max_data_decompress, image_data_length, image_data,
          image_width, channels);
    }
  }

  return is_su;
}

static int diff_image_to_video_read_frame_image(
    const unsigned char *file_data, unsigned int file_data_length,
    video_info_t *video_info, frame_info_t *frame_info,
    unsigned char **last_image_data, unsigned int *width, unsigned int *height,
    unsigned char *channels, unsigned int *delays) {
  int is_su = -1;
  unsigned char *data_com = NULL;

  if (file_data_length >= frame_info->data_length + frame_info->start_point) {
    if (*width != video_info->width || *height != video_info->height ||
        *channels != video_info->channels) {
      if (*last_image_data != NULL) {
        free(*last_image_data);
      }

      *last_image_data = (unsigned char *)malloc(
          sizeof(unsigned char) *
          (video_info->width * video_info->height * video_info->channels));
    }

    *width = video_info->width;
    *height = video_info->height;
    *delays = video_info->delays;
    *channels = video_info->channels;

    data_com = (unsigned char *)(file_data + frame_info->start_point);

    switch (frame_info->compress_type) {
    case compress_type_lz4:
      lz4_decompress_doubleBufferr_buffer(*last_image_data,
                                          *width * *height * *channels,
                                          data_com, frame_info->data_length);

      break;
    case compress_type_diff_lz4:
      is_su = diff_image_to_video_get_lz4_data_to_diff_image_data(
          data_com, frame_info->data_length, frame_info->data_decompress_length,
          *last_image_data, *width, video_info->channels);

      break;
    default:
      break;
    }

    is_su = 0;
  }

  return is_su;
}

static int diff_image_to_video_read_video_info_and_frame_info(
    const unsigned char *file_data, unsigned int file_data_length,
    video_info_t **video_info, frame_info_t **frame_info_list,
    unsigned int frame_number) {
  unsigned int file_point = 0;

  if (file_data == NULL || file_data_length < sizeof(video_info_t)) {
    return -1;
  }

  *video_info = (video_info_t *)file_data;
  file_point += sizeof(video_info_t);

  if (file_data_length < (sizeof(video_info_t) + ((*video_info)->frame_number *
                                                  sizeof(frame_info_t)))) {
    return -1;
  }
  file_point += (sizeof(frame_info_t) * frame_number);
  *frame_info_list = (frame_info_t *)(file_data + file_point);

  return 0;
}

int diff_image_to_video_read_image_data(
    const unsigned char *file_data, unsigned int file_data_length,
    unsigned int frame_number, unsigned char **last_image_data,
    unsigned int *width, unsigned int *height, unsigned char *channels,
    unsigned int *delays) {
  int is_su = -1;
  video_info_t *video_info;
  frame_info_t *frame_info_list;

  if (file_data == NULL || file_data_length < sizeof(video_info_t)) {
    return is_su;
  }

  if (diff_image_to_video_read_video_info_and_frame_info(
          file_data, file_data_length, &video_info, &frame_info_list,
          frame_number) == 0) {
    is_su = diff_image_to_video_read_frame_image(
        file_data, file_data_length, video_info, frame_info_list,
        last_image_data, width, height, channels, delays);
  }
  return is_su;
}

int diff_image_to_video_read_image_init(const unsigned char *file_data,
                                        unsigned int file_data_length) {
  int i = 0;
  video_info_t *video_info = NULL;
  unsigned int max_data_decompress_length = 0;
  unsigned int file_point = sizeof(video_info_t);

  if (file_data == NULL || file_data_length < sizeof(video_info_t)) {
    return -1;
  }
  video_info = (video_info_t *)file_data;
  if (video_info->channels == 2) {
    memcpy_fun = (memcpy_fun_t)tk_memcpy16;
  } else if (video_info->channels == 4) {
    memcpy_fun = (memcpy_fun_t)tk_memcpy32;
  } else {
    memcpy_fun = NULL;
  }

  if (g_max_data_decompress != NULL) {
    TKMEM_FREE(g_max_data_decompress);
  }

  if (file_data_length < (sizeof(video_info_t) +
                          (video_info->frame_number * sizeof(frame_info_t)))) {
    return -1;
  }
  file_point += sizeof(frame_info_t);

  for (i = 1; i < video_info->frame_number; i++) {
    frame_info_t *frame_info_list = (frame_info_t *)(file_data + file_point);
    if (frame_info_list->compress_type == compress_type_diff_lz4 &&
        frame_info_list->data_decompress_length > max_data_decompress_length) {

      max_data_decompress_length = frame_info_list->data_decompress_length;
    }
    file_point += sizeof(frame_info_t);
  }

  if (max_data_decompress_length > 0) {
    g_max_data_decompress =
        TKMEM_CALLOC(sizeof(unsigned char), max_data_decompress_length);
  }

  if (max_data_decompress_length > 0 &&
      g_max_data_decompress == NULL) {
    return -2;
  }

  return 0;
}

void diff_image_to_video_read_image_dispose() {
  if (g_max_data_decompress != NULL) {
    TKMEM_FREE(g_max_data_decompress);
    g_max_data_decompress = NULL;
  }
}
