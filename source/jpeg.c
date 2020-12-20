/*
 * Copyright 2017-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "jpeglib.h"
#include "display_support.h"
#include "board.h"
#include "sdmmc_config.h"
#include "pin_mux.h"
#include "fsl_soc_src.h"
#include "clock_config.h"


/* Camera definition. */
#define CAMERA_WIDTH        1280
#define CAMERA_HEIGHT       720

/* Image definition. */
#define IMAGE_WIDTH         CAMERA_WIDTH
#define IMAGE_HEIGHT        CAMERA_HEIGHT
#define COMPRESS_LINES      1u

   
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t JpegScanlines[COMPRESS_LINES][IMAGE_WIDTH * 3u], FRAME_BUFFER_ALIGN);


void rgb565_2_rgb888(uint8_t *rgb565, uint8_t *rgb888, int pixels);
status_t JpegCompress(uint32_t buffer[]);



status_t JpegCompress(uint32_t buffer[])
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    uint32_t (*scanlines)[IMAGE_WIDTH] = (uint32_t (*)[IMAGE_WIDTH])buffer;
    JSAMPROW row_pointer[1];

    FILE fJpeg;
    char JpegFilename[20];
    static uint32_t JpegFilenameIndex = 0u;

    PRINTF("Start of JPEG Compression... ");


    if(JpegFilenameIndex > 9999u)
   {
       JpegFilenameIndex = 0u;
   }

   sprintf(JpegFilename, "/IMG_%04d.jpg", ++JpegFilenameIndex);

   /* Open the output file. */
   if(f_open(&fJpeg, _T(JpegFilename), (FA_WRITE | FA_CREATE_ALWAYS)))
   {
        PRINTF("ERROR: can't open %s\n", JpegFilename);
        return kStatus_Fail;
   }

   /* Step 1: Initialize the JPEG compression object with default error handling. */
    cinfo.err = jpeg_std_error(&jerr);
   /*  Now we can initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);
    /* Specify data destination for compression */
    jpeg_stdio_dest(&cinfo, &fJpeg);


    /* Initialize JPEG parameters.*/
    cinfo.in_color_space = JCS_RGB; /* arbitrary guess */
    jpeg_set_defaults(&cinfo);

    /* Specify the source format. */
    cinfo.image_width = (JDIMENSION)IMAGE_WIDTH;
    cinfo.image_height = (JDIMENSION)IMAGE_HEIGHT;
    cinfo.input_components = 4u;            /*  # of color components per pixel */
    cinfo.in_color_space = JCS_RGB;         /*  colorspace of input image */

    /* Start compressor */
    jpeg_start_compress(&cinfo, TRUE);
 
    /* Process data */
   while (cinfo.next_scanline < cinfo.image_height)
   {
     /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
	row_pointer[0] = (JSAMPROW)&buffer[cinfo.next_scanline * cinfo.image_width * cinfo.input_components];
	jpeg_write_scanlines(&cinfo, row_pointer, 1);
   }

    /* Finish compression and release memory */
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    f_close(&fJpeg);

    PRINTF("Done: %s is saved. \r\n", JpegFilename);

    return kStatus_Success;
}


void rgb565_2_rgb888(uint8_t *rgb565, uint8_t *rgb888, int pixels)
{
    /* RGB565 in little-endian: "G2G1G0B4B3B2B1B0 R4R3R2R1R0G5G4G3 */
    for(uint32_t i=0; i<pixels; i++)
    {
        rgb888[3*i] = ((rgb565[2*i] & 0x1F)<<3) | (rgb565[2*i] & 0x07);/*B4B3B2B1B0B2B1B0*/
        rgb888[3*i+1] = ((rgb565[2*i+1] & 0x07)<<5) | ((rgb565[2*i] & 0xE0)>>3) | ((rgb565[2*i] & 0x60)>>5);/*G5G4G3G2G1G0G1G0*/
        rgb888[3*i+2] = (rgb565[2*i+1] & 0xF8) | ((rgb565[2*i+1] & 0x38)>>3); /*R4R3R2R1R0R2R1R0*/
    }
}

