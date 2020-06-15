/*
 * NoticeKorea NKUSBVME USB/VME Interface for Linux
 *
 *  by H.J. Kim (hongjoo@knu.ac.kr)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 *
 *   TODO
 *   
 * Changelog:
 * 
 */

int uopen(int devnum);
int uclose(int devnum);
int uwrite(int devnum, char *buffer, unsigned long length);
int uread(int devnum, char *buffer, unsigned long length);

