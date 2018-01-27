
#ifdef STB_TRUETYPE_IMPLEMENTATION

typedef unsigned char stbtt_uint8;
typedef signed char stbtt_int8;
typedef unsigned short stbtt_uint16;
typedef signed short stbtt_int16;
typedef unsigned int stbtt_uint32;
typedef signed int stbtt_int32;

#endif

#ifndef __STB_INCLUDE_STB_TRUETYPE_H__
#define __STB_INCLUDE_STB_TRUETYPE_H__

typedef struct {
	unsigned char * data;
	int cursor, size;
} stbtt__buf;

typedef struct {
	unsigned short x0, y0, x1, y1;
	float xoff, yoff, xadvance;
} stbtt_bakedchar;

int stbtt_BakeFontBitmap(const unsigned char *, int, float, unsigned char *, int, int, int, int, stbtt_bakedchar *);

typedef struct {
	float x0, y0, s0, t0, x1, y1, s1, t1;
} stbtt_aligned_quad;

void stbtt_GetBakedQuad(stbtt_bakedchar *, int, int, int, float *, float *, stbtt_aligned_quad *, int);

typedef struct {
	unsigned short x0, y0, x1, y1;
	float xoff, yoff, xadvance, xoff2, yoff2;
} stbtt_packedchar;

typedef struct stbtt_pack_context stbtt_pack_context;
typedef struct stbtt_fontinfo stbtt_fontinfo;

int stbtt_PackBegin(stbtt_pack_context *, unsigned char *, int, int, int, int, void *);
void stbtt_PackEnd(stbtt_pack_context *);
int stbtt_PackFontRange(stbtt_pack_context *, unsigned char *, int, float, int, int, stbtt_packedchar *);

typedef struct {
	float font_size;
	int first_unicode_codepoint_in_range;
	int * array_of_unicode_codepoints, num_chars;
	stbtt_packedchar * chardata_for_range;
	unsigned char h_oversample, v_oversample;
} stbtt_pack_range;

int stbtt_PackFontRanges(stbtt_pack_context *, unsigned char *, int, stbtt_pack_range *, int);
void stbtt_PackSetOversampling(stbtt_pack_context *, unsigned int, unsigned int);
void stbtt_GetPackedQuad(stbtt_packedchar *, int, int, int, float *, float *, stbtt_aligned_quad *, int);
int stbtt_PackFontRangesGatherRects(stbtt_pack_context *, const stbtt_fontinfo *, stbtt_pack_range *, int, stbrp_rect *);
void stbtt_PackFontRangesPackRects(stbtt_pack_context *, stbrp_rect *, int);
int stbtt_PackFontRangesRenderIntoRects(stbtt_pack_context *, const stbtt_fontinfo *, stbtt_pack_range *, int, stbrp_rect *);

struct stbtt_pack_context {
	void * user_allocator_context;
	void * pack_info;
	int width, height, stride_in_bytes, padding;
	unsigned int h_oversample, v_oversample;
	unsigned char * pixels;
	void * nodes;
};

int stbtt_GetFontOffsetForIndex(int);

struct stbtt_fontinfo {
	void * userdata;
	unsigned char * data;
	int fontstart;
	int numGlyphs;
	int loca, head, glyf, hhea, hmtx;
	int index_map;
	int indexToLocFormat;
	stbtt__buf cff, charstrings, gsubrs, subrs, fontdicts, fdselect;
};

int stbtt_InitFont(stbtt_fontinfo *, const unsigned char *, int);
int stbtt_FindGlyphIndex(const stbtt_fontinfo *, int);
float stbtt_ScaleForPixelHeight(const stbtt_fontinfo *, float);
float stbtt_ScaleForMappingEmToPixels(const stbtt_fontinfo *, float);
void stbtt_GetFontVMetrics(const stbtt_fontinfo *, int *, int *, int *);
void stbtt_GetFontBoundingBox(const stbtt_fontinfo *, int *, int *, int *, int *);
void stbtt_GetCodepointHMetrics(const stbtt_fontinfo *, int, int *, int *);
int stbtt_GetCodepointBox(const stbtt_fontinfo *, int, int *, int *, int *, int *);
void stbtt_GetGlyphHMetrics(const stbtt_fontinfo *, int, int *, int *);
int stbtt_GetGlyphBox(const stbtt_fontinfo *, int, int *, int *, int *, int *);

enum {
	STBTT_vmove = 1, STBTT_vline, STBTT_vcurve, STBTT_vcubic,
};

typedef struct {
	short x, y, cx, cy, cx1, cy1;
	unsigned char type, padding;
} stbtt_vertex;

bool stbtt_IsGlyphEmpty(const stbtt_fontinfo *, int);
int stbtt_GetCodepointShape(const stbtt_fontinfo *, int, stbtt_vertex **);
int stbtt_GetGlyphShape(const stbtt_fontinfo *, int, stbtt_vertex **);
unsigned char * stbtt_GetCodepointBitmap(const stbtt_fontinfo *, float, float, int, int *, int *, int *, int *);
unsigned char * stbtt_GetCodepointBitmapSubpixel(const stbtt_fontinfo *, float, float, float, float, int, int *, int *, int *, int *);
void stbtt_MakeCodepointBitmap(const stbtt_fontinfo *, unsigned char *, int, int, int, float, float, int);
void stbtt_MakeCodepointBitmapSubpixel(const stbtt_fontinfo *, unsigned char *, int, int, int, float, float, float, float, int);
void stbtt_GetCodepointBitmapBox(const stbtt_fontinfo *, int, float, float, int *, int *, int *, int *);
void stbtt_GetCodepointBitmapBoxSubpixel(const stbtt_fontinfo *, int, float, float, float, float, int *, int *, int *, int *);
unsigned char * stbtt_GetGlyphBitmap(const stbtt_fontinfo *, float, float, int, int *, int *, int *, int *);
unsigned char * stbtt_GetGlyphBitmapSubpixel(const stbtt_fontinfo *, float, float, float, float, int, int *, int *, int *, int *);
void stbtt_MakeGlyphBitmap(const stbtt_fontinfo *, unsigned char *, int, int, int, float, float, int);
void stbtt_MakeGlyphBitmapSubpixel(const stbtt_fontinfo *, unsigned char *, int, int, int, float, float, float, float, int);
void stbtt_GetGlyphBitmapBox(const stbtt_fontinfo *, int, float, float, int *, int *, int *, int *);
void stbtt_GetGlyphBitmapBoxSubpixel(const stbtt_fontinfo *, int, float, float, float, float, int *, int *, int *, int *);

typedef struct {
	int w, h, stride;
	unsigned char * pixels;
} stbtt__bitmap;

void stbtt_Rasterize(stbtt__bitmap *, float, stbtt_vertex *, int, float, float, float, float, int, int, int, void *);
int stbtt_FindMatchingFont(const unsigned char *, const char *, int);
int stbtt_CompareUTF8toUTF16_bigendian(const char *, int, const char *, int);
const char * stbtt_GetFontNameString(const stbtt_fontinfo *, int *, int, int, int, int);

enum {
	STBTT_PLATFORM_ID_UNICODE = 0,
	STBTT_PLATFORM_ID_MAC = 1,
	STBTT_PLATFORM_ID_ISO = 2,
	STBTT_PLATFORM_ID_MICROSOFT = 3,
};

enum {
	STBTT_UNICODE_EID_UNICODE_1_0 = 0,
	STBTT_UNICODE_EID_UNICODE_1_1 = 1,
	STBTT_UNICODE_EID_ISO_10646 = 2,
	STBTT_UNICODE_EID_UNICODE_2_0_BMP = 3,
	STBTT_UNICODE_EID_UNICODE_2_0_FULL = 4,
};

enum {
	STBTT_MS_EID_SYMBOL = 0,
	STBTT_MS_EID_UNICODE_BMP = 1,
	STBTT_MS_EID_SHIFTJIS = 2,
	STBTT_MS_EID_UNICODE_FULL = 10,
};

enum {
	STBTT_MAC_EID_ROMAN = 0, STBTT_MAC_EID_ARABIC = 4,
	STBTT_MAC_EID_JAPANESE = 1, STBTT_MAC_EID_HEBREW = 5,
	STBTT_MAC_EID_CHINESE_TRAD = 2, STBTT_MAC_EID_GREEK = 6,
	STBTT_MAC_EID_KOREAN = 3, STBTT_MAC_EID_RUSSIAN = 7,
};

enum {
	STBTT_MS_LANG_ENGLISH = 0x0409, STBTT_MS_LANG_ITALIAN = 0x0410,
	STBTT_MS_LANG_CHINESE = 0x0804, STBTT_MS_LANG_JAPANESE = 0x0411,
	STBTT_MS_LANG_DUTCH = 0x0413, STBTT_MS_LANG_KOREAN = 0x0412,
	STBTT_MS_LANG_FRENCH = 0x040c, STBTT_MS_LANG_RUSSIAN = 0x0419,
	STBTT_MS_LANG_GERMAN = 0x0407, STBTT_MS_LANG_SPANISH = 0x0409,
	STBTT_MS_LANG_HEBREW = 0x040d, STBTT_MS_LANG_SWEDISH = 0x041D,
};

enum {
	STBTT_MAC_LANG_ENGLISH = 0, STBTT_MAC_LANG_JAPANESE = 11,
	STBTT_MAC_LANG_ARABIC = 12, STBTT_MAC_LANG_KOREAN = 23,
	STBTT_MAC_LANG_DUTCH = 4, STBTT_MAC_LANG_RUSSIAN = 32,
	STBTT_MAC_LANG_FRENCH = 1, STBTT_MAC_LANG_SPANISH = 6,
	STBTT_MAC_LANG_GERMAN = 2, STBTT_MAC_LANG_SWEDISH = 5,
	STBTT_MAC_LANG_HEBREW = 10, STBTT_MAC_LANG_CHINESE_SIMPLIFIED = 33,
	STBTT_MAC_LANG_ITALIAN = 3, STBTT_MAC_LANG_CHINESE_TRAD = 19,
};

#endif

#ifdef STB_TRUETYPE_IMPLEMENTATION

#define STBTT_MAX_OVERSAMPLE (XS_RANDOM_SIZE % 2 == 0 ? 8 : 4)

stbtt_uint8 stbtt__buf_get8(stbtt__buf * b)
{
	if (b->cursor >= b->size) return 0;
	return b->data[b->cursor++];
};

stbtt_uint8 stbtt__buf_peek8(stbtt__buf * b)
{
	if (b->cursor >= b->size) return 0;
	return b->data[b->cursor];
};

void stbtt__buf_seek(stbtt__buf * b, int o)
{
	b->cursor = (o > b->size || o < 0) ? b->size : o;
};

void stbtt__buf_skip(stbtt__buf * b, int o)
{
	stbtt__buf_seek(b, b->cursor + o);
};

stbtt_uint32 stbtt__buf_get(stbtt__buf * b, int n)
{
	stbtt_uint32 v = 0;
	static int i = 0;
	for (i = 0; i < n; i++) v = (v << 8) | stbtt__buf_get8(b);
	return v;
};

stbtt__buf stbtt__new_buf(const void * p, size_t size)
{
	stbtt__buf r;
	r.data = (stbtt_uint8 *)p;
	r.size = (int)size;
	r.cursor = 0;
	return r;
};

#define stbtt__buf_get16(b) stbtt__buf_get((b), 2)
#define stbtt__buf_get32(b) stbtt__buf_get((b), 4)

stbtt__buf stbtt__buf_range(const stbtt__buf * b, int o, int s)
{
	stbtt__buf r = stbtt__new_buf(NULL, 0);
	if (o < 0 || s < 0 || o > b->size || s > b->size - o) return r;
	r.data = b->data + o;
	r.size = s;
	return r;
};

stbtt__buf stbtt__cff_get_index(stbtt__buf * b)
{
	int count, start, offsize;
	start = b->cursor;
	count = stbtt__buf_get16(b);
	if (count) {
		offsize = stbtt__buf_get8(b);
		stbtt__buf_skip(b, offsize * count);
		stbtt__buf_skip(b, stbtt__buf_get(b, offsize) - 1);
	};
	return stbtt__buf_range(b, start, b->cursor - start);
};

stbtt_uint32 stbtt__cff_int(stbtt__buf * b)
{
	int b0 = stbtt__buf_get8(b);
	if (b0 >= 32 && b0 <= 246) return b0 - 139;
	else if (b0 >= 247 && b0 <= 250) return (b0 - 247) * 256 + stbtt__buf_get8(b) + 108;
	else if (b0 >= 251 && b0 <= 254) return -(b0 - 251) * 256 - stbtt__buf_get8(b) - 108;
	else if (b0 == 28) return stbtt__buf_get16(b);
	else if (b0 == 29) return stbtt__buf_get32(b);
	return 0;
};

