/*

    Sega Saturn USB flash cart transfer utility
    Copyright © 2012, 2013 Anders Montonen
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <ftdi.h>

#include <sys/time.h>

#include "crc.h"

/* Optimal payload/usb transfer size, see FTDI appnote */
#define USB_READPACKET_SIZE (64*1024)
#define USB_WRITEPACKET_SIZE (4*1024)
#define USB_PAYLOAD(x) ((x)-(((x)/64)*2))
#define READ_PAYLOAD_SIZE (USB_PAYLOAD(USB_READPACKET_SIZE))
#define WRITE_PAYLOAD_SIZE (USB_PAYLOAD(USB_WRITEPACKET_SIZE))

static unsigned char SendBuf[2*WRITE_PAYLOAD_SIZE];
static unsigned char RecvBuf[2*READ_PAYLOAD_SIZE];
static struct ftdi_context Device = {0};

static void PrintUsage(const char *pProgname);
static int DoDownload(const char *pFilename, const unsigned int Address,
                       const unsigned int Size);
static int DoUpload(const char *pFilename, const unsigned int Address);
static int DoExecute(const char *pFilename, const unsigned int Address);
static int InitComms(const int VID, const int PID);
static void CloseComms(void);
static void ParseNumericArg(const char *pArg, unsigned int *pResult);
static void DoConsole(void);
static void Signal(int sig);

enum
{
    FUNC_DOWNLOAD = 1,
    FUNC_UPLOAD,
    FUNC_EXEC
};

int main(int argc, char *argv[])
{
    int             ii = 1;
    int             function = 0, error = 0, console = 0;
    unsigned int    address = 0, length = 0;
    char           *pFilename = NULL;
    int             VID = 0x0403, PID = 0x6001;
    char           *pVID = NULL, *pPID = NULL;

    if ((pVID = getenv("VID")))
        sscanf(pVID, "%x", &VID);

    if ((pPID = getenv("PID")))
        sscanf(pPID, "%x", &PID);

    while (ii < argc && !error)
    {
        if (!strcmp(argv[ii], "-v") || !strcmp(argv[ii], "-V"))
        {
            if (argc < ii + 1)
            {
                error = 1;
            }
            else
            {
                sscanf(argv[ii+1], "%x", &VID);
                ii += 2;
            }
        }
        else if (!strcmp(argv[ii], "-p") || !strcmp(argv[ii], "-P"))
        {
            if (argc < ii + 1)
            {
                error = 1;
            }
            else
            {
                sscanf(argv[ii+1], "%x", &PID);
                ii += 2;
            }
        }
        else if (!strcmp(argv[ii], "-d") || !strcmp(argv[ii], "-D"))
        {
            if (argc < ii + 4)
            {
                error = 1;
            }
            else
            {
                pFilename = argv[ii+1];
                ParseNumericArg(argv[ii+2], &address);
                ParseNumericArg(argv[ii+3], &length);
                ii += 4;
                function = FUNC_DOWNLOAD;
            }
        }
        else if (!strcmp(argv[ii], "-u") || !strcmp(argv[ii], "-U"))
        {
            if (argc < ii + 3)
            {
                error = 1;
            }
            else
            {
                pFilename = argv[ii+1];
                ParseNumericArg(argv[ii+2], &address);
                ii += 3;
                function = FUNC_UPLOAD;
            }
        }
        else if (!strcmp(argv[ii], "-x") || !strcmp(argv[ii], "-X"))
        {
            if (argc < ii + 3)
            {
                error = 1;
            }
            else
            {
                pFilename = argv[ii+1];
                ParseNumericArg(argv[ii+2], &address);
                ii += 3;
                function = FUNC_EXEC;
            }
        }
        else if (!strcmp(argv[ii], "-c") || !strcmp(argv[ii], "-C"))
        {
            console = 1;
            ii++;
        }
    }

    if (error || (!function && !console))
    {
        PrintUsage(argv[0]);
    }
    else
    {
        if (InitComms(VID, PID))
        {
            atexit(CloseComms);
            signal(SIGINT, Signal);
            switch (function)
            {
            case FUNC_DOWNLOAD:
                DoDownload(pFilename, address, length);
                break;
            case FUNC_UPLOAD:
                DoUpload(pFilename, address);
                break;
            case 3:
                DoExecute(pFilename, address);
                break;
            }

            if (console)
            {
                DoConsole();
            }
        }
    }

    return 0;
}

static void ParseNumericArg(const char *pArg, unsigned int *pResult)
{
    if (!strncmp(pArg, "0x", 2) || !strncmp(pArg, "0X", 2))
        sscanf(pArg, "%x", pResult);
    else
        sscanf(pArg, "%u", pResult);
}

