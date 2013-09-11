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

// core.h

class Buffer {
private:
	char *data;
	int size;
	int length;
	Buffer(const Buffer &); // copy forbidden
	Buffer& operator=(const Buffer&); // assignment forbidden
public:
	Buffer(int initial_size = 256);
	~Buffer() { delete[] data; }
	void reset() { length = 0; }
	void append_line(const char *line);
	void append_data(const char *p, int len);
	char *append_raw(int len);
	const char *get_data() const { return data; }
	int get_length() const { return length; }
};

struct ParseError {
	Buffer msg;
	int lineno;
};

class Parser {
private:
	const Buffer *buffer;
public:
	Parser(const Buffer *buffer_);
	bool has_signature();
	const ParseError *parse(const Buffer *program);
};

class IO {
public:
	virtual void reset() = 0;
	virtual void put(int address, int value) = 0;
	virtual int get(int address) = 0;
	virtual ~IO() {}
};

class VM {
public:
	virtual void reset() = 0;
	virtual void put(int address, int value) = 0;
	virtual void load(const Buffer *program) = 0;
	virtual int step() = 0;
	virtual ~VM() {}
};

extern VM *new_VM(IO *io);