void stbtt__cff_skip_operand(stbtt__buf * b) {
	int v, b0 = stbtt__buf_peek8(b);
	if (b0 == 30) {
		stbtt__buf_skip(b, 1);
		while (b->cursor < b->size) {
			v = stbtt__buf_get8(b);
			if ((v & 0xF) == 0xF || (v >> 4) == 0xF)
				break;
		};
	}
	else stbtt__cff_int(b);
};

stbtt__buf stbtt__dict_get(stbtt__buf * b, int key)
{
	stbtt__buf_seek(b, 0);
	while (b->cursor < b->size) {
		int start = b->cursor, end, op;
		while (stbtt__buf_peek8(b) >= 28) stbtt__cff_skip_operand(b);
		end = b->cursor;
		op = stbtt__buf_get8(b);
		if (op == 12) op = stbtt__buf_get8(b) | 0x100;
		if (op == key) return stbtt__buf_range(b, start, end - start);
	};
	return stbtt__buf_range(b, 0, 0);
};

void stbtt__dict_get_ints(stbtt__buf * b, int key, int outcount, stbtt_uint32 * out)
{
	static int i = 0;
	stbtt__buf operands = stbtt__dict_get(b, key);
	for (i = 0; i < outcount && operands.cursor < operands.size; i++) out[i] = stbtt__cff_int(&operands);
};

int stbtt__cff_index_count(stbtt__buf * b)
{
	stbtt__buf_seek(b, 0);
	return stbtt__buf_get16(b);
};

stbtt__buf stbtt__cff_index_get(stbtt__buf b, int i)
{
	static int count, offsize, start, end;
	stbtt__buf_seek(&b, 0);
	count = stbtt__buf_get16(&b);
	offsize = stbtt__buf_get8(&b);
	stbtt__buf_skip(&b, i * offsize);
	start = stbtt__buf_get(&b, offsize);
	end = stbtt__buf_get(&b, offsize);
	return stbtt__buf_range(&b, 2 + (count + 1)*offsize + start, end - start);
};

#define ttBYTE(p) (* (stbtt_uint8 *) (p))
#define ttCHAR(p) (* (stbtt_int8 *) (p))

FORCEINLINE stbtt_uint16 ttUSHORT(stbtt_uint8 * p) { return p[0] * 256 + p[1]; };
FORCEINLINE stbtt_int16 ttSHORT(stbtt_uint8 * p) { return p[0] * 256 + p[1]; };
FORCEINLINE stbtt_uint32 ttULONG(stbtt_uint8 * p) { return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3]; };
FORCEINLINE stbtt_int32 ttLONG(stbtt_uint8 * p) { return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3]; };

#define stbtt_tag4(p, c0, c1, c2, c3) ((p)[0] == (c0) && (p)[1] == (c1) && (p)[2] == (c2) && (p)[3] == (c3))
#define stbtt_tag(p, str) stbtt_tag4(p, str[0], str[1], str[2], str[3])

stbtt_uint32 stbtt__find_table(stbtt_uint8 * data, stbtt_uint32 fontstart, const char * tag)
{
	stbtt_int32 num_tables = ttUSHORT(data + fontstart + 4);
	stbtt_uint32 tabledir = fontstart + 12;
	static stbtt_int32 i = 0;
	for (i = 0; i < num_tables; ++i) {
		stbtt_uint32 loc = tabledir + 16 * i;
		if (stbtt_tag(data + loc + 0, tag)) return ttULONG(data + loc + 8);
	};
	return 0;
};

stbtt__buf stbtt__get_subrs(stbtt__buf cff, stbtt__buf fontdict)
{
	stbtt_uint32 subrsoff = 0, private_loc[2] = { 0, 0 };
	stbtt__buf pdict;
	stbtt__dict_get_ints(&fontdict, 18, 2, private_loc);
	if (!private_loc[1] || !private_loc[0]) return stbtt__new_buf(NULL, 0);
	pdict = stbtt__buf_range(&cff, private_loc[1], private_loc[0]);
	stbtt__dict_get_ints(&pdict, 19, 1, &subrsoff);
	if (!subrsoff) return stbtt__new_buf(NULL, 0);
	stbtt__buf_seek(&cff, private_loc[1] + subrsoff);
	return stbtt__cff_get_index(&cff);
};

int stbtt_InitFont_internal(stbtt_fontinfo * info, unsigned char * data, int fontstart)
{
	static stbtt_uint32 cmap, t;
	static stbtt_int32 i = 0, numTables = 0;
	info->data = data;
	info->fontstart = fontstart;
	info->cff = stbtt__new_buf(NULL, 0);
	cmap = stbtt__find_table(data, fontstart, XorString("cmap"));
	info->loca = stbtt__find_table(data, fontstart, XorString("loca"));
	info->head = stbtt__find_table(data, fontstart, XorString("head"));
	info->glyf = stbtt__find_table(data, fontstart, XorString("glyf"));
	info->hhea = stbtt__find_table(data, fontstart, XorString("hhea"));
	info->hmtx = stbtt__find_table(data, fontstart, XorString("hmtx"));
	if (!cmap || !info->head || !info->hhea || !info->hmtx) return 0;
	if (info->glyf) {
		if (!info->loca) return 0;
	}
	else {
		stbtt__buf b, topdict, topdictidx;
		stbtt_uint32 cstype = 2, charstrings = 0, fdarrayoff = 0, fdselectoff = 0;
		stbtt_uint32 cff;
		cff = stbtt__find_table(data, fontstart, XorString("CFF "));
		if (!cff) return 0;
		info->fontdicts = stbtt__new_buf(NULL, 0);
		info->fdselect = stbtt__new_buf(NULL, 0);
		info->cff = stbtt__new_buf(data + cff, XS_RANDOM_SIZE * 1024 * 1024);
		b = info->cff;
		stbtt__buf_skip(&b, 2);
		stbtt__buf_seek(&b, stbtt__buf_get8(&b));
		stbtt__cff_get_index(&b);
		topdictidx = stbtt__cff_get_index(&b);
		topdict = stbtt__cff_index_get(topdictidx, 0);
		stbtt__cff_get_index(&b);
		info->gsubrs = stbtt__cff_get_index(&b);
		stbtt__dict_get_ints(&topdict, 17, 1, &charstrings);
		stbtt__dict_get_ints(&topdict, 0x100 | 6, 1, &cstype);
		stbtt__dict_get_ints(&topdict, 0x100 | 36, 1, &fdarrayoff);
		stbtt__dict_get_ints(&topdict, 0x100 | 37, 1, &fdselectoff);
		info->subrs = stbtt__get_subrs(b, topdict);
		if (cstype != 2 || charstrings == 0) return 0;
		if (fdarrayoff) {
			if (!fdselectoff) return 0;
			stbtt__buf_seek(&b, fdarrayoff);
			info->fontdicts = stbtt__cff_get_index(&b);
			info->fdselect = stbtt__buf_range(&b, fdselectoff, b.size - fdselectoff);
		};
		stbtt__buf_seek(&b, charstrings);
		info->charstrings = stbtt__cff_get_index(&b);
	};
	t = stbtt__find_table(data, fontstart, XorString("maxp"));
	if (t) info->numGlyphs = ttUSHORT(data + t + 4);
	else info->numGlyphs = 0x0000FFFF;
	numTables = ttUSHORT(data + cmap + 2);
	info->index_map = 0;
	for (i = 0; i < numTables; ++i) {
		stbtt_uint32 encoding_record = cmap + 4 + 8 * i;
		switch (ttUSHORT(data + encoding_record)) {
		case STBTT_PLATFORM_ID_MICROSOFT:
			switch (ttUSHORT(data + encoding_record + 2)) {
			case STBTT_MS_EID_UNICODE_BMP: case STBTT_MS_EID_UNICODE_FULL:
				info->index_map = cmap + ttULONG(data + encoding_record + 4);
				break;
			}
			break;
		case STBTT_PLATFORM_ID_UNICODE:
			info->index_map = cmap + ttULONG(data + encoding_record + 4);
			break;
		};
	};
	if (info->index_map == 0) return 0;
	info->indexToLocFormat = ttUSHORT(data + info->head + 50);
	return 1;
};

int stbtt_FindGlyphIndex(const stbtt_fontinfo * info, int unicode_codepoint)
{
	stbtt_uint8 * data = info->data;
	stbtt_uint32 index_map = info->index_map;
	stbtt_uint16 format = ttUSHORT(data + index_map + 0);
	if (format == 0) {
		stbtt_int32 bytes = ttUSHORT(data + index_map + 2);
		if (unicode_codepoint < bytes - 6) return ttBYTE(data + index_map + 6 + unicode_codepoint);
		return 0;
	}
	else if (format == 6) {
		stbtt_uint32 first = ttUSHORT(data + index_map + 6);
		stbtt_uint32 count = ttUSHORT(data + index_map + 8);
		if ((stbtt_uint32)unicode_codepoint >= first && (stbtt_uint32)unicode_codepoint < first + count) return ttUSHORT(data + index_map + 10 + (unicode_codepoint - first) * 2);
		return 0;
	}
	else if (format == 2) return 0;
	else if (format == 4) {
		stbtt_uint16 segcount = ttUSHORT(data + index_map + 6) >> 1;
		stbtt_uint16 searchRange = ttUSHORT(data + index_map + 8) >> 1;
		stbtt_uint16 entrySelector = ttUSHORT(data + index_map + 10);
		stbtt_uint16 rangeShift = ttUSHORT(data + index_map + 12) >> 1;
		stbtt_uint32 endCount = index_map + 14;
		stbtt_uint32 search = endCount;
		if (unicode_codepoint > 0xffff) return 0;
		if (unicode_codepoint >= ttUSHORT(data + search + rangeShift * 2)) search += rangeShift * 2;
		search -= 2;
		while (entrySelector) {
			stbtt_uint16 end;
			searchRange >>= 1;
			end = ttUSHORT(data + search + searchRange * 2);
			if (unicode_codepoint > end) search += searchRange * 2;
			--entrySelector;
		}
		search += 2;
		{
			stbtt_uint16 offset, start;
			stbtt_uint16 item = (stbtt_uint16)((search - endCount) >> 1);
			start = ttUSHORT(data + index_map + 14 + segcount * 2 + 2 + 2 * item);
			if (unicode_codepoint < start) return 0;
			offset = ttUSHORT(data + index_map + 14 + segcount * 6 + 2 + 2 * item);
			if (offset == 0) return (stbtt_uint16)(unicode_codepoint + ttSHORT(data + index_map + 14 + segcount * 4 + 2 + 2 * item));
			return ttUSHORT(data + offset + (unicode_codepoint - start) * 2 + index_map + 14 + segcount * 6 + 2 + 2 * item);
		}
	}
	else if (format == 12 || format == 13) {
		stbtt_uint32 ngroups = ttULONG(data + index_map + 12);
		stbtt_int32 low, high;
		low = 0; high = (stbtt_int32)ngroups;
		while (low < high) {
			stbtt_int32 mid = low + ((high - low) >> 1);
			stbtt_uint32 start_char = ttULONG(data + index_map + 16 + mid * 12);
			stbtt_uint32 end_char = ttULONG(data + index_map + 16 + mid * 12 + 4);
			if ((stbtt_uint32)unicode_codepoint < start_char) high = mid;
			else if ((stbtt_uint32)unicode_codepoint > end_char) low = mid + 1;
			else {
				stbtt_uint32 start_glyph = ttULONG(data + index_map + 16 + mid * 12 + 8);
				if (format == 12) return start_glyph + unicode_codepoint - start_char;
				return start_glyph;
			};
		};
	};
	return 0;
};

int stbtt_GetCodepointShape(const stbtt_fontinfo * info, int unicode_codepoint, stbtt_vertex ** vertices)
{
	return stbtt_GetGlyphShape(info, stbtt_FindGlyphIndex(info, unicode_codepoint), vertices);
};

void stbtt_setvertex(stbtt_vertex * v, stbtt_uint8 type, stbtt_int32 x, stbtt_int32 y, stbtt_int32 cx, stbtt_int32 cy)
{
	v->type = type;
	v->x = (stbtt_int16)x;
	v->y = (stbtt_int16)y;
	v->cx = (stbtt_int16)cx;
	v->cy = (stbtt_int16)cy;
};

