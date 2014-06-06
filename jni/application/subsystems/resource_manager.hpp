#ifndef ZEPTOTEST_RESOURCE_MANAGER_HPP
#define ZEPTOTEST_RESOURCE_MANAGER_HPP

#include "defs.hpp"
#include "utils/noncopyable.hpp"
#include "utils/subsystem.hpp"
#include "graphics/texture.hpp"

#include "android_native_app_glue.h"

#include <list>
#include <map>
#include <string>
#include <memory>

namespace subsystems
{

    class resource_manager_t: private noncopyable_t, public subsystem_t
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
