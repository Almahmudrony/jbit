/*
 * Copyright (C) 2012-2013  Emanuele Fornara
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

// devimpl.cpp

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <sys/time.h>

#include "jbit.h"
#include "devimpl.h"

long long Random::next() {
	seed[0] = (seed[0] * 0x5DEECE66DLL + 0xBLL) & MAXRAND;
	return seed[0];
}

void Random::reset() {	
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long long t = ((long long)tv.tv_sec * 1000LL) + tv.tv_usec / 1000;
	seed[0] = t & MAXRAND;
	seed[1] = 0;
	put(255);

}

int Random::get() {
	long long i;
	while (n <= (i = next() / divisor))
		;
	return (int)i;

}
void Random::put(int max) {
	if (max == 0) {
		long long t = seed[0];
		seed[0] = seed[1];
		seed[1] = t;
	} else {
		n = max + 1;
		divisor = MAXRAND / n;
	}
}

void MicroIODisplay::reset() {
	memset(video_buf, ' ', sizeof(video_buf));
}

void MicroIODisplay::put(int address, int value) {
	if (address >= 0 && address < (int)sizeof(video_buf))
		video_buf[address] = value;
}

int MicroIODisplay::get(int address) const {
	if (address >= 0 && address < (int)sizeof(video_buf))
		return video_buf[address] & 0xff;
	return 0;
}

const char *MicroIODisplay::get_line(int i) const {
	if (i <= 0 || i >= (N_OF_LINES - 1)) {
		strcpy(line_buf, "+----------+");
	} else {
		line_buf[0] = '|';
		const char *p = &video_buf[(i - 1) * COLS];
		for (i = 1; i <= COLS; i++) {
			char c = *p++;
			line_buf[i] = isprint((int)c) ? c : ' ';
		}
		line_buf[COLS + 1] = '|';
		line_buf[COLS + 2] = 0;
	}
	return line_buf;
}
