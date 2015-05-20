/*

    Sega Saturn cartridge flash tool
    Copyright © 2012, 2015 Anders Montonen
    Original software by ExCyber
    Graphics routines by Charles MacDonald
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

#include <stdlib.h> // for NULL

#include "vdp2.h"
#include "flash.h"

#define ID_ENTRY    0x9090
#define ID_EXIT     0xF0F0
#define PAGEWRITE   0xA0A0

static const char NoVendorString[] = "No Vendor ID - check cartridge connection.";
static const char UnknownVendorString[] = "Unknown Vendor ID - proceed with caution!";
static const char SSTVendorString[] = "Vendor: Silicon Storage Technology";
static const char ATVendorString[] = "Vendor: Atmel";
static const char AMDVendorString[] = "Vendor: AMD";

static const char NoDeviceString[] = "No Device ID - check cartridge connection.";
static const char UnknownDeviceString[] = "Unknown Device ID - proceed with caution!";
static const char SST29EE010DeviceString[] = "Device: SST29EE010";
static const char AT29C010DeviceString[] = "Device: AT29C010";
static const char AM29F010DeviceString[] = "Device: AM29F010";
static const char SST39SF010ADeviceString[] = "Device: SST39SF010A";

static volatile uint16_t* const ADDR_FLASH = (uint16_t*)0x22000000;
static volatile uint16_t* const ADDR_55 = (uint16_t*)0x2200AAAA; /* The flash sees this as 0x5555 */
static volatile uint16_t* const ADDR_AA = (uint16_t*)0x22005554; /* The flash sees this as 0x2AAA */

static void FlashUnlock(void)
{
    *ADDR_55 = 0xAAAA;
    *ADDR_AA = 0x5555;
}

static void FlashCommand(const uint16_t Command)
{
    FlashUnlock();
    *ADDR_55 = Command;
}

static void EraseFlashDefault(const FlashDevice_t *pDevice)
{
    uint32_t ii;
    uint32_t nPages = pDevice->RomSize / pDevice->PageSize;

    for (ii = 0; ii < nPages; ii++)
    {
        pDevice->EraseFlashPage(pDevice, ii);
    }
}

static void EraseFlashPageDefault(const FlashDevice_t *pDevice, const uint32_t RomPage)
{
    uint32_t ii;

    FlashCommand(PAGEWRITE);
    for (ii = 0; ii < pDevice->PageSize; ii++)
    {
        *(volatile uint16_t*)(ADDR_FLASH + (pDevice->PageSize * RomPage) + ii) = 0xFFFF;
    }

    WaitForVBLANKIn();
    WaitForVBLANKOut();
    WaitForVBLANKIn();
    WaitForVBLANKOut();
}

static void WriteFlashPageDefault(const FlashDevice_t *pDevice, const uint32_t RomPage,
                                  const uint16_t *pSourcePage)
{
    uint32_t ii;
    FlashCommand(PAGEWRITE);

    for (ii = 0; ii < pDevice->PageSize; ii++)
    {
        *(volatile uint16_t*)(ADDR_FLASH + (pDevice->PageSize * RomPage) + ii) = pSourcePage[ii];
    }

    WaitForVBLANKIn();
    WaitForVBLANKOut();
    WaitForVBLANKIn();
    WaitForVBLANKOut();
}

static void EraseFlashAMD29F010(const FlashDevice_t *pDevice)
{
    FlashUnlock();
    *ADDR_55 = 0x8080;
    *ADDR_55 = 0xAAAA;
    *ADDR_AA = 0x5555;
    *ADDR_55 = 0x1010;

    while(((*ADDR_FLASH) & 0x8080) == 0)
        ;
}

static void EraseFlashPageAMD29F010(const FlashDevice_t *pDevice, const uint32_t RomPage)
{
    FlashUnlock();
    *ADDR_55 = 0x8080;
    *ADDR_55 = 0xAAAA;
    *ADDR_AA = 0x5555;
    *(volatile uint16_t*)(ADDR_FLASH + (pDevice->PageSize * RomPage)) = 0x3030;

    while ((*(ADDR_FLASH + (pDevice->PageSize * RomPage)) & 0x8080) == 0)
        ;
}