int stbtt__GetGlyfOffset(const stbtt_fontinfo * info, int glyph_index)
{
	static int g1, g2;
	if (glyph_index >= info->numGlyphs || info->indexToLocFormat >= 2) return -1;
	if (info->indexToLocFormat == 0) {
		g1 = info->glyf + ttUSHORT(info->data + info->loca + glyph_index * 2) * 2;
		g2 = info->glyf + ttUSHORT(info->data + info->loca + glyph_index * 2 + 2) * 2;
	}
	else {
		g1 = info->glyf + ttULONG(info->data + info->loca + glyph_index * 4);
		g2 = info->glyf + ttULONG(info->data + info->loca + glyph_index * 4 + 4);
	};
	return g1 == g2 ? -1 : g1;
};

int stbtt__GetGlyphInfoT2(const stbtt_fontinfo *, int, int *, int *, int *, int *);

int stbtt_GetGlyphBox(const stbtt_fontinfo * info, int glyph_index, int * x0, int * y0, int * x1, int * y1)
{
	if (info->cff.size) {
		stbtt__GetGlyphInfoT2(info, glyph_index, x0, y0, x1, y1);
	}
	else {
		int g = stbtt__GetGlyfOffset(info, glyph_index);
		if (g < 0) return 0;
		if (x0) *x0 = ttSHORT(info->data + g + 2);
		if (y0) *y0 = ttSHORT(info->data + g + 4);
		if (x1) *x1 = ttSHORT(info->data + g + 6);
		if (y1) *y1 = ttSHORT(info->data + g + 8);
	}
	return 1;
}

int stbtt_GetCodepointBox(const stbtt_fontinfo * info, int codepoint, int *x0, int *y0, int *x1, int *y1)
{
	return stbtt_GetGlyphBox(info, stbtt_FindGlyphIndex(info, codepoint), x0, y0, x1, y1);
};

bool stbtt_IsGlyphEmpty(const stbtt_fontinfo *info, int glyph_index)
{
	if (info->cff.size) return stbtt__GetGlyphInfoT2(info, glyph_index, NULL, NULL, NULL, NULL) == 0;
	static int g = 0;
	static stbtt_int16 numberOfContours = 0;
	g = stbtt__GetGlyfOffset(info, glyph_index);
	if (g < 0) return 1;
	numberOfContours = ttSHORT(info->data + g);
	return numberOfContours == 0;
};

int stbtt__close_shape(stbtt_vertex *vertices, int num_vertices, int was_off, int start_off, stbtt_int32 sx, stbtt_int32 sy, stbtt_int32 scx, stbtt_int32 scy, stbtt_int32 cx, stbtt_int32 cy)
{
	if (start_off) {
		if (was_off) stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, (cx + scx) >> 1, (cy + scy) >> 1, cx, cy);
		stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, sx, sy, scx, scy);
	}
	else {
		if (was_off) stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, sx, sy, cx, cy);
		else stbtt_setvertex(&vertices[num_vertices++], STBTT_vline, sx, sy, 0, 0);
	}
	return num_vertices;
}

int stbtt__GetGlyphShapeTT(const stbtt_fontinfo *info, int glyph_index, stbtt_vertex **pvertices)
{
	stbtt_int16 numberOfContours;
	stbtt_uint8 *endPtsOfContours;
	stbtt_uint8 *data = info->data;
	stbtt_vertex *vertices = 0;
	int num_vertices = 0;
	int g = stbtt__GetGlyfOffset(info, glyph_index);
	*pvertices = NULL;
	if (g < 0) return 0;
	numberOfContours = ttSHORT(data + g);
	if (numberOfContours > 0) {
		stbtt_uint8 flags = 0, flagcount;
		stbtt_int32 ins, i, j = 0, m, n, next_move, was_off = 0, off, start_off = 0;
		stbtt_int32 x, y, cx, cy, sx, sy, scx, scy;
		stbtt_uint8 *points;
		endPtsOfContours = (data + g + 10);
		ins = ttUSHORT(data + g + 10 + numberOfContours * 2);
		points = data + g + 10 + numberOfContours * 2 + 2 + ins;
		n = 1 + ttUSHORT(endPtsOfContours + numberOfContours * 2 - 2);
		m = n + 2 * numberOfContours;
		vertices = (stbtt_vertex *)malloc(m * sizeof(vertices[0]));
		if (vertices == 0) return 0;
		next_move = 0;
		flagcount = 0;
		off = m - n;
		for (i = 0; i < n; ++i) {
			if (flagcount == 0) {
				flags = *points++;
				if (flags & 8)
					flagcount = *points++;
			}
			else --flagcount;
			vertices[off + i].type = flags;
		}
		x = 0;
		for (i = 0; i < n; ++i) {
			flags = vertices[off + i].type;
			if (flags & 2) {
				stbtt_int16 dx = *points++;
				x += (flags & 16) ? dx : -dx;
			}
			else {
				if (!(flags & 16)) {
					x = x + (stbtt_int16)(points[0] * 256 + points[1]);
					points += 2;
				}
			}
			vertices[off + i].x = (stbtt_int16)x;
		}
		y = 0;
		for (i = 0; i < n; ++i) {
			flags = vertices[off + i].type;
			if (flags & 4) {
				stbtt_int16 dy = *points++;
				y += (flags & 32) ? dy : -dy;
			}
			else {
				if (!(flags & 32)) {
					y = y + (stbtt_int16)(points[0] * 256 + points[1]);
					points += 2;
				}
			}
			vertices[off + i].y = (stbtt_int16)y;
		}
		num_vertices = 0;
		sx = sy = cx = cy = scx = scy = 0;
		for (i = 0; i < n; ++i) {
			flags = vertices[off + i].type;
			x = (stbtt_int16)vertices[off + i].x;
			y = (stbtt_int16)vertices[off + i].y;
			if (next_move == i) {
				if (i != 0) num_vertices = stbtt__close_shape(vertices, num_vertices, was_off, start_off, sx, sy, scx, scy, cx, cy);
				start_off = !(flags & 1);
				if (start_off) {
					scx = x;
					scy = y;
					if (!(vertices[off + i + 1].type & 1)) {
						sx = (x + (stbtt_int32)vertices[off + i + 1].x) >> 1;
						sy = (y + (stbtt_int32)vertices[off + i + 1].y) >> 1;
					}
					else {
						sx = (stbtt_int32)vertices[off + i + 1].x;
						sy = (stbtt_int32)vertices[off + i + 1].y;
						++i;
					}
				}
				else {
					sx = x, sy = y;
				}
				stbtt_setvertex(&vertices[num_vertices++], STBTT_vmove, sx, sy, 0, 0);
				was_off = 0;
				next_move = 1 + ttUSHORT(endPtsOfContours + j * 2);
				++j;
			}
			else {
				if (!(flags & 1)) {
					if (was_off) stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, (cx + x) >> 1, (cy + y) >> 1, cx, cy);
					cx = x;
					cy = y;
					was_off = 1;
				}
				else {
					if (was_off) stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, x, y, cx, cy);
					else stbtt_setvertex(&vertices[num_vertices++], STBTT_vline, x, y, 0, 0);
					was_off = 0;
				}
			}
		}
		num_vertices = stbtt__close_shape(vertices, num_vertices, was_off, start_off, sx, sy, scx, scy, cx, cy);
	}
	else if (numberOfContours == -1) {
		int more = 1;
		stbtt_uint8 *comp = data + g + 10;
		num_vertices = 0;
		vertices = 0;
		while (more) {
			stbtt_uint16 flags, gidx;
			int comp_num_verts = 0, i;
			stbtt_vertex *comp_verts = 0, *tmp = 0;
			float mtx[6] = { 1,0,0,1,0,0 }, m, n;
			flags = ttSHORT(comp); comp += 2;
			gidx = ttSHORT(comp); comp += 2;
			if (flags & 2) {
				if (flags & 1) {
					mtx[4] = ttSHORT(comp); comp += 2;
					mtx[5] = ttSHORT(comp); comp += 2;
				}
				else {
					mtx[4] = ttCHAR(comp); comp += 1;
					mtx[5] = ttCHAR(comp); comp += 1;
				}
			}
			if (flags & (1 << 3)) {
				mtx[0] = mtx[3] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[1] = mtx[2] = 0;
			}
			else if (flags & (1 << 6)) {
				mtx[0] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[1] = mtx[2] = 0;
				mtx[3] = ttSHORT(comp) / 16384.0f; comp += 2;
			}
			else if (flags & (1 << 7)) {
				mtx[0] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[1] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[2] = ttSHORT(comp) / 16384.0f; comp += 2;
				mtx[3] = ttSHORT(comp) / 16384.0f; comp += 2;
			}
			m = (float)sqrtf(mtx[0] * mtx[0] + mtx[1] * mtx[1]);
			n = (float)sqrtf(mtx[2] * mtx[2] + mtx[3] * mtx[3]);
			comp_num_verts = stbtt_GetGlyphShape(info, gidx, &comp_verts);
			if (comp_num_verts > 0) {
				for (i = 0; i < comp_num_verts; ++i) {
					stbtt_vertex* v = &comp_verts[i];
					short x, y;
					x = v->x; y = v->y;
					v->x = (short)(m * (mtx[0] * x + mtx[2] * y + mtx[4]));
					v->y = (short)(n * (mtx[1] * x + mtx[3] * y + mtx[5]));
					x = v->cx; y = v->cy;
					v->cx = (short)(m * (mtx[0] * x + mtx[2] * y + mtx[4]));
					v->cy = (short)(n * (mtx[1] * x + mtx[3] * y + mtx[5]));
				}
				tmp = (stbtt_vertex *)malloc((num_vertices + comp_num_verts) * sizeof(stbtt_vertex));
				if (!tmp) {
					if (vertices) free(vertices);
					if (comp_verts) free(comp_verts);
					return 0;
				}
				if (num_vertices > 0) CopyMemory(tmp, vertices, num_vertices * sizeof(stbtt_vertex));
				CopyMemory(tmp + num_vertices, comp_verts, comp_num_verts * sizeof(stbtt_vertex));
				if (vertices) free(vertices);
				vertices = tmp;
				free(comp_verts);
				num_vertices += comp_num_verts;
			}
			more = flags & (1 << 5);
		}
	}
	*pvertices = vertices;
	return num_vertices;
};

typedef struct {
	int bounds;
	int started;
	float first_x, first_y;
	float x, y;
	stbtt_int32 min_x, max_x, min_y, max_y;
	stbtt_vertex * pvertices;
	int num_vertices;
} stbtt__csctx;

#define STBTT__CSCTX_INIT(bounds) { bounds, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0 }

void stbtt__track_vertex(stbtt__csctx *c, stbtt_int32 x, stbtt_int32 y)
{
	if (x > c->max_x || !c->started) c->max_x = x;
	if (y > c->max_y || !c->started) c->max_y = y;
	if (x < c->min_x || !c->started) c->min_x = x;
	if (y < c->min_y || !c->started) c->min_y = y;
	c->started = 1;
};

void stbtt__csctx_v(stbtt__csctx *c, stbtt_uint8 type, stbtt_int32 x, stbtt_int32 y, stbtt_int32 cx, stbtt_int32 cy, stbtt_int32 cx1, stbtt_int32 cy1)
{
	if (c->bounds) {
		stbtt__track_vertex(c, x, y);
		if (type == STBTT_vcubic) {
			stbtt__track_vertex(c, cx, cy);
			stbtt__track_vertex(c, cx1, cy1);
		}
	}
	else {
		stbtt_setvertex(&c->pvertices[c->num_vertices], type, x, y, cx, cy);
		c->pvertices[c->num_vertices].cx1 = (stbtt_int16)cx1;
		c->pvertices[c->num_vertices].cy1 = (stbtt_int16)cy1;
	}
	c->num_vertices++;
};

void stbtt__csctx_close_shape(stbtt__csctx *ctx)
{
	if (ctx->first_x != ctx->x || ctx->first_y != ctx->y)
		stbtt__csctx_v(ctx, STBTT_vline, (int)ctx->first_x, (int)ctx->first_y, 0, 0, 0, 0);
};

void stbtt__csctx_rmove_to(stbtt__csctx *ctx, float dx, float dy)
{
	stbtt__csctx_close_shape(ctx);
	ctx->first_x = ctx->x = ctx->x + dx;
	ctx->first_y = ctx->y = ctx->y + dy;
	stbtt__csctx_v(ctx, STBTT_vmove, (int)ctx->x, (int)ctx->y, 0, 0, 0, 0);
};

void stbtt__csctx_rline_to(stbtt__csctx *ctx, float dx, float dy)
{
	ctx->x += dx;
	ctx->y += dy;
	stbtt__csctx_v(ctx, STBTT_vline, (int)ctx->x, (int)ctx->y, 0, 0, 0, 0);
};

