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
#define ImgBuffSize         3*CAMERA_HEIGHT*CAMERA_WIDTH


   
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t ImageRGBBuf[ImgBuffSize], FRAME_BUFFER_ALIGN);




status_t JpegCompress(uint8_t *buffer);







/*******************************************************************************
* Function Name  : JpegCompress
* Description    : this function compresses raw image data
* Return         : status_t value (0 or 1)
*******************************************************************************/
status_t JpegCompress(uint8_t *buffer)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];

    FILE fJpeg;
    char JpegFilename[20];
    static uint32_t JpegFilenameIndex = 0u;
    int counter=0;
    int s=0;

    PRINTF("Start of JPEG Compression... ");

    //convert to RGB888
    for(int m=0; m<CAMERA_HEIGHT;m++)
    {
       for(int n=0; n<4*CAMERA_WIDTH;n++)
       {
         ImageRGBBuf[s] = *buffer;
         buffer++;
         s++;
         counter++;
            if(counter == 3)
        	 {
        		n++;
        		buffer++;
        		counter = 0;
        	}
        }
     }


    if(JpegFilenameIndex > 9999u)
   {
       JpegFilenameIndex = 0u;
   }

   sprintf(JpegFilename, "/img_1/jpg_5.dat", ++JpegFilenameIndex);

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
    cinfo.input_components = 3u;            /*  # of color components per pixel */
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
	row_pointer[0] = (JSAMPROW)&ImageRGBBuf[cinfo.next_scanline * cinfo.image_width * cinfo.input_components];
	jpeg_write_scanlines(&cinfo, row_pointer, 1);
   }

    /* Finish compression and release memory */
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    f_close(&fJpeg);

    PRINTF("Done: %s is saved. \r\n", JpegFilename);

    return kStatus_Success;
}