static void WriteFlashPageAMD29F010(const FlashDevice_t *pDevice, const uint32_t RomPage,
                                    const uint16_t *pSourcePage)
{
    uint32_t ii;
    uint16_t CurrentData;
    volatile uint16_t *pCurrentAddr;

    for (ii = 0; ii < pDevice->PageSize; ii++)
    {
        pCurrentAddr = (ADDR_FLASH + (pDevice->PageSize * RomPage) + ii);
        CurrentData = pSourcePage[ii];
        FlashCommand(PAGEWRITE);
        *pCurrentAddr = CurrentData;
        while (((*pCurrentAddr) & 0x8080) != (CurrentData & 0x8080))
            ;
        while (((*pCurrentAddr) & 0x8080) != (CurrentData & 0x8080))
            ;
        while (((*pCurrentAddr) & 0x8080) != (CurrentData & 0x8080))
            ;
    }
}

static void FillNoDeviceInfo(const uint16_t DeviceID, FlashDevice_t *pFlashDevice)
{
    pFlashDevice->pVendorString = NoVendorString;
    pFlashDevice->pDeviceString = NoDeviceString;
    pFlashDevice->RomSize = 0;
    pFlashDevice->PageSize = 0;
    pFlashDevice->EraseFlash = NULL;
    pFlashDevice->EraseFlashPage = NULL;
    pFlashDevice->WriteFlashPage = NULL;
}

static void FillUnknownDeviceInfo(const uint16_t DeviceID, FlashDevice_t *pFlashDevice)
{
    pFlashDevice->pVendorString = UnknownVendorString;
    pFlashDevice->pDeviceString = UnknownDeviceString;
    pFlashDevice->RomSize = 131072;
    pFlashDevice->PageSize = 128;
    pFlashDevice->EraseFlash = EraseFlashDefault;
    pFlashDevice->EraseFlashPage = EraseFlashPageDefault;
    pFlashDevice->WriteFlashPage = WriteFlashPageDefault;
}

static void FillSSTDeviceInfo(const uint16_t DeviceID, FlashDevice_t *pFlashDevice)
{
    pFlashDevice->pVendorString = SSTVendorString;

    switch (DeviceID)
    {
    case 0x0707:
        pFlashDevice->pDeviceString = SST29EE010DeviceString;
        pFlashDevice->RomSize = 131072;
        pFlashDevice->PageSize = 128;
        pFlashDevice->EraseFlash = EraseFlashDefault;
        pFlashDevice->EraseFlashPage = EraseFlashPageDefault;
        pFlashDevice->WriteFlashPage = WriteFlashPageDefault;
        break;
    case 0xB5B5:
        pFlashDevice->pDeviceString = SST39SF010ADeviceString;
        pFlashDevice->RomSize = 131072;
        pFlashDevice->PageSize = 4096;
        pFlashDevice->EraseFlash = EraseFlashAMD29F010;
        pFlashDevice->EraseFlashPage = EraseFlashPageAMD29F010;
        pFlashDevice->WriteFlashPage = WriteFlashPageAMD29F010;
        break;
    default:
        pFlashDevice->pDeviceString = UnknownDeviceString;
        pFlashDevice->RomSize = 131072;
        pFlashDevice->PageSize = 128;
        pFlashDevice->EraseFlash = EraseFlashDefault;
        pFlashDevice->EraseFlashPage = EraseFlashPageDefault;
        pFlashDevice->WriteFlashPage = WriteFlashPageDefault;
        break;
    }
}