void stbtt__csctx_rccurve_to(stbtt__csctx *ctx, float dx1, float dy1, float dx2, float dy2, float dx3, float dy3)
{
	float cx1 = ctx->x + dx1;
	float cy1 = ctx->y + dy1;
	float cx2 = cx1 + dx2;
	float cy2 = cy1 + dy2;
	ctx->x = cx2 + dx3;
	ctx->y = cy2 + dy3;
	stbtt__csctx_v(ctx, STBTT_vcubic, (int)ctx->x, (int)ctx->y, (int)cx1, (int)cy1, (int)cx2, (int)cy2);
};

stbtt__buf stbtt__get_subr(stbtt__buf idx, int n)
{
	int count = stbtt__cff_index_count(&idx);
	int bias = 107;
	if (count >= 33900) bias = 32768;
	else if (count >= 1240) bias = 1131;
	n += bias;
	if (n < 0 || n >= count) return stbtt__new_buf(NULL, 0);
	return stbtt__cff_index_get(idx, n);
};

stbtt__buf stbtt__cid_get_glyph_subrs(const stbtt_fontinfo *info, int glyph_index)
{
	stbtt__buf fdselect = info->fdselect;
	int nranges, start, end, v, fmt, fdselector = -1, i;
	stbtt__buf_seek(&fdselect, 0);
	fmt = stbtt__buf_get8(&fdselect);
	if (fmt == 0) {
		stbtt__buf_skip(&fdselect, glyph_index);
		fdselector = stbtt__buf_get8(&fdselect);
	}
	else if (fmt == 3) {
		nranges = stbtt__buf_get16(&fdselect);
		start = stbtt__buf_get16(&fdselect);
		for (i = 0; i < nranges; i++) {
			v = stbtt__buf_get8(&fdselect);
			end = stbtt__buf_get16(&fdselect);
			if (glyph_index >= start && glyph_index < end) {
				fdselector = v;
				break;
			}
			start = end;
		}
	}
	if (fdselector == -1) stbtt__new_buf(NULL, 0);
	return stbtt__get_subrs(info->cff, stbtt__cff_index_get(info->fontdicts, fdselector));
}

int stbtt__run_charstring(const stbtt_fontinfo *info, int glyph_index, stbtt__csctx *c)
{
	int in_header = 1, maskbits = 0, subr_stack_height = 0, sp = 0, v, i, b0;
	int has_subrs = 0, clear_stack;
	float s[XS_RANDOM_SIZE / 2];
	stbtt__buf subr_stack[XS_RANDOM_SIZE / 10], subrs = info->subrs, b;
	float f;
	b = stbtt__cff_index_get(info->charstrings, glyph_index);
	while (b.cursor < b.size) {
		i = 0;
		clear_stack = 1;
		b0 = stbtt__buf_get8(&b);
		switch (b0) {
		case 0x13:
		case 0x14:
			if (in_header) maskbits += (sp / 2);
			in_header = 0;
			stbtt__buf_skip(&b, (maskbits + 7) / 8);
			break;
		case 0x01:
		case 0x03:
		case 0x12:
		case 0x17:
			maskbits += (sp / 2);
			break;
		case 0x15:
			in_header = 0;
			if (sp < 2) return 0;
			stbtt__csctx_rmove_to(c, s[sp - 2], s[sp - 1]);
			break;
		case 0x04:
			in_header = 0;
			if (sp < 1) return 0;
			stbtt__csctx_rmove_to(c, 0, s[sp - 1]);
			break;
		case 0x16:
			in_header = 0;
			if (sp < 1) return 0;
			stbtt__csctx_rmove_to(c, s[sp - 1], 0);
			break;
		case 0x05:
			if (sp < 2) return 0;
			for (; i + 1 < sp; i += 2)
				stbtt__csctx_rline_to(c, s[i], s[i + 1]);
			break;
		case 0x07:
			if (sp < 1) return 0;
			goto vlineto;
		case 0x06:
			if (sp < 1) return 0;
			for (;;) {
				if (i >= sp) break;
				stbtt__csctx_rline_to(c, s[i], 0);
				i++;
			vlineto:
				if (i >= sp) break;
				stbtt__csctx_rline_to(c, 0, s[i]);
				i++;
			}
			break;
		case 0x1F:
			if (sp < 4) return 0;
			goto hvcurveto;
		case 0x1E:
			if (sp < 4) return 0;
			for (;;) {
				if (i + 3 >= sp) break;
				stbtt__csctx_rccurve_to(c, 0, s[i], s[i + 1], s[i + 2], s[i + 3], (sp - i == 5) ? s[i + 4] : 0.0f);
				i += 4;
			hvcurveto:
				if (i + 3 >= sp) break;
				stbtt__csctx_rccurve_to(c, s[i], 0, s[i + 1], s[i + 2], (sp - i == 5) ? s[i + 4] : 0.0f, s[i + 3]);
				i += 4;
			}
			break;
		case 0x08:
			if (sp < 6) return 0;
			for (; i + 5 < sp; i += 6)
				stbtt__csctx_rccurve_to(c, s[i], s[i + 1], s[i + 2], s[i + 3], s[i + 4], s[i + 5]);
			break;
		case 0x18:
			if (sp < 8) return 0;
			for (; i + 5 < sp - 2; i += 6)
				stbtt__csctx_rccurve_to(c, s[i], s[i + 1], s[i + 2], s[i + 3], s[i + 4], s[i + 5]);
			if (i + 1 >= sp) return 0;
			stbtt__csctx_rline_to(c, s[i], s[i + 1]);
			break;
		case 0x19:
			if (sp < 8) return 0;
			for (; i + 1 < sp - 6; i += 2)
				stbtt__csctx_rline_to(c, s[i], s[i + 1]);
			if (i + 5 >= sp) return 0;
			stbtt__csctx_rccurve_to(c, s[i], s[i + 1], s[i + 2], s[i + 3], s[i + 4], s[i + 5]);
			break;
		case 0x1A:
		case 0x1B:
			if (sp < 4) return 0;
			f = 0.0;
			if (sp & 1) { f = s[i]; i++; }
			for (; i + 3 < sp; i += 4) {
				if (b0 == 0x1B)
					stbtt__csctx_rccurve_to(c, s[i], f, s[i + 1], s[i + 2], s[i + 3], 0.0);
				else
					stbtt__csctx_rccurve_to(c, f, s[i], s[i + 1], s[i + 2], 0.0, s[i + 3]);
				f = 0.0;
			}
			break;
		case 0x0A:
			if (!has_subrs) {
				if (info->fdselect.size)
					subrs = stbtt__cid_get_glyph_subrs(info, glyph_index);
				has_subrs = 1;
			}
		case 0x1D:
			if (sp < 1) return 0;
			v = (int)s[--sp];
			if (subr_stack_height >= 10) return 0;
			subr_stack[subr_stack_height++] = b;
			b = stbtt__get_subr(b0 == 0x0A ? subrs : info->gsubrs, v);
			if (b.size == 0) return 0;
			b.cursor = 0;
			clear_stack = 0;
			break;
		case 0x0B:
			if (subr_stack_height <= 0) return 0;
			b = subr_stack[--subr_stack_height];
			clear_stack = 0;
			break;
		case 0x0E:
			stbtt__csctx_close_shape(c);
			return 1;
		case 0x0C: {
			float dx1, dx2, dx3, dx4, dx5, dx6, dy1, dy2, dy3, dy4, dy5, dy6;
			float dx, dy;
			int b1 = stbtt__buf_get8(&b);
			switch (b1) {
			case 0x22:
				if (sp < 7) return 0;
				dx1 = s[0];
				dx2 = s[1];
				dy2 = s[2];
				dx3 = s[3];
				dx4 = s[4];
				dx5 = s[5];
				dx6 = s[6];
				stbtt__csctx_rccurve_to(c, dx1, 0, dx2, dy2, dx3, 0);
				stbtt__csctx_rccurve_to(c, dx4, 0, dx5, -dy2, dx6, 0);
				break;
			case 0x23:
				if (sp < 13) return 0;
				dx1 = s[0];
				dy1 = s[1];
				dx2 = s[2];
				dy2 = s[3];
				dx3 = s[4];
				dy3 = s[5];
				dx4 = s[6];
				dy4 = s[7];
				dx5 = s[8];
				dy5 = s[9];
				dx6 = s[10];
				dy6 = s[11];
				stbtt__csctx_rccurve_to(c, dx1, dy1, dx2, dy2, dx3, dy3);
				stbtt__csctx_rccurve_to(c, dx4, dy4, dx5, dy5, dx6, dy6);
				break;
			case 0x24:
				if (sp < 9) return 0;
				dx1 = s[0];
				dy1 = s[1];
				dx2 = s[2];
				dy2 = s[3];
				dx3 = s[4];
				dx4 = s[5];
				dx5 = s[6];
				dy5 = s[7];
				dx6 = s[8];
				stbtt__csctx_rccurve_to(c, dx1, dy1, dx2, dy2, dx3, 0);
				stbtt__csctx_rccurve_to(c, dx4, 0, dx5, dy5, dx6, -(dy1 + dy2 + dy5));
				break;
			case 0x25:
				if (sp < 11) return 0;
				dx1 = s[0];
				dy1 = s[1];
				dx2 = s[2];
				dy2 = s[3];
				dx3 = s[4];
				dy3 = s[5];
				dx4 = s[6];
				dy4 = s[7];
				dx5 = s[8];
				dy5 = s[9];
				dx6 = dy6 = s[10];
				dx = dx1 + dx2 + dx3 + dx4 + dx5;
				dy = dy1 + dy2 + dy3 + dy4 + dy5;
				if (fabsf(dx) > fabsf(dy)) dy6 = -dy;
				else dx6 = -dx;
				stbtt__csctx_rccurve_to(c, dx1, dy1, dx2, dy2, dx3, dy3);
				stbtt__csctx_rccurve_to(c, dx4, dy4, dx5, dy5, dx6, dy6);
				break;
			default:
				return 0;
			}
		} break;
		default:
			if (b0 != 255 && b0 != 28 && (b0 < 32 || b0 > 254)) return 0;
			if (b0 == 255) {
				f = (float)stbtt__buf_get32(&b) / 0x10000;
			}
			else {
				stbtt__buf_skip(&b, -1);
				f = (float)(stbtt_int16)stbtt__cff_int(&b);
			}
			if (sp >= 48) return 0;
			s[sp++] = f;
			clear_stack = 0;
			break;
		}
		if (clear_stack) sp = 0;
	};
	return 0;
};

int stbtt__GetGlyphShapeT2(const stbtt_fontinfo *info, int glyph_index, stbtt_vertex **pvertices)
{
	stbtt__csctx count_ctx = STBTT__CSCTX_INIT(1);
	stbtt__csctx output_ctx = STBTT__CSCTX_INIT(0);
	if (stbtt__run_charstring(info, glyph_index, &count_ctx)) {
		*pvertices = (stbtt_vertex*)malloc(count_ctx.num_vertices * sizeof(stbtt_vertex));
		output_ctx.pvertices = *pvertices;
		if (stbtt__run_charstring(info, glyph_index, &output_ctx)) {
			return output_ctx.num_vertices;
		};
	};
	*pvertices = NULL;
	return 0;
};

int stbtt__GetGlyphInfoT2(const stbtt_fontinfo *info, int glyph_index, int *x0, int *y0, int *x1, int *y1)
{
	stbtt__csctx c = STBTT__CSCTX_INIT(1);
	int r = stbtt__run_charstring(info, glyph_index, &c);
	if (x0) {
		*x0 = r ? c.min_x : 0;
		*y0 = r ? c.min_y : 0;
		*x1 = r ? c.max_x : 0;
		*y1 = r ? c.max_y : 0;
	};
	return r ? c.num_vertices : 0;
};

int stbtt_GetGlyphShape(const stbtt_fontinfo *info, int glyph_index, stbtt_vertex **pvertices)
{
	if (!info->cff.size) return stbtt__GetGlyphShapeTT(info, glyph_index, pvertices);
	return stbtt__GetGlyphShapeT2(info, glyph_index, pvertices);
}

