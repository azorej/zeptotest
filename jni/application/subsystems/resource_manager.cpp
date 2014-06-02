#include "resource_manager.hpp"

#include "opengl_utils.hpp"

#include "png.h"
#include "pngstruct.h"

#include "GLES2/gl2.h"


void read_from_aasset_buffer(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead)
{
	my_assert(png_ptr->io_ptr);
	aasset_buffer_t& buffer = *(aasset_buffer_t*)png_ptr->io_ptr;

	buffer.read_next((BYTE*)outBytes, byteCountToRead);
}

aasset_buffer_t::aasset_buffer_t(AAsset* asset)
: _asset(asset)
, _position(0)
, _data(nullptr)
{
	_size = AAsset_getLength(_asset);
}

BYTE* aasset_buffer_t::data()
{
	if(!_data) _data = (BYTE*)AAsset_getBuffer(_asset);
	return _data;
}

uint32_t aasset_buffer_t::size() const
{
	return _size;
}

bool aasset_buffer_t::seek(uint32_t pos)
{
	if(pos > size()-1) return false;
	_position = pos;
	return true;
}

bool aasset_buffer_t::read_next(BYTE* dest, uint32_t count)
{

	if(_position + count > size()) count -= size() - _position;
	if(count > 0)
	{
		{
			std::copy(data()+_position, data()+_position+count, dest);
		}
		_position += count;
	}
	else return false;

	return true;
}

class raw_bitmap_t : private noncopyable_t
{
public:
	raw_bitmap_t()
	: _width(0)
	, _height(0)
	, _bytes_per_pixel(0)
	, _row_offset(0)
	, _format(0)
	, _alpha(false)
	{
	}

	BYTE* data()
	{
		return _data.get();
	}

	uint32_t width() const
	{
		return _width;
	}

	uint32_t height() const
	{
		return _height;
	}

	uint32_t format() const
	{
		return _format;
	}

	uint32_t size() const
	{
		return (_width*_bytes_per_pixel + _row_offset)*_height;
	}

	bool alpha() const
	{
		return _alpha;
	}

	uint8_t bytes_per_pixel() const
	{
		return _bytes_per_pixel;
	}

	void set_pixel(uint32_t row_idx, uint32_t col_idx, BYTE r, BYTE g, BYTE b, BYTE a)
	{
		my_assert(_bytes_per_pixel == 4);
		auto ptr = get_row(row_idx);
		ptr += col_idx * 4;

		ptr[0] = r;
		ptr[1] = g;
		ptr[2] = b;
		ptr[3] = a;
	}

	BYTE* get_row(uint32_t row_idx)
	{
		auto offset = row_idx * (_width * _bytes_per_pixel + _row_offset);
		return (_data.get() + offset);
	}

	void init(uint32_t w, uint32_t h, uint32_t format)
	{
		switch(format)
		{
			case GL_RGB:
			{
				_bytes_per_pixel = 3;
				break;
			}
			case GL_RGBA:
			{
				_bytes_per_pixel = 4;
				_alpha = true;
				break;
			}
			default:
			{
				my_assert(false && "Unknown image format");
				return;
			}
		}
		_format = format;

		uint32_t row_bytes = w * _bytes_per_pixel;

		// glTexImage2d requires rows to be 4-byte aligned
		_row_offset = 3 - ((row_bytes-1) % 4);
		row_bytes += _row_offset;

		_width = w;
		_height = h;
		_data.reset(new BYTE[h*row_bytes]);
	}

	bool is_valid() const
	{
		return (_data && _width && _height && _bytes_per_pixel);
	}

private:
	std::unique_ptr<BYTE> _data;
	uint32_t _width;
	uint32_t _height;
	uint8_t _bytes_per_pixel;
	uint8_t _row_offset;
	uint32_t _format;
	bool _alpha;
};

