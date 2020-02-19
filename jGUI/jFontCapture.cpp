#include "jFontCapture.h"
#include "jCacheMgr.h"

#include <ft2build.h>
#include FT_FREETYPE_H
FT_Library library;

#pragma comment(lib,"freetype.lib")

typedef struct Text_Dimensions_ {
	int            width;
	int            height;
	int            baseline;
	unsigned char *outpuffer;
} Text_Dimension;
typedef struct Mono_Glyph_ {
	int            top;
	int            height;
	int            width;
	int            descent;
	int            ascent;
	int            advance_width;
} Mono_Glyph;
int kerning_offset(wchar_t c, wchar_t pc, FT_Face face);
unsigned char* unpack_mono_bitmap(FT_Bitmap bitmap);
Mono_Glyph convert_glyph(FT_GlyphSlot slot);
Mono_Glyph get_glyph(wchar_t character, FT_Face face);
Text_Dimension text_dimension(wstring text, FT_Face face);
void draw_puffer(FT_Face face, int x, int y, Text_Dimension text_info);
void showBitmap(unsigned char* buf, int w, int h);

jFontCapture::jFontCapture()
{
}


jFontCapture::~jFontCapture()
{
}

void jFontCapture::Load(string fontFullname, int fontSize, wstring text_unicode)
{
	mImage = nullptr;
	mFontname = fontFullname;
	mFontsize = fontSize;
	mText = text_unicode;
	mKey = CalcKey();

	mImage = (jUIBitmap *)jCacheMgr::GetInst().Cache(mKey, [&]() {
		jUIBitmap *bitmap = new jUIBitmap();

		FT_Face face;
		if (FT_Init_FreeType(&library) != 0)
			_errorTrace();
		if (FT_New_Face(library, fontFullname.c_str(), 0, &face) != 0)
			_errorTrace();
		if (FT_Set_Pixel_Sizes(face, 0, fontSize) != 0)
			_errorTrace();

		FT_GlyphSlot slot = face->glyph;
		Text_Dimension text_dimensions = text_dimension(text_unicode, face);

		FT_Vector pen;
		pen.x = 0;
		wchar_t pc = 0;
		int len = text_unicode.length();
		for (int n = 0; n < len; n++)
		{
			wchar_t c = text_unicode[n];
			Mono_Glyph glyph = get_glyph(c, face);
			pen.x += kerning_offset(c, pc, face);
			pen.y = text_dimensions.height - glyph.ascent - text_dimensions.baseline;
			draw_puffer(face, pen.x, pen.y, text_dimensions);
			pen.x += glyph.advance_width;
			pc = c;
		}

		//showBitmap(text_dimensions.outpuffer, text_dimensions.width, text_dimensions.height);

		bitmap->width = text_dimensions.width;
		bitmap->height = text_dimensions.height;
		bitmap->byteperpixel = 4;
		bitmap->buf.resize(text_dimensions.width * text_dimensions.height);
		int totalSize = text_dimensions.width * text_dimensions.height;
		for (int i = 0; i < totalSize; ++i)
		{
			unsigned char value = 255 - text_dimensions.outpuffer[i];
			jColor color(value, value, value, value == 255 ? 0 : 255);
			bitmap->buf[i] = color;
		}

		if (bitmap->Load() == false)
		{
			delete bitmap;
			bitmap = nullptr;
		}

		free(text_dimensions.outpuffer);
		FT_Done_Face(face);
		FT_Done_FreeType(library);
		library = nullptr;

		return bitmap;
	});
}

unsigned int jFontCapture::CalcKey()
{
	int len = mFontname.length() + sizeof(mFontsize) + mText.length() * 2;
	vector<char> buf;
	buf.resize(len);
	memcpy(&buf[0], mFontname.c_str(), mFontname.length());
	memcpy(&buf[0] + mFontname.length(), &mFontsize, sizeof(mFontsize));
	memcpy(&buf[0] + mFontname.length() + sizeof(mFontsize), mText.c_str(), mText.length() * 2);
	return ToKey(&buf[0], buf.size());
}




/* Get Kerning offset if needed. */
int kerning_offset(wchar_t c, wchar_t pc, FT_Face face)
{
	FT_Vector kerning;
	int error;

	error = FT_Get_Kerning(face, c, pc, FT_KERNING_DEFAULT, &kerning);

	if (error) {
		// TODO error handling.
	}

	return kerning.x / 64;
}

