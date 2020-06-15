/*
 * libusb example program to manipulate U.are.U 4000B fingerprint scanner.
 * Copyright (C) 2007 Daniel Drake <dsd@gentoo.org>
 *
 * Basic image capture program only, does not consider the powerup quirks or
 * the fact that image encryption may be enabled. Not expected to work
 * flawlessly all of the time.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#ifndef NK6UVME_H
#define NK6UVME_H

#include <libusb.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VME_VENDOR_ID (0x04b4)
#define VME_PRODUCT_ID (0x00f0)

#define VME6U_SF_READ (0x82)
#define VME6U_SF_WRITE (0x06)

// VME mode definition
#define A16D16  (0x69)
#define A16D32  (0x29)
#define A24D16  (0x79)
#define A24D32  (0x39)
#define A32D16  (0x49)
#define A32D32  (0x09)

  extern int VMEinit(libusb_context **ctx);
  extern void VMEprintOpenDevices(void);
  extern int VMEopen(int sid, libusb_context *ctx);
  extern void VMEclose(int sid);
  extern void VMEexit(libusb_context *ctx);
  extern int VMEwrite(int sid, unsigned char am, unsigned char tout, unsigned long address, unsigned long data);
  extern unsigned long VMEread(int sid, unsigned char am, unsigned char tout, unsigned long address);
  extern int VMEblockread(int sid, unsigned char am, unsigned char tout, unsigned long address, unsigned long nbytes, unsigned char *data);
  extern int VMEreset(int sid);

  extern unsigned long VMEread_error(int sid, unsigned char am, unsigned char tout, unsigned long address);
  extern int VMEblockread_error(int sid, unsigned char am, unsigned char tout, unsigned long address, unsigned long nbytes, unsigned char *data);

#ifdef __cplusplus
}
#endif

#endif