static void PrintUsage(const char *pProgname)
{
    printf("Sega Saturn USB flash cart transfer utility\n");
    printf("by Anders Montonen, 2012\n");
    printf("Usage: %s [-options] [-commands]\n\n", pProgname);
    printf("Options:\n");
    printf("    -v  <VID>                     Device VID (Default 0x0403)\n");
    printf("    -p  <PID>                     Device PID (Default 0x6001)\n");
    printf("    -c                            Run debug console\n");
    printf("\n");
    printf("Commands:\n");
    printf("    -d  <file>  <address>  <size> Download data to file\n");
    printf("    -u  <file>  <address>         Upload data from file\n");
    printf("    -x  <file>  <address>         Upload program and execute\n");
    printf("USB IDs are given in hexadecimal, other arguments in decimal\n");
    printf("or hexadecimal (preceded by '0x')\n");
}

static int DoDownload(const char *pFilename, const unsigned int address,
                      const unsigned int size)
{
    unsigned char  *pFileBuffer = NULL;
    FILE           *File = NULL;
    unsigned int    received = 0;
    int             status = -1;
    crc_t           readChecksum, calcChecksum;
    struct timeval      before, after;
    signed long long    timedelta;

    pFileBuffer = (unsigned char*)malloc(size);
    if (pFileBuffer != NULL)
    {
        gettimeofday(&before, NULL);
        SendBuf[0] = FUNC_DOWNLOAD; /* Client function */
        SendBuf[1] = (unsigned char)(address >> 24);
        SendBuf[2] = (unsigned char)(address >> 16);
        SendBuf[3] = (unsigned char)(address >> 8);
        SendBuf[4] = (unsigned char)(address);
        SendBuf[5] = (unsigned char)(size >> 24);
        SendBuf[6] = (unsigned char)(size >> 16);
        SendBuf[7] = (unsigned char)(size >> 8);
        SendBuf[8] = (unsigned char)(size);

        status = ftdi_write_data(&Device, SendBuf, 9);
        if (status < 0)
        {
            printf("Send download command error: %s\n",
                   ftdi_get_error_string(&Device));
            goto DownloadError;
        }

        while (size - received > 0)
        {
            status = ftdi_read_data(&Device, &pFileBuffer[received], size);
            if (status < 0)
            {
                printf("Read data error: %s\n",
                       ftdi_get_error_string(&Device));
                goto DownloadError;
            }

            received += status;
        }

        // The transfer may timeout, so loop until a byte
        // is received or an error occurs.
        do
        {
            status = ftdi_read_data(&Device, (unsigned char*)&readChecksum, 1);
            if (status < 0)
            {
                printf("Read data error: %s\n",
                       ftdi_get_error_string(&Device));
                goto DownloadError;
            }
        } while (status == 0);

        gettimeofday(&after, NULL);
        timedelta = (signed long long) after.tv_sec * 1000000ll +
                    (signed long long) after.tv_usec -
                    (signed long long) before.tv_sec * 1000000ll -
                    (signed long long) before.tv_usec;
        printf("Transfer time %f\n", timedelta/1000000.0f);
        printf("Transfer speed %f K/s\n", (size/1024.0f)/(timedelta/1000000.0f));

        calcChecksum = crc_init();
        calcChecksum = crc_update(calcChecksum, pFileBuffer, size);
        calcChecksum = crc_finalize(calcChecksum);

        if (readChecksum != calcChecksum)
        {
            printf("Checksum error (%0x, should be %0x)\n",
                   calcChecksum, readChecksum);
            status = -1;
            goto DownloadError;
        }

        File = fopen(pFilename, "wb");
        if (File == NULL)
        {
            printf("Error creating output file\n");
            status = -1;
        }
        else
        {
            fwrite(pFileBuffer, 1, size, File);
            fclose(File);
        }

DownloadError:
        free(pFileBuffer);
    }

    return status < 0 ? 0 : 1;
}

/* Sending the write command and data separately is inefficient,
   but simplifies the code. The alternative is to copy also the data
   into the sendbuffer. */