void png_load_from_memory(aasset_buffer_t& buf, raw_bitmap_t* out_bitmap)
{
	//png header - 8 bytes
	if (png_sig_cmp(buf.data(), 0, 8))
	{
		my_assert(false && "Couldn't load texture. It isn't png file.");
		return;
	}

	auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	my_assert(png_ptr && "Couldn't load texture. Png load error.");
	MAKE_GUARD(png_ptr, [](decltype(png_ptr) ptr){png_destroy_read_struct(&ptr, (png_infopp)NULL, (png_infopp)NULL);});

    auto info_ptr = png_create_info_struct(png_ptr);
    my_assert(info_ptr && "Couldn't load texture. Png load error.");
    auto info_deleter = [png_ptr](decltype(info_ptr) ptr){png_destroy_info_struct(png_ptr, &ptr);};
    MAKE_GUARD(info_ptr, info_deleter);

    // the code in this if statement gets called if libpng encounters an error
	if (setjmp((long*)png_jmpbuf(png_ptr)))
	{
		my_assert(false && "error from libpng");
	    return;
	}

    png_set_read_fn(png_ptr, &buf, read_from_aasset_buffer);
    // tell libpng we already read the signature
    png_set_sig_bytes(png_ptr, 8);
    buf.seek(8);

    png_read_info(png_ptr, info_ptr);

    png_uint_32 width = 0;
    png_uint_32 height = 0;
    int bitDepth = 0;
    int colorType = -1;
    my_assert(png_get_IHDR(png_ptr, info_ptr, &width, &height, &bitDepth, &colorType, NULL, NULL, NULL) == 1);

    bool transparency = false;
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
		transparency = true;
	}
	// Expands PNG with less than 8bits per channel to 8bits.
	if (bitDepth < 8) {
		png_set_packing (png_ptr);
	// Shrinks PNG with 16bits per color channel down to 8bits.
	} else if (bitDepth == 16) {
		png_set_strip_16(png_ptr);
	}

    uint32_t format = 0;
    switch(colorType)
    {
       case PNG_COLOR_TYPE_RGB:
		   format = transparency ? GL_RGBA : GL_RGB;
		   break;

       case PNG_COLOR_TYPE_RGBA:
          format = GL_RGBA;
          break;

       case PNG_COLOR_TYPE_PALETTE:
       {
		   png_set_palette_to_rgb(png_ptr);
		   format = transparency ? GL_RGBA : GL_RGB;
    	   break;
       }
       default:
    	   my_assert(false && "Png read error. Uknown color type");
    	   return;
    }
    //apply transformations
    png_read_update_info(png_ptr, info_ptr);

    out_bitmap->init(width, height, format);

    auto row_size = png_get_rowbytes(png_ptr, info_ptr);
	my_assert(row_size > 0);
	my_assert(row_size == width * out_bitmap->bytes_per_pixel());

	auto row_ptrs = new png_bytep[height];
	MAKE_GUARD(row_ptrs, [](decltype(row_ptrs) ptr){delete[] ptr;});

	for (int32_t i = 0; i < height; ++i) {
		row_ptrs[height - (i + 1)] = out_bitmap->get_row(i);
	}

	png_read_image(png_ptr, row_ptrs);
}

texture_t::texture_t(aasset_buffer_t& buffer)
: _alpha(false)
{
	raw_bitmap_t bitmap;

	png_load_from_memory(buffer, &bitmap);

	//Now generate the OpenGL texture object
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, bitmap.format(), bitmap.width(), bitmap.height(), 0, bitmap.format(), GL_UNSIGNED_BYTE,(GLvoid*)bitmap.data());

	glBindTexture(GL_TEXTURE_2D, 0);

	CHECK_GL_ERRORS("Couldn't load texture");

	if(bitmap.alpha()) _alpha = true;
}

void texture_t::activate()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _id);

	if(_alpha)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void texture_t::deactivate()
{
	glBindTexture(GL_TEXTURE_2D, 0);

	if(_alpha)
	{
		glDisable(GL_BLEND);
	}
}

//    AASSET_MODE_BUFFER : This performs fast small reads
//    AASSET_MODE_RANDOM : This reads chunks of data forward and backward
//    AASSET_MODE_STREAMING : This reads data sequentially with occasional forward seeks

namespace subsystems {

resource_manager_t::resource_manager_t(android_app* java_app)
: _java_app(java_app)
, _asset_manager(java_app->activity->assetManager)
{
}

void resource_manager_t::start()
{
//	FreeImage_Initialise(FALSE);
	load_textures();
}

void resource_manager_t::stop()
{
//	FreeImage_DeInitialise();
}

texture_t* resource_manager_t::get_texture(const char* name)
{
	auto it = _tex_map.find(std::string(name));
	my_assert(it != _tex_map.end());

	return it->second.get();
}

void resource_manager_t::load_textures()
{
	auto files = get_file_list("textures");

	for(auto name : files)
	{
		std::string path("textures/");
		path.append(name);
		auto asset = AAssetManager_open(_asset_manager, path.c_str(), AASSET_MODE_STREAMING);
		my_assert(asset && "Couldn't open texture file");
		MAKE_GUARD(asset, [](decltype(asset) ptr) { AAsset_close(ptr);});

		auto buf = aasset_buffer_t(asset);
		_tex_map[std::string(name)] = std::unique_ptr<texture_t>(new texture_t(buf));
	}
}

std::list<std::string> resource_manager_t::get_file_list(const char* path)
{
	std::list<std::string> ret;

	auto dir = AAssetManager_openDir(_asset_manager, "textures");
	my_assert(dir && "Couldn't open asset directory");

	MAKE_GUARD(dir, [](decltype(dir) ptr) { AAssetDir_close(ptr); });

	const char* name;
	while( (name = AAssetDir_getNextFileName(dir)) != NULL)
	{
		ret.push_back(name);
	}

	return ret;
}

} /* namespace subsystems */
