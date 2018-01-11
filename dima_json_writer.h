#ifndef DIMA_JSON_WRITER_H_INCLUDED
#define DIMA_JSON_WRITER_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>

#define DIMA_JSON_WRITER_MAX_DEPTH 64
#define DIMA_JSON_WRITER_DEFAULT_BUF_LEN 512
#define DIMA_JSON_WRITER_REALLOC_DIFF 512

#ifndef DIMA_JSON_WRITER_USE_STB_SPRINTF
#ifdef _CRT_SECURE_NO_WARNINGS
#define DIMA_JSON_WRITER_SPRINTF sprintf
#else
#define DIMA_JSON_WRITER_SPRINTF sprintf
#endif
#else
#define DIMA_JSON_WRITER_SPRINTF stbsp_sprintf
#endif

#ifndef DIMA_JSON_WRITER_DEF
#ifdef DIMA_JSON_WRITER_STATIC
#define DIMA_JSON_WRITER_DEF static
#else
#define DIMA_JSON_WRITER_DEF extern
#endif
#endif

enum {
	JSONWriterFlag_None = 0,
	JSONWriterFlag_Pretty,
};

struct json_writer {
	char* Buf;
	int32_t BufSize;

	int32_t CurrentIndex;
	int32_t LastPossibleCommaIndex;

	int32_t CurrentLayer;
	int32_t LayerElements[DIMA_JSON_WRITER_MAX_DEPTH];

	uint32_t Flags;
};

#ifdef __cplusplus
extern "C" {
#endif

	DIMA_JSON_WRITER_DEF void JSONInit(json_writer* Writer, uint32_t Flags = JSONWriterFlag_None);
	DIMA_JSON_WRITER_DEF void JSONFree(json_writer* Writer);

	DIMA_JSON_WRITER_DEF void JSONBegin(json_writer* Writer);
	DIMA_JSON_WRITER_DEF void JSONBeginName(json_writer* Writer, char* Name);
	DIMA_JSON_WRITER_DEF void JSONEnd(json_writer* Writer);

	DIMA_JSON_WRITER_DEF void JSONBeginArr(json_writer* Writer, char* Name);
	DIMA_JSON_WRITER_DEF void JSONEndArr(json_writer* Writer);

	DIMA_JSON_WRITER_DEF void JSONAddU32(json_writer* Writer, char* Key, uint32_t Value);
	DIMA_JSON_WRITER_DEF void JSONAddS32(json_writer* Writer, char* Key, int32_t Value);
	DIMA_JSON_WRITER_DEF void JSONAddU64(json_writer* Writer, char* Key, uint64_t Value);
	DIMA_JSON_WRITER_DEF void JSONAddS64(json_writer* Writer, char* Key, int64_t Value);

	DIMA_JSON_WRITER_DEF void JSONAddSTR(json_writer* Writer, char* Key, char* Value);
	DIMA_JSON_WRITER_DEF void JSONAddFixedSTR(json_writer* Writer, char* Key, char* Value, int32_t ValueLen);

	DIMA_JSON_WRITER_DEF void JSONAddDataHex(json_writer* Writer, char* Key, uint8_t* Value, int32_t ValueLen);

	DIMA_JSON_WRITER_DEF char* JSONGetBuf(json_writer* Writer);
#ifdef __cplusplus
}
#endif


#endif

#if defined(DIMA_JSON_WRITER_IMPLEMENTATION) && !defined(DIMA_JSON_WRITER_IMPLEMENTATION_DONE)
#define DIMA_JSON_WRITER_IMPLEMENTATION_DONE

static void JSONReallocIfNeeded(json_writer* Writer) {
	if (Writer->CurrentIndex > Writer->BufSize - 128) {
		Writer->Buf = (char*)realloc(Writer->Buf, Writer->BufSize + DIMA_JSON_WRITER_REALLOC_DIFF);
		Writer->BufSize += DIMA_JSON_WRITER_REALLOC_DIFF;
	}
}

static void JSONCopyCharToBuf(json_writer* Writer, char ToWrite) {
	JSONReallocIfNeeded(Writer);
	Writer->Buf[Writer->CurrentIndex++] = ToWrite;
}

