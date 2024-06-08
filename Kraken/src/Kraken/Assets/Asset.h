//
// Created by sebsn on 25-05-2024.
//

#pragma once

#include <krpch.h>

// Forward declarations
namespace Kraken {
	class Shader;
    class Texture2D;
}

namespace Kraken {
	class AssetSpecification {
    public:
		virtual ~AssetSpecification() = default;
		[[nodiscard]] virtual const Identifier& GetIdentifier() const = 0;
        [[nodiscard]] virtual long long ToBuf(char **buffer) = 0;
        [[nodiscard]] virtual std::string ToString() = 0;
    };

    class FileAssetSpecification final : public AssetSpecification {
    public:
		explicit FileAssetSpecification(Identifier identifier, std::filesystem::path path) : m_Path(std::move(path)), m_Identifier(
			std::move(identifier)) {}

        [[nodiscard]] const Identifier& GetIdentifier() const override { return m_Identifier;  }
		[[nodiscard]] long long ToBuf(char** buffer) override;
        [[nodiscard]] std::string ToString() override;

	private:
        std::filesystem::path m_Path;
        Identifier m_Identifier;
	    
    };

    class DomainAssetProvider {
    public:
        virtual ~DomainAssetProvider() = default;

        virtual AssetSpecification& Get(Identifier& identifier) = 0;
        virtual const std::string& GetDomain() = 0;
    };

    class FolderAssetProvider : public DomainAssetProvider {
    public:
        FolderAssetProvider(const std::string& domain, const std::string& folderPath);
	    ~FolderAssetProvider() override = default;

	    AssetSpecification& Get(Identifier& identifier) override;
        const std::string& GetDomain() override { return m_Domain; }
    private:
        std::map<std::string, Scope<FileAssetSpecification>> m_Map;
        std::filesystem::path m_Folder;
        const std::string m_Domain;
    };

    template <typename T>
    class AssetLibrary {
    public:
        virtual ~AssetLibrary() = default;
        Ref<T> Get(const Identifier& identifier);

        [[nodiscard]] bool Exists(const Identifier& identifier) const;
    private:
        void Add(const Identifier& identifier, const Ref<T>& asset);

        std::unordered_map<std::string, Ref<T>> m_Assets;
    private:
        virtual Ref<T> CreateAsset(AssetSpecification& specs) = 0;
    };


    // Library utility macros
#define KR_ASSETLIB_TYPE(type) class type##Library final : public AssetLibrary<type> { Ref<type> CreateAsset(AssetSpecification& specs) override; };

#define KR_INTERNAL_ASSETMANAGER_LIBTYPE(type)                                     \
    public:                                                                        \
		[[nodiscard]] static Ref<type> Get##type(const Identifier& identifier);    \
		[[nodiscard]] static bool type##Exists(const Identifier& identifier);      \
	private:                                                                       \
		static type##Library s_Lib##type##s;

#define KR_INTERNAL_ASSETMANAGER_LIBTYPE_CPP(type)                                                                            \
    [[nodiscard]] Ref<type> AssetsManager::Get##type(const Identifier& identifier) { return s_Lib##type##s.Get(identifier); } \
    [[nodiscard]] bool AssetsManager::type##Exists(const Identifier& identifier) { return s_Lib##type##s.Exists(identifier); }  \
    type##Library AssetsManager::s_Lib##type##s;

    // Library Types
    KR_ASSETLIB_TYPE(Shader)
    KR_ASSETLIB_TYPE(Texture2D)

    class AssetsManager {
    public:
        static void RegisterAssetProvider(const std::string& domain, Scope<DomainAssetProvider> assetProvider);
        static AssetSpecification& Get(Identifier identifier);

		KR_INTERNAL_ASSETMANAGER_LIBTYPE(Shader)
		KR_INTERNAL_ASSETMANAGER_LIBTYPE(Texture2D)
    };
}