static void FillATDeviceInfo(const uint16_t DeviceID, FlashDevice_t *pFlashDevice)
{
    pFlashDevice->pVendorString = ATVendorString;

    switch (DeviceID)
    {
    case 0xD5D5:
        pFlashDevice->pDeviceString = AT29C010DeviceString;
        pFlashDevice->RomSize = 131072;
        pFlashDevice->PageSize = 128;
        pFlashDevice->EraseFlash = EraseFlashDefault;
        pFlashDevice->EraseFlashPage = EraseFlashPageDefault;
        pFlashDevice->WriteFlashPage = WriteFlashPageDefault;
        break;
    default:
        pFlashDevice->pDeviceString = UnknownDeviceString;
        pFlashDevice->RomSize = 131072;
        pFlashDevice->PageSize = 128;
        pFlashDevice->EraseFlash = EraseFlashDefault;
        pFlashDevice->EraseFlashPage = EraseFlashPageDefault;
        pFlashDevice->WriteFlashPage = WriteFlashPageDefault;
        break;
    }
}

static void FillAMDDeviceInfo(const uint16_t DeviceID, FlashDevice_t *pFlashDevice)
{
    pFlashDevice->pVendorString = AMDVendorString;

    switch (DeviceID)
    {
    case 0x2020:
        pFlashDevice->pDeviceString = AM29F010DeviceString;
        pFlashDevice->RomSize = 131072;
        pFlashDevice->PageSize = 1024*16;
        pFlashDevice->EraseFlash = EraseFlashAMD29F010;
        pFlashDevice->EraseFlashPage = EraseFlashPageAMD29F010;
        pFlashDevice->WriteFlashPage = WriteFlashPageAMD29F010;
        break;
    default:
        pFlashDevice->pDeviceString = UnknownDeviceString;
        pFlashDevice->RomSize = 131072;
        pFlashDevice->PageSize = 128;
        pFlashDevice->EraseFlash = EraseFlashAMD29F010;
        pFlashDevice->EraseFlashPage = EraseFlashPageAMD29F010;
        pFlashDevice->WriteFlashPage = WriteFlashPageAMD29F010;
        break;
    }
}

void FillFlashDeviceInfo(const uint16_t VendorID, const uint16_t DeviceID,
                         FlashDevice_t *pFlashDevice)
{
    switch (VendorID)
    {
    case 0xBFBF:
        FillSSTDeviceInfo(DeviceID, pFlashDevice);
        break;
    case 0x1F1F:
        FillATDeviceInfo(DeviceID, pFlashDevice);
        break;
    case 0x0101:
        FillAMDDeviceInfo(DeviceID, pFlashDevice);
        break;
    case 0xFFFF:
        FillNoDeviceInfo(DeviceID, pFlashDevice);
        break;
    default:
        FillUnknownDeviceInfo(DeviceID, pFlashDevice);
        break;
    }
}

uint16_t FlashGetVendorID(void)
{
    uint16_t VendorID;
    static volatile uint16_t* const ADDR_VENDOR = (uint16_t*)0x22000000;

    FlashCommand(ID_ENTRY);
    WaitForVBLANKIn();
    WaitForVBLANKOut();
    WaitForVBLANKIn(); /* Bodge to wait for flash to complete the command */
    WaitForVBLANKOut();
    VendorID = *ADDR_VENDOR;
    FlashCommand(ID_EXIT);
    WaitForVBLANKIn();
    WaitForVBLANKOut();
    WaitForVBLANKIn();
    WaitForVBLANKOut();

    return VendorID;
}

uint16_t FlashGetDeviceID(void)
{
    uint16_t DeviceID;
    static volatile uint16_t* const ADDR_DEVICE = (uint16_t*)0x22000002; /* The flash sees this as 0x0001 */

    FlashCommand(ID_ENTRY);
    WaitForVBLANKIn();
    WaitForVBLANKOut();
    WaitForVBLANKIn();
    WaitForVBLANKOut();
    DeviceID = *ADDR_DEVICE;
    FlashCommand(ID_EXIT);
    WaitForVBLANKIn();
    WaitForVBLANKOut();
    WaitForVBLANKIn();
    WaitForVBLANKOut();

    return DeviceID;
}