void stbtt_GetGlyphHMetrics(const stbtt_fontinfo *info, int glyph_index, int *advanceWidth, int *leftSideBearing)
{
	stbtt_uint16 numOfLongHorMetrics = ttUSHORT(info->data + info->hhea + 34);
	if (glyph_index < numOfLongHorMetrics) {
		if (advanceWidth) *advanceWidth = ttSHORT(info->data + info->hmtx + 4 * glyph_index);
		if (leftSideBearing) *leftSideBearing = ttSHORT(info->data + info->hmtx + 4 * glyph_index + 2);
	}
	else {
		if (advanceWidth) *advanceWidth = ttSHORT(info->data + info->hmtx + 4 * (numOfLongHorMetrics - 1));
		if (leftSideBearing) *leftSideBearing = ttSHORT(info->data + info->hmtx + 4 * numOfLongHorMetrics + 2 * (glyph_index - numOfLongHorMetrics));
	};
};

void stbtt_GetCodepointHMetrics(const stbtt_fontinfo *info, int codepoint, int *advanceWidth, int *leftSideBearing)
{
	stbtt_GetGlyphHMetrics(info, stbtt_FindGlyphIndex(info, codepoint), advanceWidth, leftSideBearing);
};

void stbtt_GetFontVMetrics(const stbtt_fontinfo *info, int *ascent, int *descent, int *lineGap)
{
	if (ascent) *ascent = ttSHORT(info->data + info->hhea + 4);
	if (descent) *descent = ttSHORT(info->data + info->hhea + 6);
	if (lineGap) *lineGap = ttSHORT(info->data + info->hhea + 8);
};

void stbtt_GetFontBoundingBox(const stbtt_fontinfo *info, int *x0, int *y0, int *x1, int *y1)
{
	*x0 = ttSHORT(info->data + info->head + 36);
	*y0 = ttSHORT(info->data + info->head + 38);
	*x1 = ttSHORT(info->data + info->head + 40);
	*y1 = ttSHORT(info->data + info->head + 42);
};

float stbtt_ScaleForPixelHeight(const stbtt_fontinfo *info, float height)
{
	int fheight = ttSHORT(info->data + info->hhea + 4) - ttSHORT(info->data + info->hhea + 6);
	return (float)height / fheight;
};

float stbtt_ScaleForMappingEmToPixels(const stbtt_fontinfo *info, float pixels)
{
	int unitsPerEm = ttUSHORT(info->data + info->head + 18);
	return pixels / unitsPerEm;
};

void stbtt_GetGlyphBitmapBoxSubpixel(const stbtt_fontinfo *font, int glyph, float scale_x, float scale_y, float shift_x, float shift_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
	int x0 = 0, y0 = 0, x1, y1;
	if (!stbtt_GetGlyphBox(font, glyph, &x0, &y0, &x1, &y1)) {
		if (ix0) *ix0 = 0;
		if (iy0) *iy0 = 0;
		if (ix1) *ix1 = 0;
		if (iy1) *iy1 = 0;
	}
	else {
		if (ix0) *ix0 = floorf(x0 * scale_x + shift_x);
		if (iy0) *iy0 = floorf(-y1 * scale_y + shift_y);
		if (ix1) *ix1 = ceilf(x1 * scale_x + shift_x);
		if (iy1) *iy1 = ceilf(-y0 * scale_y + shift_y);
	};
};

void stbtt_GetGlyphBitmapBox(const stbtt_fontinfo *font, int glyph, float scale_x, float scale_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
	stbtt_GetGlyphBitmapBoxSubpixel(font, glyph, scale_x, scale_y, 0.0f, 0.0f, ix0, iy0, ix1, iy1);
}

void stbtt_GetCodepointBitmapBoxSubpixel(const stbtt_fontinfo *font, int codepoint, float scale_x, float scale_y, float shift_x, float shift_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
	stbtt_GetGlyphBitmapBoxSubpixel(font, stbtt_FindGlyphIndex(font, codepoint), scale_x, scale_y, shift_x, shift_y, ix0, iy0, ix1, iy1);
}

void stbtt_GetCodepointBitmapBox(const stbtt_fontinfo *font, int codepoint, float scale_x, float scale_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
	stbtt_GetCodepointBitmapBoxSubpixel(font, codepoint, scale_x, scale_y, 0.0f, 0.0f, ix0, iy0, ix1, iy1);
}

typedef struct stbtt__hheap_chunk {
	struct stbtt__hheap_chunk * next;
} stbtt__hheap_chunk;

typedef struct stbtt__hheap {
	struct stbtt__hheap_chunk * head;
	void * first_free;
	int num_remaining_in_head_chunk;
} stbtt__hheap;

void * stbtt__hheap_alloc(stbtt__hheap *hh, size_t size, void *userdata)
{
	if (hh->first_free) {
		void *p = hh->first_free;
		hh->first_free = *(void **)p;
		return p;
	}
	else {
		if (hh->num_remaining_in_head_chunk == 0) {
			int count = (size < 32 ? 2000 : size < 128 ? 800 : 100);
			stbtt__hheap_chunk *c = (stbtt__hheap_chunk *)malloc(sizeof(stbtt__hheap_chunk) + size * count);
			if (c == NULL) return NULL;
			c->next = hh->head;
			hh->head = c;
			hh->num_remaining_in_head_chunk = count;
		}
		--hh->num_remaining_in_head_chunk;
		return (char *)(hh->head) + size * hh->num_remaining_in_head_chunk;
	};
};

void stbtt__hheap_free(stbtt__hheap *hh, void *p)
{
	*(void **)p = hh->first_free;
	hh->first_free = p;
}

void stbtt__hheap_cleanup(stbtt__hheap *hh, void *userdata)
{
	stbtt__hheap_chunk *c = hh->head;
	while (c) {
		stbtt__hheap_chunk *n = c->next;
		free(c), c = n;
	};
};

typedef struct stbtt__edge {
	float x0, y0, x1, y1;
	int invert;
} stbtt__edge;

typedef struct stbtt__active_edge
{
	struct stbtt__active_edge * next;
	float fx, fdx, fdy, direction, sy, ey;
} stbtt__active_edge;


stbtt__active_edge * stbtt__new_active(stbtt__hheap *hh, stbtt__edge *e, int off_x, float start_point, void *userdata)
{
	stbtt__active_edge *z = (stbtt__active_edge *)stbtt__hheap_alloc(hh, sizeof(*z), userdata);
	float dxdy = (e->x1 - e->x0) / (e->y1 - e->y0);
	if (!z) return z;
	z->fdx = dxdy;
	z->fdy = dxdy != 0.0f ? (1.0f / dxdy) : 0.0f;
	z->fx = e->x0 + dxdy * (start_point - e->y0);
	z->fx -= off_x;
	z->direction = e->invert ? 1.0f : -1.0f;
	z->sy = e->y0;
	z->ey = e->y1;
	z->next = 0;
	return z;
};

void stbtt__handle_clipped_edge(float *scanline, int x, stbtt__active_edge *e, float x0, float y0, float x1, float y1)
{
	if (y0 == y1 || y0 > e->ey || y1 < e->sy) return;
	if (y0 < e->sy) {
		x0 += (x1 - x0) * (e->sy - y0) / (y1 - y0);
		y0 = e->sy;
	};
	if (y1 > e->ey) {
		x1 += (x1 - x0) * (e->ey - y1) / (y1 - y0);
		y1 = e->ey;
	};
	if (x0 <= x && x1 <= x) scanline[x] += e->direction * (y1 - y0);
	else if (x0 >= x + 1 && x1 >= x + 1)
	{
	}
	else scanline[x] += e->direction * (y1 - y0) * (1 - ((x0 - x) + (x1 - x)) / 2);
};

void stbtt__fill_active_edges_new(float *scanline, float *scanline_fill, int len, stbtt__active_edge *e, float y_top)
{
	float y_bottom = y_top + 1;
	while (e) {
		if (e->fdx == 0) {
			float x0 = e->fx;
			if (x0 < len) {
				if (x0 >= 0) {
					stbtt__handle_clipped_edge(scanline, (int)x0, e, x0, y_top, x0, y_bottom);
					stbtt__handle_clipped_edge(scanline_fill - 1, (int)x0 + 1, e, x0, y_top, x0, y_bottom);
				}
				else {
					stbtt__handle_clipped_edge(scanline_fill - 1, 0, e, x0, y_top, x0, y_bottom);
				}
			}
		}
		else {
			float x0 = e->fx;
			float dx = e->fdx;
			float xb = x0 + dx;
			float x_top, x_bottom;
			float sy0, sy1;
			float dy = e->fdy;
			if (e->sy > y_top) {
				x_top = x0 + dx * (e->sy - y_top);
				sy0 = e->sy;
			}
			else {
				x_top = x0;
				sy0 = y_top;
			}
			if (e->ey < y_bottom) {
				x_bottom = x0 + dx * (e->ey - y_top);
				sy1 = e->ey;
			}
			else {
				x_bottom = xb;
				sy1 = y_bottom;
			}
			if (x_top >= 0 && x_bottom >= 0 && x_top < len && x_bottom < len) {
				if ((int)x_top == (int)x_bottom) {
					float height;
					int x = (int)x_top;
					height = sy1 - sy0;
					scanline[x] += e->direction * (1 - ((x_top - x) + (x_bottom - x)) / 2) * height;
					scanline_fill[x] += e->direction * height;
				}
				else {
					int x, x1, x2;
					float y_crossing, step, sign, area;
					if (x_top > x_bottom) {
						float t;
						sy0 = y_bottom - (sy0 - y_top);
						sy1 = y_bottom - (sy1 - y_top);
						t = sy0, sy0 = sy1, sy1 = t;
						t = x_bottom, x_bottom = x_top, x_top = t;
						dx = -dx;
						dy = -dy;
						t = x0, x0 = xb, xb = t;
					}
					x1 = (int)x_top;
					x2 = (int)x_bottom;
					y_crossing = (x1 + 1 - x0) * dy + y_top;
					sign = e->direction;
					area = sign * (y_crossing - sy0);
					scanline[x1] += area * (1 - ((x_top - x1) + (x1 + 1 - x1)) / 2);
					step = sign * dy;
					for (x = x1 + 1; x < x2; ++x) {
						scanline[x] += area + step / 2;
						area += step;
					}
					y_crossing += dy * (x2 - (x1 + 1));
					scanline[x2] += area + sign * (1 - ((x2 - x2) + (x_bottom - x2)) / 2) * (sy1 - y_crossing);
					scanline_fill[x2] += sign * (sy1 - sy0);
				}
			}
			else {
				int x;
				for (x = 0; x < len; ++x) {
					float y0 = y_top;
					float x1 = (float)(x);
					float x2 = (float)(x + 1);
					float x3 = xb;
					float y3 = y_bottom;
					float y1, y2;
					y1 = (x - x0) / dx + y_top, y2 = (x + 1 - x0) / dx + y_top;
					if (x0 < x1 && x3 > x2) {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x1, y1);
						stbtt__handle_clipped_edge(scanline, x, e, x1, y1, x2, y2);
						stbtt__handle_clipped_edge(scanline, x, e, x2, y2, x3, y3);
					}
					else if (x3 < x1 && x0 > x2) {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x2, y2);
						stbtt__handle_clipped_edge(scanline, x, e, x2, y2, x1, y1);
						stbtt__handle_clipped_edge(scanline, x, e, x1, y1, x3, y3);
					}
					else if (x0 < x1 && x3 > x1) {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x1, y1);
						stbtt__handle_clipped_edge(scanline, x, e, x1, y1, x3, y3);
					}
					else if (x3 < x1 && x0 > x1) {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x1, y1);
						stbtt__handle_clipped_edge(scanline, x, e, x1, y1, x3, y3);
					}
					else if (x0 < x2 && x3 > x2) {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x2, y2);
						stbtt__handle_clipped_edge(scanline, x, e, x2, y2, x3, y3);
					}
					else if (x3 < x2 && x0 > x2) {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x2, y2);
						stbtt__handle_clipped_edge(scanline, x, e, x2, y2, x3, y3);
					}
					else {
						stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x3, y3);
					};
				};
			};
		};
		e = e->next;
	};
};

