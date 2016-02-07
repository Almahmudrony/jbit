/*** File generated by inc2h.pl - DO NOT EDIT ***/

/*                                                                         */
/*  xv65.inc                                                               */
/*                                                                         */

/*                                                                         */
/*  Copyright (C) 2012-2013  Emanuele Fornara                              */
/*  All rights reserved.                                                   */
/*                                                                         */
/*  Redistribution and use in source and binary forms, with or without     */
/*  modification, are permitted provided that the following conditions     */
/*  are met:                                                               */
/*   * Redistributions of source code must retain the above copyright      */
/*     notice, this list of conditions and the following disclaimer.       */
/*   * Redistributions in binary form must reproduce the above copyright   */
/*     notice, this list of conditions and the following disclaimer in     */
/*     the documentation and/or other materials provided with the          */
/*     distribution.                                                       */
/*                                                                         */
/*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS    */
/*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT      */
/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS      */
/*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE         */
/*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,    */
/*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,   */
/*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS  */
/*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED     */
/*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, */
/*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT     */
/*  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF     */
/*  SUCH DAMAGE.                                                           */
/*                                                                         */

#define PUTCHAR 0x0200

#define REQPUT 0x0201
#define REQEND 0x0202
#define REQRES 0x0203

#define REQPTRLO 0x0204
#define REQPTRHI 0x0205

#define FRMFPS 0x0211
#define FRMDRAW 0x0212
#define TERMIOS 0x0215
#define PUTUINT8 0x0216
#define RANDOM 0x0217
#define KEYBUF 0x0218

#define CONCOLS 0x0220
#define CONROWS 0x0221
#define CONESC 0x0227
#define CONVIDEO 0x0228
#define CONROW0 0x0228
#define CONROW1 0x0232
#define CONROW2 0x023C
#define CONROW3 0x0246

#define REQERRNO 0x0250
#define PIDSIZE 0x0251
#define TRCLEVEL 0x0252
#define ERREXIT 0x0253
#define REDIRPUT 0x0254

#define REQDAT 0x0260
#define REQDAT2 0x0268

#define REQ_FORK 1
#define REQ_EXIT 2
#define REQ_WAIT 3
#define REQ_PIPE 4
#define REQ_READ 5
#define REQ_KILL 6
#define REQ_EXEC 7
#define REQ_FSTAT 8
#define REQ_CHDIR 9
#define REQ_DUP 10
#define REQ_GETPID 11
/* REQ_SBRK   = 12                                                         */
#define REQ_SLEEP 13
/* REQ_UPTIME = 14                                                         */
#define REQ_OPEN 15
#define REQ_WRITE 16
/* REQ_MKNOD  = 17                                                         */
#define REQ_UNLINK 18
/* REQ_LINK   = 19                                                         */
#define REQ_MKDIR 20
#define REQ_CLOSE 21

#define REQ_ARGC 32
#define REQ_ARGV 33
#define REQ_ENV 34

#define REQ_TIME 48
#define REQ_RMDIR 49
#define REQ_LSEEK 50

#define TERMIOS_ICANON 1
#define TERMIOS_MICROIO 2
#define TERMIOS_RAW 3
#define TERMIOS_MODMASK 7
#define TERMIOS_DIRTY 128

#define ESC_HOME 1
#define ESC_CLEAR 2
#define ESC_NORMAL 3
#define ESC_OBR 4

#define ESC_FG_BLACK 30
#define ESC_FG_RED 31
#define ESC_FG_GREEN 32
#define ESC_FG_YELLOW 33
#define ESC_FG_BLUE 34
#define ESC_FG_MAGENTA 35
#define ESC_FG_CYAN 36
#define ESC_FG_WHITE 37

#define ESC_BG_BLACK 40
#define ESC_BG_RED 41
#define ESC_BG_GREEN 42
#define ESC_BG_YELLOW 43
#define ESC_BG_BLUE 44
#define ESC_BG_MAGENTA 45
#define ESC_BG_CYAN 46
#define ESC_BG_WHITE 47


#define XV65_SIGHUP 1
#define XV65_SIGINT 2
#define XV65_SIGQUIT 3
#define XV65_SIGILL 4
#define XV65_SIGABRT 6
#define XV65_SIGFPE 8
#define XV65_SIGKILL 9
#define XV65_SIGSEGV 11
#define XV65_SIGPIPE 13
#define XV65_SIGALRM 14
#define XV65_SIGTERM 15

#define XV65_EPERM 1
#define XV65_ENOENT 2
#define XV65_ESRCH 3
#define XV65_EINTR 4
#define XV65_EIO 5
#define XV65_ENXIO 6
#define XV65_E2BIG 7
#define XV65_ENOEXEC 8
#define XV65_EBADF 9
#define XV65_ECHILD 10
#define XV65_EAGAIN 11
#define XV65_ENOMEM 12
#define XV65_EACCES 13
#define XV65_EFAULT 14
#define XV65_ENOTBLK 15
#define XV65_EBUSY 16
#define XV65_EEXIST 17
#define XV65_EXDEV 18
#define XV65_ENODEV 19
#define XV65_ENOTDIR 20
#define XV65_EISDIR 21
#define XV65_EINVAL 22
#define XV65_ENFILE 23
#define XV65_EMFILE 24
#define XV65_ENOTTY 25
#define XV65_ETXTBSY 26
#define XV65_EFBIG 27
#define XV65_ENOSPC 28
#define XV65_ESPIPE 29
#define XV65_EROFS 30
#define XV65_EMLINK 31
#define XV65_EPIPE 32
#define XV65_EDOM 33
#define XV65_ERANGE 34

#define XV65_O_RDONLY 1
#define XV65_O_WRONLY 2
#define XV65_O_RDWR 3
#define XV65_O_CREAT 16
#define XV65_O_TRUNC 32
#define XV65_O_APPEND 64
#define XV65_O_EXCL 128

#define XV65_T_BLK 1
#define XV65_T_CHR 2
#define XV65_T_DIR 4
#define XV65_T_FIFO 8
#define XV65_T_LNK 16
#define XV65_T_REG 32
#define XV65_T_SOCK 64

#define XV65_SEEK_SET 0
#define XV65_SEEK_CUR 1
#define XV65_SEEK_END 2