/* convert FreeType glyph bitmap to a simple 0|1 bitmap */
unsigned char* unpack_mono_bitmap(FT_Bitmap bitmap)
{
	unsigned char* result;
	int y, x;
	unsigned char byte_value;

	result = (unsigned char *)malloc(bitmap.rows * bitmap.width);
	int idx = 0;
	for (y = 0; y < bitmap.rows; y++) {
		for (x = 0; x < bitmap.width; x++) {
			byte_value = bitmap.buffer[y * bitmap.width + x];
			result[idx] = byte_value;
			idx++;
		}
	}

	return result;
}

/* Convert monochrome glyph slot information to simple structure. */
Mono_Glyph convert_glyph(FT_GlyphSlot slot)
{
	Mono_Glyph glyph_data;
	int descent = 0, ascent = 0, ascent_calc;

	glyph_data.width = slot->bitmap.width;
	glyph_data.height = slot->bitmap.rows;
	glyph_data.top = slot->bitmap_top;
	glyph_data.advance_width = slot->advance.x / 64;
	if (descent < (glyph_data.height - glyph_data.top)) {
		descent = glyph_data.height - glyph_data.top;
	}
	glyph_data.descent = descent;

	if (glyph_data.top < glyph_data.height) {
		ascent_calc = glyph_data.height;
	}
	else {
		ascent_calc = glyph_data.top;
	}
	if (ascent < (ascent_calc - descent)) {
		ascent = ascent_calc - descent;
	}
	glyph_data.ascent = ascent;

	return glyph_data;
}

/* Get simple monochrome structure for given char. */
Mono_Glyph get_glyph(wchar_t character, FT_Face face)
{
	/* load glyph image into the slot (erase previous one) */
	int error = FT_Load_Char(face, character, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP);
	if (error) {
		_errorTrace();
	}

	//int index = FT_Get_Char_Index(face, character);
	//FT_Matrix matrix = { 0x10000, 0x06000,0x00000,0x10000 };
	//FT_Load_Glyph(face, index, FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP);
	//FT_Set_Transform(face, &matrix, nullptr);
	//FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

	return convert_glyph(face->glyph);
}

/* Get dimensions information of the given Test. */
Text_Dimension text_dimension(wstring text, FT_Face face)
{
	int width, max_ascent, max_descent, kerning_x, n, num_chars;
	wchar_t character, prev_character;
	Text_Dimension result;
	Mono_Glyph glyph;

	width = 0;
	max_ascent = 0;
	max_descent = 0;
	prev_character = 0;

	num_chars = text.length();

	for (n = 0; n < num_chars; n++) {
		character = text[n];
		glyph = get_glyph(character, face);
		if (max_ascent < glyph.ascent) {
			max_ascent = glyph.ascent;
		}
		if (max_descent < glyph.descent) {
			max_descent = glyph.descent;
		}

		kerning_x = kerning_offset(character, prev_character, face);

		if ((glyph.advance_width + kerning_x) < (glyph.width + kerning_x)) {
			width += (glyph.width + kerning_x);
		}
		else {
			width += (glyph.advance_width + kerning_x);
		}

		prev_character = character;
	}
	result.height = max_ascent + max_descent;
	result.width = width;
	result.baseline = max_descent;
	result.outpuffer = (unsigned char *)malloc(result.height * result.width);
	memset(result.outpuffer, 0x00, result.height * result.width);

	return result;
}

/* Draw char to puffer on button. */
void draw_puffer(FT_Face face, int x, int y, Text_Dimension text_info)
{
	int srcpixel, dstpixel, row_offset, sx, sy;

	srcpixel = 0;
	dstpixel = y * text_info.width + x;
	row_offset = text_info.width - face->glyph->bitmap.width;

	for (sy = 0; sy < face->glyph->bitmap.rows; sy++) {
		for (sx = 0; sx < face->glyph->bitmap.width; sx++) {
			text_info.outpuffer[dstpixel] |= face->glyph->bitmap.buffer[srcpixel];
			srcpixel++;
			dstpixel++;
		}
		dstpixel += row_offset;
	}
}

void showBitmap(unsigned char* buf, int w, int h)
{
	int  i, j;

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
			//printf("%2x ", buf[w * i + j]);
			putchar(buf[w * i + j] == 0 ? '.' : '*');
			
		putchar('\n');
	}
}