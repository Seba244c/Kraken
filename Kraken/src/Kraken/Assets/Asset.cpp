#include "Asset.h"

namespace Kraken {
	FolderAssetProvider::FolderAssetProvider(const std::string& domain, const std::string& folderPath) : m_Domain(domain) {
		m_Folder = folderPath;
		if(!exists(m_Folder)) {
			create_directories(m_Folder);
		}

		if(!is_directory(m_Folder)) {
			KRC_ERROR("FolderPath does not specify a folder!");
			KRC_ASSERT(false)
			return;
		}

		// Search for assets
		using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
		for (const auto& dirEntry : recursive_directory_iterator(m_Folder)) {
			// dirEntry path usually looks like this "assets/shaders\\Sandbox.glsl"
			if (is_regular_file(dirEntry)) {
				auto path = dirEntry.path().string();
				std::ranges::replace(path, '\\', '/');

				auto assetName = path;
				if (assetName.starts_with(folderPath)) assetName = assetName.substr(folderPath.length());

				KRC_TRACE("Discovered Asset: {0}", assetName);
				m_Map[assetName] =  CreateScope<AssetSpecification>(Identifier{domain,assetName}, path);
			}
		}
	}

	AssetSpecification& FolderAssetProvider::Get(Identifier& identifier) {
		KRC_ASSERT(m_Map.contains(identifier.name))
		return *m_Map[identifier.name];
	}

	static std::map<std::string, Scope<DomainAssetProvider>> s_providerMap;

	void AssetsManager::RegisterAssetProvider(const std::string& domain, Scope<DomainAssetProvider> assetProvider) {
		s_providerMap[domain] = std::move(assetProvider);
	}

	AssetSpecification& AssetsManager::Get(Identifier identifier) {
		KRC_ASSERT(s_providerMap.contains(identifier.domain), "Domain not found!")
		return s_providerMap[identifier.domain]->Get(identifier);
	}
}