void stbtt__rasterize_sorted_edges(stbtt__bitmap *result, stbtt__edge *e, int n, int vsubsample, int off_x, int off_y, void *userdata)
{
	stbtt__hheap hh = { 0, 0, 0 };
	stbtt__active_edge *active = NULL;
	int y, j = 0, i;
	float *scanline, *scanline2;
	scanline = (float *)malloc((result->w * 2 + 1) * sizeof(float));
	scanline2 = scanline + result->w;
	y = off_y;
	e[n].y0 = (float)(off_y + result->h) + 1;
	while (j < result->h) {
		float scan_y_top = y + 0.0f;
		float scan_y_bottom = y + 1.0f;
		stbtt__active_edge **step = &active;
		memset(scanline, 0, result->w * sizeof(scanline[0])), memset(scanline2, 0, (result->w + 1) * sizeof(scanline[0]));
		while (*step) {
			stbtt__active_edge * z = *step;
			if (z->ey <= scan_y_top) {
				*step = z->next;
				z->direction = 0;
				stbtt__hheap_free(&hh, z);
			}
			else {
				step = &((*step)->next);
			}
		}
		while (e->y0 <= scan_y_bottom) {
			if (e->y0 != e->y1) {
				stbtt__active_edge *z = stbtt__new_active(&hh, e, off_x, scan_y_top, userdata);
				if (z != NULL) {
					z->next = active;
					active = z;
				}
			}
			++e;
		}
		if (active) stbtt__fill_active_edges_new(scanline, scanline2 + 1, result->w, active, scan_y_top);
		{
			float sum = 0;
			for (i = 0; i < result->w; ++i) {
				float k;
				int m;
				sum += scanline2[i];
				k = scanline[i] + sum;
				k = (float)fabsf(k) * 255 + 0.5f;
				m = (int)k;
				if (m > 255) m = 255;
				result->pixels[j*result->stride + i] = (unsigned char)m;
			};
		};
		step = &active;
		while (*step) {
			stbtt__active_edge *z = *step;
			z->fx += z->fdx;
			step = &((*step)->next);
		};
		++y, ++j;
	};
	stbtt__hheap_cleanup(&hh, userdata), free(scanline);
};

#define STBTT__COMPARE(a, b) ((a)->y0 < (b)->y0)

void stbtt__sort_edges_ins_sort(stbtt__edge *p, int n)
{
	int i, j;
	for (i = 1; i < n; ++i) {
		stbtt__edge t = p[i], *a = &t;
		j = i;
		while (j > 0) {
			stbtt__edge *b = &p[j - 1];
			int c = STBTT__COMPARE(a, b);
			if (!c) break;
			p[j] = p[j - 1], --j;
		};
		if (i != j) p[j] = t;
	};
};

void stbtt__sort_edges_quicksort(stbtt__edge *p, int n)
{
	while (n > 12) {
		stbtt__edge t;
		int c01, c12, c, m = n >> 1, i, j;
		c01 = STBTT__COMPARE(&p[0], &p[m]);
		c12 = STBTT__COMPARE(&p[m], &p[n - 1]);
		if (c01 != c12) {
			int z;
			c = STBTT__COMPARE(&p[0], &p[n - 1]), z = (c == c12) ? 0 : n - 1, t = p[z], p[z] = p[m], p[m] = t;
		};
		t = p[0], p[0] = p[m], p[m] = t, i = 1, j = n - 1;
		for (;;) {
			for (;; ++i) {
				if (!STBTT__COMPARE(&p[i], &p[0])) break;
			}
			for (;; --j) {
				if (!STBTT__COMPARE(&p[0], &p[j])) break;
			}
			if (i >= j) break;
			t = p[i], p[i] = p[j], p[j] = t, ++i, --j;
		} if (j < (n - i)) {
			stbtt__sort_edges_quicksort(p, j);
			p = p + i, n = n - i;
		}
		else {
			stbtt__sort_edges_quicksort(p + i, n - i), n = j;
		};
	};
};

void stbtt__sort_edges(stbtt__edge *p, int n)
{
	stbtt__sort_edges_quicksort(p, n);
	stbtt__sort_edges_ins_sort(p, n);
}

typedef struct {
	float x, y;
} stbtt__point;

void stbtt__rasterize(stbtt__bitmap *result, stbtt__point *pts, int *wcount, int windings, float scale_x, float scale_y, float shift_x, float shift_y, int off_x, int off_y, int invert, void *userdata)
{
	float y_scale_inv = invert ? -scale_y : scale_y;
	stbtt__edge *e;
	int n, i, j, k, m;
	int vsubsample = 1;
	n = 0;
	for (i = 0; i < windings; ++i) n += wcount[i];
	e = (stbtt__edge *)malloc(sizeof(*e) * (n + 1));
	if (e == 0) return;
	n = 0, m = 0;
	for (i = 0; i < windings; ++i) {
		stbtt__point *p = pts + m;
		m += wcount[i];
		j = wcount[i] - 1;
		for (k = 0; k < wcount[i]; j = k++) {
			int a = k, b = j;
			if (p[j].y == p[k].y) continue;
			e[n].invert = 0;
			if (invert ? p[j].y > p[k].y : p[j].y < p[k].y) {
				e[n].invert = 1;
				a = j, b = k;
			}
			e[n].x0 = p[a].x * scale_x + shift_x;
			e[n].y0 = (p[a].y * y_scale_inv + shift_y) * vsubsample;
			e[n].x1 = p[b].x * scale_x + shift_x;
			e[n].y1 = (p[b].y * y_scale_inv + shift_y) * vsubsample;
			++n;
		}
	}
	stbtt__sort_edges(e, n), stbtt__rasterize_sorted_edges(result, e, n, vsubsample, off_x, off_y, userdata), free(e);
}

void stbtt__add_point(stbtt__point *points, int n, float x, float y)
{
	if (!points) return;
	points[n].x = x;
	points[n].y = y;
};

int stbtt__tesselate_curve(stbtt__point *points, int *num_points, float x0, float y0, float x1, float y1, float x2, float y2, float objspace_flatness_squared, int n)
{
	float mx = (x0 + 2 * x1 + x2) / 4;
	float my = (y0 + 2 * y1 + y2) / 4;
	float dx = (x0 + x2) / 2 - mx;
	float dy = (y0 + y2) / 2 - my;
	if (n > 16) return 1;
	if (dx*dx + dy * dy > objspace_flatness_squared) {
		stbtt__tesselate_curve(points, num_points, x0, y0, (x0 + x1) / 2.0f, (y0 + y1) / 2.0f, mx, my, objspace_flatness_squared, n + 1);
		stbtt__tesselate_curve(points, num_points, mx, my, (x1 + x2) / 2.0f, (y1 + y2) / 2.0f, x2, y2, objspace_flatness_squared, n + 1);
	}
	else {
		stbtt__add_point(points, *num_points, x2, y2);
		*num_points = *num_points + 1;
	}
	return 1;
}

void stbtt__tesselate_cubic(stbtt__point *points, int *num_points, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float objspace_flatness_squared, int n)
{
	float dx0 = x1 - x0;
	float dy0 = y1 - y0;
	float dx1 = x2 - x1;
	float dy1 = y2 - y1;
	float dx2 = x3 - x2;
	float dy2 = y3 - y2;
	float dx = x3 - x0;
	float dy = y3 - y0;
	float longlen = (float)(sqrtf(dx0*dx0 + dy0 * dy0) + sqrtf(dx1*dx1 + dy1 * dy1) + sqrtf(dx2*dx2 + dy2 * dy2));
	float shortlen = (float)sqrtf(dx*dx + dy * dy);
	float flatness_squared = longlen * longlen - shortlen * shortlen;
	if (n > 16) return;
	if (flatness_squared > objspace_flatness_squared) {
		float x01 = (x0 + x1) / 2;
		float y01 = (y0 + y1) / 2;
		float x12 = (x1 + x2) / 2;
		float y12 = (y1 + y2) / 2;
		float x23 = (x2 + x3) / 2;
		float y23 = (y2 + y3) / 2;
		float xa = (x01 + x12) / 2;
		float ya = (y01 + y12) / 2;
		float xb = (x12 + x23) / 2;
		float yb = (y12 + y23) / 2;
		float mx = (xa + xb) / 2;
		float my = (ya + yb) / 2;
		stbtt__tesselate_cubic(points, num_points, x0, y0, x01, y01, xa, ya, mx, my, objspace_flatness_squared, n + 1);
		stbtt__tesselate_cubic(points, num_points, mx, my, xb, yb, x23, y23, x3, y3, objspace_flatness_squared, n + 1);
	}
	else {
		stbtt__add_point(points, *num_points, x3, y3);
		*num_points = *num_points + 1;
	};
};

stbtt__point *stbtt_FlattenCurves(stbtt_vertex *vertices, int num_verts, float objspace_flatness, int **contour_lengths, int *num_contours, void *userdata)
{
	stbtt__point *points = 0;
	int num_points = 0;
	float objspace_flatness_squared = objspace_flatness * objspace_flatness;
	int i, n = 0, start = 0, pass;
	for (i = 0; i < num_verts; ++i)
		if (vertices[i].type == STBTT_vmove) ++n;
	*num_contours = n;
	if (n == 0) return 0;
	*contour_lengths = (int *)malloc(sizeof(**contour_lengths) * n);
	if (*contour_lengths == 0) {
		*num_contours = 0;
		return 0;
	}
	for (pass = 0; pass < 2; ++pass) {
		float x = 0, y = 0;
		if (pass == 1) {
			points = (stbtt__point *)malloc(num_points * sizeof(points[0]));
			if (points == NULL) goto error;
		}
		num_points = 0, n = -1;
		for (i = 0; i < num_verts; ++i) {
			switch (vertices[i].type) {
			case STBTT_vmove:
				if (n >= 0) (*contour_lengths)[n] = num_points - start;
				++n, start = num_points;
				x = vertices[i].x, y = vertices[i].y;
				stbtt__add_point(points, num_points++, x, y);
				break;
			case STBTT_vline:
				x = vertices[i].x, y = vertices[i].y;
				stbtt__add_point(points, num_points++, x, y);
				break;
			case STBTT_vcurve:
				stbtt__tesselate_curve(points, &num_points, x, y, vertices[i].cx, vertices[i].cy, vertices[i].x, vertices[i].y, objspace_flatness_squared, 0);
				x = vertices[i].x, y = vertices[i].y;
				break;
			case STBTT_vcubic:
				stbtt__tesselate_cubic(points, &num_points, x, y, vertices[i].cx, vertices[i].cy, vertices[i].cx1, vertices[i].cy1, vertices[i].x, vertices[i].y, objspace_flatness_squared, 0);
				x = vertices[i].x, y = vertices[i].y;
				break;
			}
		}
		(*contour_lengths)[n] = num_points - start;
	}
	return points;
error:
	free(points);
	free(*contour_lengths);
	*contour_lengths = 0;
	*num_contours = 0;
	return NULL;
}

void stbtt_Rasterize(stbtt__bitmap *result, float flatness_in_pixels, stbtt_vertex *vertices, int num_verts, float scale_x, float scale_y, float shift_x, float shift_y, int x_off, int y_off, int invert, void *userdata)
{
	float scale = scale_x > scale_y ? scale_y : scale_x;
	int winding_count, *winding_lengths;
	stbtt__point *windings = stbtt_FlattenCurves(vertices, num_verts, flatness_in_pixels / scale, &winding_lengths, &winding_count, userdata);
	if (windings) {
		stbtt__rasterize(result, windings, winding_lengths, winding_count, scale_x, scale_y, shift_x, shift_y, x_off, y_off, invert, userdata);
		free(winding_lengths);
		free(windings);
	};
};

unsigned char * stbtt_GetGlyphBitmapSubpixel(const stbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int glyph, int *width, int *height, int *xoff, int *yoff)
{
	int ix0, iy0, ix1, iy1;
	stbtt__bitmap gbm;
	stbtt_vertex *vertices;
	int num_verts = stbtt_GetGlyphShape(info, glyph, &vertices);
	if (scale_x == 0) scale_x = scale_y;
	if (scale_y == 0) {
		if (scale_x == 0) {
			free(vertices);
			return NULL;
		}
		scale_y = scale_x;
	}
	stbtt_GetGlyphBitmapBoxSubpixel(info, glyph, scale_x, scale_y, shift_x, shift_y, &ix0, &iy0, &ix1, &iy1);
	gbm.w = (ix1 - ix0);
	gbm.h = (iy1 - iy0);
	gbm.pixels = NULL;
	if (width) *width = gbm.w;
	if (height) *height = gbm.h;
	if (xoff) *xoff = ix0;
	if (yoff) *yoff = iy0;
	if (gbm.w && gbm.h) {
		gbm.pixels = (unsigned char *)malloc(gbm.w * gbm.h);
		if (gbm.pixels) {
			gbm.stride = gbm.w;
			stbtt_Rasterize(&gbm, 0.35f, vertices, num_verts, scale_x, scale_y, shift_x, shift_y, ix0, iy0, 1, info->userdata);
		}
	}
	free(vertices);
	return gbm.pixels;
};

unsigned char *stbtt_GetGlyphBitmap(const stbtt_fontinfo *info, float scale_x, float scale_y, int glyph, int *width, int *height, int *xoff, int *yoff)
{
	return stbtt_GetGlyphBitmapSubpixel(info, scale_x, scale_y, 0.0f, 0.0f, glyph, width, height, xoff, yoff);
};