static int DoUpload(const char *pFilename, const unsigned int Address)
{
    unsigned char      *pFileBuffer = NULL;
    unsigned int        size = -1, sent = 0;
    FILE               *File = NULL;
    int                 status = 0;
    crc_t               checksum = crc_init();
    struct timeval      before, after;
    signed long long    timedelta;

    File = fopen(pFilename, "rb");
    if (File == NULL)
    {
        printf("Can't open the file '%s'\n", pFilename);
    }
    else
    {
        fseek(File, 0, SEEK_END);
        size = ftell(File);
        fseek(File, 0, SEEK_SET);
        pFileBuffer = malloc(size);

        if (pFileBuffer == NULL)
        {
            printf("Memory allocation error\n");
        }
        else
        {
            fread(pFileBuffer, 1, size, File);
            checksum = crc_update(checksum, pFileBuffer, size);
            checksum = crc_finalize(checksum);

            gettimeofday(&before, NULL);
            SendBuf[0] = FUNC_UPLOAD; /* Client function */
            SendBuf[1] = (unsigned char)(Address >> 24);
            SendBuf[2] = (unsigned char)(Address >> 16);
            SendBuf[3] = (unsigned char)(Address >> 8);
            SendBuf[4] = (unsigned char)Address;
            SendBuf[5] = (unsigned char)(size >> 24);
            SendBuf[6] = (unsigned char)(size >> 16);
            SendBuf[7] = (unsigned char)(size >> 8);
            SendBuf[8] = (unsigned char)size;
            status = ftdi_write_data(&Device, SendBuf, 9);

            if (status < 0)
            {
                printf("Send upload command error: %s\n",
                       ftdi_get_error_string(&Device));
                goto UploadError;
            }

            while (size - sent > 0)
            {
                status = ftdi_write_data(&Device, &pFileBuffer[sent], size-sent);
                if (status < 0)
                {
                    printf("Send data error: %s\n",
                           ftdi_get_error_string(&Device));
                    goto UploadError;
                }

                sent += status;
            }

            SendBuf[0] = (unsigned char)checksum;
            status = ftdi_write_data(&Device, SendBuf, 1);

            if (status < 0)
            {
                printf("Send checksum error: %s\n",
                       ftdi_get_error_string(&Device));
                goto UploadError;
            }

            do
            {
                status = ftdi_read_data(&Device, RecvBuf, 1);
                if (status < 0)
                {
                    printf("Read upload result failed: %s\n",
                           ftdi_get_error_string(&Device));
                    goto UploadError;
                }
            } while (status == 0);

            if (RecvBuf[0] != 0)
            {
                status = -1;
            }

            gettimeofday(&after, NULL);
            timedelta = (signed long long) after.tv_sec * 1000000ll +
                        (signed long long) after.tv_usec -
                        (signed long long) before.tv_sec * 1000000ll -
                        (signed long long) before.tv_usec;
            printf("Transfer time %f\n", timedelta/1000000.0f);
            printf("Transfer speed %f K/s\n", (size/1024.0f)/(timedelta/1000000.0f));

UploadError:
            free(pFileBuffer);
        }
        fclose(File);
    }

    return status < 0 ? 0 : 1;
}

static int DoExecute(const char *pFilename, const unsigned int Address)
{
    int status = 0;
    if (DoUpload(pFilename, Address))
    {
        SendBuf[0] = FUNC_EXEC; /* Client function */
        SendBuf[1] = (unsigned char)(Address >> 24);
        SendBuf[2] = (unsigned char)(Address >> 16);
        SendBuf[3] = (unsigned char)(Address >> 8);
        SendBuf[4] = (unsigned char)Address;
        status = ftdi_write_data(&Device, SendBuf, 5);
        if (status < 0)
        {
            printf("Send execute error: %s\n",
                   ftdi_get_error_string(&Device));
        }
    }

    return status < 0 ? 0 : 1;
}

static int InitComms(const int VID, const int PID)
{
    int status = ftdi_init(&Device);
    int error = 0;

    if (status < 0)
    {
        printf("Init error: %s\n", ftdi_get_error_string(&Device));
        error = 1;
    }
    else
    {
        status = ftdi_usb_open(&Device, VID, PID);
        if (status < 0 && status != -5)
        {
            printf("Device open error: %s\n", ftdi_get_error_string(&Device));
            error = 1;
        }
        else
        {
            status = ftdi_usb_purge_buffers(&Device);
            if (status < 0)
            {
                printf("Purge buffers error: %s\n",
                       ftdi_get_error_string(&Device));
                error = 1;
            }

            status = ftdi_read_data_set_chunksize(&Device, USB_READPACKET_SIZE);
            if (status < 0)
            {
                printf("Set read chunksize error: %s\n",
                       ftdi_get_error_string(&Device));
                error = 1;
            }

            status = ftdi_write_data_set_chunksize(&Device, USB_WRITEPACKET_SIZE);
            if (status < 0)
            {
                printf("Set write chunksize error: %s\n",
                       ftdi_get_error_string(&Device));
                error = 1;
            }

            status = ftdi_set_bitmode(&Device, 0x0, BITMODE_RESET);
            if (status < 0)
            {
                printf("Bitmode configuration error: %s\n",
                       ftdi_get_error_string(&Device));
                error = 1;
            }

            if (error)
            {
                ftdi_usb_close(&Device);
            }
        }
    }

    return !error;
}

static void CloseComms(void)
{
    int status = ftdi_usb_purge_buffers(&Device);
    if (status < 0)
    {
        printf("Purge buffers error: %s\n",
               ftdi_get_error_string(&Device));
    }

    ftdi_usb_close(&Device);
}

static void Signal(int sig)
{
    exit(EXIT_FAILURE);
}

static void DoConsole(void)
{
    int ii;
    int status = 0;

    while (status >= 0)
    {
        // Read data in smaller chunks
        status = ftdi_read_data(&Device, RecvBuf, 62);
        if (status < 0)
        {
            printf("Read data error: %s\n",
                   ftdi_get_error_string(&Device));
        }
        else
        {
            for (ii = 0; ii < status; ++ii)
            {
                if (isprint(RecvBuf[ii]) || isblank(RecvBuf[ii]))
                {
                    putchar(RecvBuf[ii]);
                }
            }

            fflush(stdout);
        }
    }
}
