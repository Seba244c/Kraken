#pragma once

class Resource {
public:
    Resource(const char* start, const size_t len) : resource_data(start), data_len(len) {}

    const char * const &data() const { return resource_data; }
    const size_t &size() const { return data_len; }

    const char *begin() const { return resource_data; }
    const char *end() const { return resource_data + data_len; }

    std::string toString() { return std::string(data(), size()); }

private:
    const char* resource_data;
    const size_t data_len;

};

#ifdef __cplusplus

#define KRI_LINK_RESOURCE(RESOURCE) extern "C" { extern const char _resource_##RESOURCE[]; extern const size_t _resource_##RESOURCE##_len; }
KRI_LINK_RESOURCE(assets_Renderer2D_Text_glsl)
KRI_LINK_RESOURCE(assets_Renderer2D_Quad_glsl)
KRI_LINK_RESOURCE(assets_VKDemo_glsl)

#endif