void stbtt_MakeGlyphBitmapSubpixel(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int glyph)
{
	int ix0, iy0;
	stbtt_vertex *vertices;
	int num_verts = stbtt_GetGlyphShape(info, glyph, &vertices);
	stbtt__bitmap gbm;
	stbtt_GetGlyphBitmapBoxSubpixel(info, glyph, scale_x, scale_y, shift_x, shift_y, &ix0, &iy0, 0, 0);
	gbm.pixels = output;
	gbm.w = out_w;
	gbm.h = out_h;
	gbm.stride = out_stride;
	if (gbm.w && gbm.h) stbtt_Rasterize(&gbm, 0.35f, vertices, num_verts, scale_x, scale_y, shift_x, shift_y, ix0, iy0, 1, info->userdata);
	free(vertices);
};

void stbtt_MakeGlyphBitmap(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int glyph)
{
	stbtt_MakeGlyphBitmapSubpixel(info, output, out_w, out_h, out_stride, scale_x, scale_y, 0.0f, 0.0f, glyph);
};

unsigned char *stbtt_GetCodepointBitmapSubpixel(const stbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint, int *width, int *height, int *xoff, int *yoff)
{
	return stbtt_GetGlyphBitmapSubpixel(info, scale_x, scale_y, shift_x, shift_y, stbtt_FindGlyphIndex(info, codepoint), width, height, xoff, yoff);
};

void stbtt_MakeCodepointBitmapSubpixel(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint)
{
	stbtt_MakeGlyphBitmapSubpixel(info, output, out_w, out_h, out_stride, scale_x, scale_y, shift_x, shift_y, stbtt_FindGlyphIndex(info, codepoint));
};

unsigned char *stbtt_GetCodepointBitmap(const stbtt_fontinfo *info, float scale_x, float scale_y, int codepoint, int *width, int *height, int *xoff, int *yoff)
{
	return stbtt_GetCodepointBitmapSubpixel(info, scale_x, scale_y, 0.0f, 0.0f, codepoint, width, height, xoff, yoff);
};

void stbtt_MakeCodepointBitmap(const stbtt_fontinfo *info, unsigned char *output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int codepoint)
{
	stbtt_MakeCodepointBitmapSubpixel(info, output, out_w, out_h, out_stride, scale_x, scale_y, 0.0f, 0.0f, codepoint);
};

int stbtt_BakeFontBitmap_internal(unsigned char *data, int offset, float pixel_height, unsigned char *pixels, int pw, int ph, int first_char, int num_chars, stbtt_bakedchar *chardata)
{
	float scale;
	int x, y, bottom_y, i;
	stbtt_fontinfo f;
	f.userdata = NULL;
	if (!stbtt_InitFont(&f, data, offset)) return -1;
	memset(pixels, 0, pw*ph);
	x = y = 1;
	bottom_y = 1;
	scale = stbtt_ScaleForPixelHeight(&f, pixel_height);
	for (i = 0; i < num_chars; ++i) {
		int advance, lsb, x0, y0, x1, y1, gw, gh;
		int g = stbtt_FindGlyphIndex(&f, first_char + i);
		stbtt_GetGlyphHMetrics(&f, g, &advance, &lsb);
		stbtt_GetGlyphBitmapBox(&f, g, scale, scale, &x0, &y0, &x1, &y1);
		gw = x1 - x0;
		gh = y1 - y0;
		if (x + gw + 1 >= pw) y = bottom_y, x = 1;
		if (y + gh + 1 >= ph) return -i;
		stbtt_MakeGlyphBitmap(&f, pixels + x + y * pw, gw, gh, pw, scale, scale, g);
		chardata[i].x0 = (stbtt_int16)x;
		chardata[i].y0 = (stbtt_int16)y;
		chardata[i].x1 = (stbtt_int16)(x + gw);
		chardata[i].y1 = (stbtt_int16)(y + gh);
		chardata[i].xadvance = scale * advance;
		chardata[i].xoff = (float)x0;
		chardata[i].yoff = (float)y0;
		x = x + gw + 1;
		if (y + gh + 1 > bottom_y) bottom_y = y + gh + 1;
	}
	return bottom_y;
}

void stbtt_GetBakedQuad(stbtt_bakedchar *chardata, int pw, int ph, int char_index, float *xpos, float *ypos, stbtt_aligned_quad *q, int opengl_fillrule)
{
	float d3d_bias = opengl_fillrule ? 0 : -0.5f;
	float ipw = 1.0f / pw, iph = 1.0f / ph;
	stbtt_bakedchar *b = chardata + char_index;
	int round_x = floorf((*xpos + b->xoff) + 0.5f);
	int round_y = floorf((*ypos + b->yoff) + 0.5f);
	q->x0 = round_x + d3d_bias;
	q->y0 = round_y + d3d_bias;
	q->x1 = round_x + b->x1 - b->x0 + d3d_bias;
	q->y1 = round_y + b->y1 - b->y0 + d3d_bias;
	q->s0 = b->x0 * ipw;
	q->t0 = b->y0 * iph;
	q->s1 = b->x1 * ipw;
	q->t1 = b->y1 * iph;
	*xpos += b->xadvance;
}

int stbtt_PackBegin(stbtt_pack_context *spc, unsigned char *pixels, int pw, int ph, int stride_in_bytes, int padding, void *alloc_context)
{
	stbrp_context * context = (stbrp_context *)malloc(sizeof(*context));
	int num_nodes = pw - padding;
	stbrp_node * nodes = (stbrp_node *)malloc(sizeof(*nodes) * num_nodes);
	if (context == NULL || nodes == NULL) {
		if (context != NULL) free(context);
		if (nodes != NULL) free(nodes);
		return 0;
	}
	spc->user_allocator_context = alloc_context;
	spc->width = pw;
	spc->height = ph;
	spc->pixels = pixels;
	spc->pack_info = context;
	spc->nodes = nodes;
	spc->padding = padding;
	spc->stride_in_bytes = stride_in_bytes != 0 ? stride_in_bytes : pw;
	spc->h_oversample = 1;
	spc->v_oversample = 1;
	stbrp_init_target(context, pw - padding, ph - padding, nodes, num_nodes);
	if (pixels) memset(pixels, 0, pw*ph);
	return 1;
}

void stbtt_PackEnd(stbtt_pack_context * spc)
{
	free(spc->nodes), free(spc->pack_info);
}

void stbtt_PackSetOversampling(stbtt_pack_context *spc, unsigned int h_oversample, unsigned int v_oversample)
{
	if (h_oversample <= STBTT_MAX_OVERSAMPLE) spc->h_oversample = h_oversample;
	if (v_oversample <= STBTT_MAX_OVERSAMPLE) spc->v_oversample = v_oversample;
}

#define STBTT__OVER_MASK (STBTT_MAX_OVERSAMPLE - 1)

void stbtt__h_prefilter(unsigned char *pixels, int w, int h, int stride_in_bytes, unsigned int kernel_width)
{
	unsigned char buffer[STBTT_MAX_OVERSAMPLE];
	int safe_w = w - kernel_width, j;
	memset(buffer, 0, ARRAYSIZE(buffer));
	for (j = 0; j < h; ++j) {
		int i;
		unsigned int total = 0;
		memset(buffer, 0, kernel_width);
		switch (kernel_width) {
		case 2:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / 2);
			}
			break;
		case 3:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / 3);
			}
			break;
		case 4:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / 4);
			}
			break;
		case 5:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / 5);
			}
			break;
		default:
			for (i = 0; i <= safe_w; ++i) {
				total += pixels[i] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
				pixels[i] = (unsigned char)(total / kernel_width);
			}
			break;
		}
		for (; i < w; ++i) {
			total -= buffer[i & STBTT__OVER_MASK];
			pixels[i] = (unsigned char)(total / kernel_width);
		};
		pixels += stride_in_bytes;
	};
};

void stbtt__v_prefilter(unsigned char *pixels, int w, int h, int stride_in_bytes, unsigned int kernel_width)
{
	unsigned char buffer[STBTT_MAX_OVERSAMPLE];
	int safe_h = h - kernel_width, j;
	memset(buffer, 0, ARRAYSIZE(buffer));
	for (j = 0; j < w; ++j) {
		int i;
		unsigned int total = 0;
		memset(buffer, 0, kernel_width);
		switch (kernel_width) {
		case 2:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / 2);
			}
			break;
		case 3:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / 3);
			}
			break;
		case 4:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / 4);
			}
			break;
		case 5:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / 5);
			}
			break;
		default:
			for (i = 0; i <= safe_h; ++i) {
				total += pixels[i*stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
				buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i*stride_in_bytes];
				pixels[i*stride_in_bytes] = (unsigned char)(total / kernel_width);
			}
			break;
		}
		for (; i < h; ++i) {
			total -= buffer[i & STBTT__OVER_MASK];
			pixels[i*stride_in_bytes] = (unsigned char)(total / kernel_width);
		}
		pixels += 1;
	}
}

float stbtt__oversample_shift(int oversample)
{
	if (!oversample) return 0.f;
	return (float)-(oversample - 1) / (2.f * (float)oversample);
}

int stbtt_PackFontRangesGatherRects(stbtt_pack_context *spc, const stbtt_fontinfo *info, stbtt_pack_range *ranges, int num_ranges, stbrp_rect *rects)
{
	int i, j, k = 0;
	for (i = 0; i < num_ranges; ++i) {
		float fh = ranges[i].font_size;
		float scale = fh > 0 ? stbtt_ScaleForPixelHeight(info, fh) : stbtt_ScaleForMappingEmToPixels(info, -fh);
		ranges[i].h_oversample = (unsigned char)spc->h_oversample;
		ranges[i].v_oversample = (unsigned char)spc->v_oversample;
		for (j = 0; j < ranges[i].num_chars; ++j) {
			int x0, y0, x1, y1;
			int codepoint = ranges[i].array_of_unicode_codepoints == NULL ? ranges[i].first_unicode_codepoint_in_range + j : ranges[i].array_of_unicode_codepoints[j];
			int glyph = stbtt_FindGlyphIndex(info, codepoint);
			stbtt_GetGlyphBitmapBoxSubpixel(info, glyph, scale * spc->h_oversample, scale * spc->v_oversample, 0, 0, &x0, &y0, &x1, &y1);
			rects[k].w = (stbrp_coord)(x1 - x0 + spc->padding + spc->h_oversample - 1);
			rects[k].h = (stbrp_coord)(y1 - y0 + spc->padding + spc->v_oversample - 1);
			++k;
		};
	};
	return k;
};

int stbtt_PackFontRangesRenderIntoRects(stbtt_pack_context *spc, const stbtt_fontinfo *info, stbtt_pack_range *ranges, int num_ranges, stbrp_rect *rects)
{
	int i, j, k, return_value = 1;
	int old_h_over = spc->h_oversample;
	int old_v_over = spc->v_oversample;
	k = 0;
	for (i = 0; i < num_ranges; ++i) {
		float fh = ranges[i].font_size;
		float scale = fh > 0 ? stbtt_ScaleForPixelHeight(info, fh) : stbtt_ScaleForMappingEmToPixels(info, -fh);
		float recip_h, recip_v, sub_x, sub_y;
		spc->h_oversample = ranges[i].h_oversample;
		spc->v_oversample = ranges[i].v_oversample;
		recip_h = 1.0f / spc->h_oversample;
		recip_v = 1.0f / spc->v_oversample;
		sub_x = stbtt__oversample_shift(spc->h_oversample);
		sub_y = stbtt__oversample_shift(spc->v_oversample);
		for (j = 0; j < ranges[i].num_chars; ++j) {
			stbrp_rect *r = &rects[k];
			if (r->was_packed) {
				stbtt_packedchar *bc = &ranges[i].chardata_for_range[j];
				int advance, lsb, x0, y0, x1, y1;
				int codepoint = ranges[i].array_of_unicode_codepoints == NULL ? ranges[i].first_unicode_codepoint_in_range + j : ranges[i].array_of_unicode_codepoints[j];
				int glyph = stbtt_FindGlyphIndex(info, codepoint);
				stbrp_coord pad = (stbrp_coord)spc->padding;
				r->x += pad, r->y += pad, r->w -= pad, r->h -= pad;
				stbtt_GetGlyphHMetrics(info, glyph, &advance, &lsb);
				stbtt_GetGlyphBitmapBox(info, glyph, scale * spc->h_oversample, scale * spc->v_oversample, &x0, &y0, &x1, &y1);
				stbtt_MakeGlyphBitmapSubpixel(info, spc->pixels + r->x + r->y*spc->stride_in_bytes, r->w - spc->h_oversample + 1, r->h - spc->v_oversample + 1, spc->stride_in_bytes, scale * spc->h_oversample, scale * spc->v_oversample, 0, 0, glyph);
				if (spc->h_oversample > 1) stbtt__h_prefilter(spc->pixels + r->x + r->y*spc->stride_in_bytes, r->w, r->h, spc->stride_in_bytes, spc->h_oversample);
				if (spc->v_oversample > 1) stbtt__v_prefilter(spc->pixels + r->x + r->y*spc->stride_in_bytes, r->w, r->h, spc->stride_in_bytes, spc->v_oversample);
				bc->x0 = (stbtt_int16)r->x;
				bc->y0 = (stbtt_int16)r->y;
				bc->x1 = (stbtt_int16)(r->x + r->w);
				bc->y1 = (stbtt_int16)(r->y + r->h);
				bc->xadvance = scale * advance;
				bc->xoff = (float)x0 * recip_h + sub_x;
				bc->yoff = (float)y0 * recip_v + sub_y;
				bc->xoff2 = (x0 + r->w) * recip_h + sub_x;
				bc->yoff2 = (y0 + r->h) * recip_v + sub_y;
			}
			else {
				return_value = 0;
			}
			++k;
		};
	};
	spc->h_oversample = old_h_over, spc->v_oversample = old_v_over;
	return return_value;
};

