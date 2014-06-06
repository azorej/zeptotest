#include "resource_manager.hpp"

#include "utils/opengl.hpp"

namespace subsystems
{

    resource_manager_t::resource_manager_t(android_app* java_app)
            : _java_app(java_app),
              _asset_manager(java_app->activity->assetManager)
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

        for (auto name : files)
        {
            std::string path("textures/");
            path.append(name);
            auto asset = AAssetManager_open(_asset_manager, path.c_str(), AASSET_MODE_STREAMING);
            my_assert(asset && "Couldn't open texture file");
            MAKE_GUARD(asset, [](decltype(asset) ptr)
            {   AAsset_close(ptr);});

            auto buf = aasset_buffer_t(asset);
            _tex_map[std::string(name)] = std::unique_ptr<texture_t>(new texture_t(buf));
        }
    }

    std::list<std::string> resource_manager_t::get_file_list(const char* path)
    {
        std::list<std::string> ret;

        auto dir = AAssetManager_openDir(_asset_manager, "textures");
        my_assert(dir && "Couldn't open asset directory");

        MAKE_GUARD(dir, [](decltype(dir) ptr)
        {   AAssetDir_close(ptr);});

        const char* name;
        while ((name = AAssetDir_getNextFileName(dir)) != NULL)
        {
            ret.push_back(name);
        }

        return ret;
    }

} /* namespace subsystems */
