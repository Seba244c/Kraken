//
// Created by sebsn on 25-05-2024.
//

#pragma once

#include <krpch.h>

namespace Kraken {
	class AssetSpecification {
    public:
		explicit AssetSpecification(std::filesystem::path path) : m_Path(std::move(path)) {}
        std::filesystem::path GetPath() { return m_Path;  }
    private:
        std::filesystem::path m_Path;
    };

    class DomainAssetProvider {
    public:
        virtual ~DomainAssetProvider() = default;

        virtual AssetSpecification& Get(Identifier& identifier) = 0;
    };

    class FolderAssetProvider : public DomainAssetProvider {
    public:
        FolderAssetProvider(const std::string& folderPath);
	    ~FolderAssetProvider() override = default;

	    AssetSpecification& Get(Identifier& identifier) override;
    private:
        std::map<std::string, Scope<AssetSpecification>> m_Map;
        std::filesystem::path m_Folder;
    };

    class AssetsManager {
    public:
        static void RegisterAssetProvider(const std::string& domain, Scope<DomainAssetProvider> assetProvider);
        static AssetSpecification& Get(Identifier identifier);
    };
}