static void JSONCopyStrToBuf(json_writer* Writer, char* ToWrite) {
	char* At = ToWrite;
	while (*At) {
		JSONCopyCharToBuf(Writer, *At);

		At++;
	}
}

static void JSONWriteLine(json_writer* Writer, char* Str) {
	int32_t StrLen = strlen(Str);

	if (Str[StrLen - 1] != '}' && Str[StrLen - 1] != ']') {
		if (Writer->LayerElements[Writer->CurrentLayer] > 0) {
			Writer->CurrentIndex = Writer->LastPossibleCommaIndex;
			JSONCopyCharToBuf(Writer, ',');

			if (Writer->Flags & JSONWriterFlag_Pretty) {
				JSONCopyCharToBuf(Writer, '\n');
			}
		}
	}

	if (Writer->Flags & JSONWriterFlag_Pretty) {
		for (int TabulationIndex = 0;
			TabulationIndex < Writer->CurrentLayer;
			TabulationIndex++)
		{
			JSONCopyCharToBuf(Writer, '\t');
		}
	}

	JSONCopyStrToBuf(Writer, Str);

	Writer->LastPossibleCommaIndex = Writer->CurrentIndex;

	if (Writer->Flags & JSONWriterFlag_Pretty) {
		JSONCopyCharToBuf(Writer, '\n');
	}
}

void JSONInit(json_writer* Writer, uint32_t Flags) {
	Writer->Buf = (char*)malloc(DIMA_JSON_WRITER_DEFAULT_BUF_LEN * sizeof(char));
	Writer->BufSize = DIMA_JSON_WRITER_DEFAULT_BUF_LEN;

	Writer->CurrentIndex = 0;
	Writer->LastPossibleCommaIndex = 0;
	Writer->Flags = Flags;

	Writer->CurrentLayer = 0;
	for (int LayerIndex = 0;
		LayerIndex < DIMA_JSON_WRITER_MAX_DEPTH;
		LayerIndex++)
	{
		Writer->LayerElements[LayerIndex] = 0;
	}
}

void JSONFree(json_writer* Writer) {
	free(Writer->Buf);
}

void JSONBegin(json_writer* Writer) {
	JSONWriteLine(Writer, "{");

	Writer->CurrentLayer++;
}

void JSONBeginName(json_writer* Writer, char* Name) {
	char LineBuf[256];
	DIMA_JSON_WRITER_SPRINTF(LineBuf, "\"%s\": {", Name);

	JSONWriteLine(Writer, LineBuf);

	Writer->CurrentLayer++;
}

void JSONEnd(json_writer* Writer) {
	Writer->LayerElements[Writer->CurrentLayer] = 0;
	Writer->CurrentLayer--;

	JSONWriteLine(Writer, "}");
	Writer->Buf[Writer->CurrentIndex] = 0;

	Writer->LayerElements[Writer->CurrentLayer]++;
}

void JSONBeginArr(json_writer* Writer, char* Name) {
	char LineBuf[256];
	DIMA_JSON_WRITER_SPRINTF(LineBuf, "\"%s\": [", Name);

	JSONWriteLine(Writer, LineBuf);

	Writer->CurrentLayer++;
}

void JSONEndArr(json_writer* Writer) {
	Writer->LayerElements[Writer->CurrentLayer] = 0;
	Writer->CurrentLayer--;

	JSONWriteLine(Writer, "]");

	Writer->LayerElements[Writer->CurrentLayer]++;
}

void JSONAddU32(json_writer* Writer, char* Key, uint32_t Value) {
	char LineBuf[256];
	DIMA_JSON_WRITER_SPRINTF(LineBuf, "\"%s\": %u", Key, Value);

	JSONWriteLine(Writer, LineBuf);

	Writer->LayerElements[Writer->CurrentLayer]++;
}

void JSONAddS32(json_writer* Writer, char* Key, int32_t Value) {
	char LineBuf[256];
	DIMA_JSON_WRITER_SPRINTF(LineBuf, "\"%s\": %d", Key, Value);

	JSONWriteLine(Writer, LineBuf);

	Writer->LayerElements[Writer->CurrentLayer]++;
}

