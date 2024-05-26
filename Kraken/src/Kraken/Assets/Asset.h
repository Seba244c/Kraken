//
// Created by sebsn on 25-05-2024.
//

#pragma once

#include <krpch.h>

namespace Kraken {
	class AssetSpecification {
    public:
		explicit AssetSpecification(Identifier identifier, std::filesystem::path path) : m_Path(std::move(path)), m_Identifier(identifier) {}
        [[nodiscard]] std::filesystem::path GetPath() { return m_Path;  }
        [[nodiscard]] const Identifier& GetIdentifier() const { return m_Identifier;  }
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
        std::map<std::string, Scope<AssetSpecification>> m_Map;
        std::filesystem::path m_Folder;
        const std::string& m_Domain;
    };

    class AssetsManager {
    public:
        static void RegisterAssetProvider(const std::string& domain, Scope<DomainAssetProvider> assetProvider);
        static AssetSpecification& Get(Identifier identifier);
    };
}