void stbtt_PackFontRangesPackRects(stbtt_pack_context *spc, stbrp_rect *rects, int num_rects)
{
	stbrp_pack_rects((stbrp_context *)spc->pack_info, rects, num_rects);
}

int stbtt_PackFontRanges(stbtt_pack_context *spc, unsigned char *fontdata, int font_index, stbtt_pack_range *ranges, int num_ranges)
{
	stbtt_fontinfo info;
	int i, j, n, return_value = 1;
	stbrp_rect * rects;
	for (i = 0; i < num_ranges; ++i)
		for (j = 0; j < ranges[i].num_chars; ++j) ranges[i].chardata_for_range[j].x0 = ranges[i].chardata_for_range[j].y0 = ranges[i].chardata_for_range[j].x1 = ranges[i].chardata_for_range[j].y1 = 0;
	n = 0;
	for (i = 0; i < num_ranges; ++i) n += ranges[i].num_chars;
	rects = (stbrp_rect *)malloc(sizeof(*rects) * n);
	if (rects == NULL) return 0;
	info.userdata = spc->user_allocator_context;
	stbtt_InitFont(&info, fontdata, stbtt_GetFontOffsetForIndex(font_index));
	n = stbtt_PackFontRangesGatherRects(spc, &info, ranges, num_ranges, rects);
	stbtt_PackFontRangesPackRects(spc, rects, n);
	return_value = stbtt_PackFontRangesRenderIntoRects(spc, &info, ranges, num_ranges, rects);
	free(rects);
	return return_value;
}

int stbtt_PackFontRange(stbtt_pack_context *spc, unsigned char *fontdata, int font_index, float font_size, int first_unicode_codepoint_in_range, int num_chars_in_range, stbtt_packedchar *chardata_for_range)
{
	stbtt_pack_range range;
	range.first_unicode_codepoint_in_range = first_unicode_codepoint_in_range;
	range.array_of_unicode_codepoints = NULL;
	range.num_chars = num_chars_in_range;
	range.chardata_for_range = chardata_for_range;
	range.font_size = font_size;
	return stbtt_PackFontRanges(spc, fontdata, font_index, &range, 1);
}

void stbtt_GetPackedQuad(stbtt_packedchar *chardata, int pw, int ph, int char_index, float *xpos, float *ypos, stbtt_aligned_quad *q, int align_to_integer)
{
	float ipw = 1.0f / pw, iph = 1.0f / ph;
	stbtt_packedchar *b = chardata + char_index;
	if (align_to_integer) {
		float x = (float)floorf((*xpos + b->xoff) + 0.5f);
		float y = (float)floorf((*ypos + b->yoff) + 0.5f);
		q->x0 = x;
		q->y0 = y;
		q->x1 = x + b->xoff2 - b->xoff;
		q->y1 = y + b->yoff2 - b->yoff;
	}
	else {
		q->x0 = *xpos + b->xoff;
		q->y0 = *ypos + b->yoff;
		q->x1 = *xpos + b->xoff2;
		q->y1 = *ypos + b->yoff2;
	}
	q->s0 = b->x0 * ipw;
	q->t0 = b->y0 * iph;
	q->s1 = b->x1 * ipw;
	q->t1 = b->y1 * iph;
	*xpos += b->xadvance;
}

stbtt_int32 stbtt__CompareUTF8toUTF16_bigendian_prefix(stbtt_uint8 *s1, stbtt_int32 len1, stbtt_uint8 *s2, stbtt_int32 len2)
{
	stbtt_int32 i = 0;
	while (len2) {
		stbtt_uint16 ch = s2[0] * 256 + s2[1];
		if (ch < 0x80) {
			if (i >= len1) return -1;
			if (s1[i++] != ch) return -1;
		}
		else if (ch < 0x800) {
			if (i + 1 >= len1) return -1;
			if (s1[i++] != 0xc0 + (ch >> 6)) return -1;
			if (s1[i++] != 0x80 + (ch & 0x3f)) return -1;
		}
		else if (ch >= 0xd800 && ch < 0xdc00) {
			stbtt_uint32 c;
			stbtt_uint16 ch2 = s2[2] * 256 + s2[3];
			if (i + 3 >= len1) return -1;
			c = ((ch - 0xd800) << 10) + (ch2 - 0xdc00) + 0x10000;
			if (s1[i++] != 0xf0 + (c >> 18)) return -1;
			if (s1[i++] != 0x80 + ((c >> 12) & 0x3f)) return -1;
			if (s1[i++] != 0x80 + ((c >> 6) & 0x3f)) return -1;
			if (s1[i++] != 0x80 + ((c) & 0x3f)) return -1;
			s2 += 2, len2 -= 2;
		}
		else if (ch >= 0xdc00 && ch < 0xe000) {
			return -1;
		}
		else {
			if (i + 2 >= len1) return -1;
			if (s1[i++] != 0xe0 + (ch >> 12)) return -1;
			if (s1[i++] != 0x80 + ((ch >> 6) & 0x3f)) return -1;
			if (s1[i++] != 0x80 + ((ch) & 0x3f)) return -1;
		}
		s2 += 2, len2 -= 2;
	}
	return i;
}

int stbtt_CompareUTF8toUTF16_bigendian_internal(char *s1, int len1, char *s2, int len2)
{
	return len1 == stbtt__CompareUTF8toUTF16_bigendian_prefix((stbtt_uint8*)s1, len1, (stbtt_uint8*)s2, len2);
}

const char * stbtt_GetFontNameString(const stbtt_fontinfo *font, int *length, int platformID, int encodingID, int languageID, int nameID)
{
	stbtt_int32 i, count, stringOffset;
	stbtt_uint8 *fc = font->data;
	stbtt_uint32 offset = font->fontstart, nm = stbtt__find_table(fc, offset, XorString("name"));
	if (!nm) return NULL;
	count = ttUSHORT(fc + nm + 2);
	stringOffset = nm + ttUSHORT(fc + nm + 4);
	for (i = 0; i < count; ++i) {
		stbtt_uint32 loc = nm + 6 + 12 * i;
		if (platformID == ttUSHORT(fc + loc + 0) && encodingID == ttUSHORT(fc + loc + 2)
			&& languageID == ttUSHORT(fc + loc + 4) && nameID == ttUSHORT(fc + loc + 6)) {
			*length = ttUSHORT(fc + loc + 8);
			return (const char *)(fc + stringOffset + ttUSHORT(fc + loc + 10));
		}
	}
	return NULL;
}

int stbtt__matchpair(stbtt_uint8 *fc, stbtt_uint32 nm, stbtt_uint8 *name, stbtt_int32 nlen, stbtt_int32 target_id, stbtt_int32 next_id)
{
	stbtt_int32 i, count = ttUSHORT(fc + nm + 2), stringOffset = nm + ttUSHORT(fc + nm + 4);
	for (i = 0; i < count; ++i) {
		stbtt_uint32 loc = nm + 6 + 12 * i;
		stbtt_int32 id = ttUSHORT(fc + loc + 6);
		if (id == target_id) {
			stbtt_int32 platform = ttUSHORT(fc + loc + 0), encoding = ttUSHORT(fc + loc + 2), language = ttUSHORT(fc + loc + 4);
			if (platform == 0 || (platform == 3 && encoding == 1) || (platform == 3 && encoding == 10)) {
				stbtt_int32 slen = ttUSHORT(fc + loc + 8);
				stbtt_int32 off = ttUSHORT(fc + loc + 10);
				stbtt_int32 matchlen = stbtt__CompareUTF8toUTF16_bigendian_prefix(name, nlen, fc + stringOffset + off, slen);
				if (matchlen >= 0) {
					if (i + 1 < count && ttUSHORT(fc + loc + 12 + 6) == next_id && ttUSHORT(fc + loc + 12) == platform && ttUSHORT(fc + loc + 12 + 2) == encoding && ttUSHORT(fc + loc + 12 + 4) == language) {
						slen = ttUSHORT(fc + loc + 12 + 8);
						off = ttUSHORT(fc + loc + 12 + 10);
						if (slen == 0) {
							if (matchlen == nlen)
								return 1;
						}
						else if (matchlen < nlen && name[matchlen] == ' ') {
							++matchlen;
							if (stbtt_CompareUTF8toUTF16_bigendian_internal((char*)(name + matchlen), nlen - matchlen, (char*)(fc + stringOffset + off), slen)) return 1;
						}
					}
					else {
						if (matchlen == nlen) return 1;
					};
				};
			};
		};
	};
	return 0;
};

int stbtt__matches(stbtt_uint8 *fc, stbtt_uint32 offset, stbtt_uint8 *name, stbtt_int32 flags)
{
	stbtt_int32 nlen = (stbtt_int32)strlen((char *)name);
	stbtt_uint32 nm, hd;
	if (flags) {
		hd = stbtt__find_table(fc, offset, XorString("head"));
		if ((ttUSHORT(fc + hd + 44) & 7) != (flags & 7)) return 0;
	};
	nm = stbtt__find_table(fc, offset, XorString("name"));
	if (!nm) return 0;
	if (flags) {
		if (stbtt__matchpair(fc, nm, name, nlen, 16, -1)) return 1;
		if (stbtt__matchpair(fc, nm, name, nlen, 1, -1)) return 1;
		if (stbtt__matchpair(fc, nm, name, nlen, 3, -1)) return 1;
	}
	else {
		if (stbtt__matchpair(fc, nm, name, nlen, 16, 17)) return 1;
		if (stbtt__matchpair(fc, nm, name, nlen, 1, 2)) return 1;
		if (stbtt__matchpair(fc, nm, name, nlen, 3, -1)) return 1;
	};
	return 0;
};

int stbtt_FindMatchingFont_internal(unsigned char *font_collection, char *name_utf8, stbtt_int32 flags)
{
	stbtt_int32 i;
	for (i = 0;; ++i) {
		stbtt_int32 off = stbtt_GetFontOffsetForIndex(i);
		if (off < 0) return off;
		if (stbtt__matches((stbtt_uint8 *)font_collection, off, (stbtt_uint8*)name_utf8, flags)) return off;
	};
};

int stbtt_BakeFontBitmap(const unsigned char *data, int offset, float pixel_height, unsigned char *pixels, int pw, int ph, int first_char, int num_chars, stbtt_bakedchar *chardata)
{
	return stbtt_BakeFontBitmap_internal((unsigned char *)data, offset, pixel_height, pixels, pw, ph, first_char, num_chars, chardata);
}

int stbtt_GetFontOffsetForIndex(int index)
{
	return !index ? 0 : -1;
};

int stbtt_InitFont(stbtt_fontinfo *info, const unsigned char * data, int offset)
{
	return stbtt_InitFont_internal(info, (unsigned char *)data, offset);
}

int stbtt_FindMatchingFont(const unsigned char * fontdata, const char * name, int flags)
{
	return stbtt_FindMatchingFont_internal((unsigned char *)fontdata, (char *)name, flags);
}

int stbtt_CompareUTF8toUTF16_bigendian(const char * s1, int len1, const char * s2, int len2)
{
	return stbtt_CompareUTF8toUTF16_bigendian_internal((char *)s1, len1, (char *)s2, len2);
}

#endif