void JSONAddU64(json_writer* Writer, char* Key, uint64_t Value) {
	char LineBuf[256];
	DIMA_JSON_WRITER_SPRINTF(LineBuf, "\"%s\": %llu", Key, Value);

	JSONWriteLine(Writer, LineBuf);

	Writer->LayerElements[Writer->CurrentLayer]++;
}

void JSONAddS64(json_writer* Writer, char* Key, int64_t Value) {
	char LineBuf[256];
	DIMA_JSON_WRITER_SPRINTF(LineBuf, "\"%s\": %lld", Key, Value);

	JSONWriteLine(Writer, LineBuf);

	Writer->LayerElements[Writer->CurrentLayer]++;
}


/*
void add_str(char* Key, char* Value) {
	char LineBuf[256];
	DIMA_JSON_WRITER_SPRINTF(LineBuf, "\"%s\": \"%s\"", Key, Value);

	write_line(LineBuf);

	this->LayerElements[this->CurrentLayer]++;
}
*/

void JSONAddSTR(json_writer* Writer, char* Key, char* Value) {
	int32_t KeyLen = strlen(Key);
	int32_t ValueLen = strlen(Value);

	char* LineBuf = (char*)malloc(KeyLen + ValueLen + 16);
	if (Value) {
		DIMA_JSON_WRITER_SPRINTF(LineBuf, "\"%s\": \"%s\"", Key, Value);
	}
	else {
		DIMA_JSON_WRITER_SPRINTF(LineBuf, "\"%s\": 0", Key);
	}

	JSONWriteLine(Writer, LineBuf);

	Writer->LayerElements[Writer->CurrentLayer]++;

	free(LineBuf);
}

void JSONAddFixedSTR(json_writer* Writer, char* Key, char* Value, int32_t ValueLen) {
	int32_t KeyLen = strlen(Key);
	char* LineBuf = (char*)malloc(KeyLen + ValueLen + 16);

	if (Value) {
		char LinePart1[256];
		DIMA_JSON_WRITER_SPRINTF(LinePart1, "\"%s\": \"", Key);

		char LinePart2[256];
		int i = 0;
		for (i = 0; i < ValueLen; i++)
		{
			LinePart2[i] = Value[i];
		}
		LinePart2[i++] = '\"';
		LinePart2[i] = 0;

		DIMA_JSON_WRITER_SPRINTF(LineBuf, "%s%s", LinePart1, LinePart2);
	}
	else {
		DIMA_JSON_WRITER_SPRINTF(LineBuf, "\"%s\": 0", Key);
	}

	JSONWriteLine(Writer, LineBuf);

	Writer->LayerElements[Writer->CurrentLayer]++;

	free(LineBuf);
}

void JSONAddDataHex(json_writer* Writer, char* Key, uint8_t* Value, int32_t ValueLen) {
	int32_t KeyLen = strlen(Key);

	char* LineBuf = (char*)malloc(KeyLen + ValueLen * 2 + 16);

	if (Value) {
		char* To = LineBuf;
		*To++ = '\"';

		for (int i = 0; i < KeyLen; i++) {
			*To++ = Key[i];
		}
		*To++ = '\"';
		*To++ = ':';
		*To++ = ' ';
		*To++ = '\"';

		for (int i = 0; i < ValueLen; i++) {
			char Temp[4];
			DIMA_JSON_WRITER_SPRINTF(Temp, "%2.2X", Value[i]);

			*To++ = Temp[0];
			*To++ = Temp[1];
		}
		*To++ = '\"';
		*To = 0;
	}
	else {
		DIMA_JSON_WRITER_SPRINTF(LineBuf, "\"%s\": 0", Key);
	}

	JSONWriteLine(Writer, LineBuf);

	Writer->LayerElements[Writer->CurrentLayer]++;

	free(LineBuf);
}

char* JSONGetBuf(json_writer* Writer) {
	char* Result = Writer->Buf;

	return(Result);
}

#endif