#ifndef ZEPTOTEST_RESOURCE_MANAGER_HPP
#define ZEPTOTEST_RESOURCE_MANAGER_HPP

#include "defs.hpp"
#include "utils/noncopyable.hpp"
#include "utils/subsystem.hpp"

#include "android_native_app_glue.h"

#include <list>
#include <map>
#include <string>
#include <memory>

#include <GLES2/gl2.h>

class aasset_buffer_t
{
public:
	explicit aasset_buffer_t(AAsset* asset);

	BYTE* data();

	uint32_t size() const;

	bool seek(uint32_t pos);

	bool read_next(BYTE* dest, uint32_t count);

private:
	AAsset* _asset;
	uint32_t _size;
	BYTE* _data;
	uint32_t _position;
};

class texture_t
{
public:
	explicit texture_t(aasset_buffer_t& buffer);

	void activate();
	void deactivate();

private:
	GLuint _id;
	bool _alpha;
};

namespace subsystems {

class resource_manager_t : private noncopyable_t, public subsystem_t
{
public:
    explicit resource_manager_t(android_app* java_app);
    virtual void start() override;
	virtual void stop() override;

	texture_t* get_texture(const char* name);
private:
	android_app* _java_app;
	AAssetManager* _asset_manager;
	std::map<std::string, std::unique_ptr<texture_t> > _tex_map;

	void load_textures();

	std::list<std::string> get_file_list(const char* path);
};

} /* namespace subsystems */

#endif // ZEPTOTEST_RESOURCE_MANAGER_HPP
