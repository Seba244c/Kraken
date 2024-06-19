#include "Asset.h"
#include "Kraken/Graphics/RenderCommand.h"
#include "Kraken/Renderer/Font.h"
#include "Kraken/Renderer/Sprite.h"
#include "Kraken/Utils/Files.h"

namespace Kraken {
	long long FileAssetSpecification::ToBuf(char** buffer) {
		return Files::LoadFile(buffer, m_Path);
	}

	std::string FileAssetSpecification::ToString() {
		return Files::ReadFile(m_Path);
	}

	std::vector<std::string> FileAssetSpecification::ToLines() {
		return Files::ReadLines(m_Path);
	}

	ResourceAssetSpecification::ResourceAssetSpecification(const Identifier &identifier, const Resource data) : m_Data(data), m_Identifier(identifier)  {
	}

	ResourceAssetSpecification::~ResourceAssetSpecification() {
	}

	long long ResourceAssetSpecification::ToBuf(char** buffer) {
		*buffer = const_cast<char *>(m_Data.data());
		return m_Data.size();
	}

	std::string ResourceAssetSpecification::ToString() {
		return m_Data.toString();
	}

	std::vector<std::string> ResourceAssetSpecification::ToLines() {
		std::vector<std::string> out;

		std::stringstream stream(ToString());
		std::string line;
		while (std::getline(stream, line)) {
		  out.push_back(line);
		}

		return out;
	}

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
				m_Map[assetName] =  CreateScope<FileAssetSpecification>(Identifier{domain,assetName}, path);
			}
		}
	}

	AssetSpecification& FolderAssetProvider::Get(Identifier& identifier) {
		KRC_ASSERT(m_Map.contains(identifier.name))
		return *m_Map[identifier.name];
	}
	
#define KRI_ADD_RESOURCE(NAME, RESOURCE) m_Map[NAME] = CreateScope<ResourceAssetSpecification>(Identifier{ "KRInternal", NAME },Resource(_resource_##RESOURCE, _resource_##RESOURCE##_len))
	KrakenInternalAssetProvider::KrakenInternalAssetProvider() : m_Domain("KRInternal") {
		KRI_ADD_RESOURCE("Renderer2D_Text.glsl", assets_Renderer2D_Text_glsl);
		KRI_ADD_RESOURCE("Renderer2D_Quad.glsl", assets_Renderer2D_Quad_glsl);
		KRI_ADD_RESOURCE("04B_03__.TTF", assets_04B_03___TTF);
		KRI_ADD_RESOURCE("VKDemo.glsl", assets_VKDemo_glsl);
	}

	KrakenInternalAssetProvider::~KrakenInternalAssetProvider() {
	}

	AssetSpecification& KrakenInternalAssetProvider::Get(Identifier& identifier) {
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

	template <typename T>
	Ref<T> AssetLibrary<T>::Get(const Identifier& identifier) {
		if (!Exists(identifier)) {
			KRC_TRACE("Creating Asset: {0}", identifier.ToString());
			auto& spec = AssetsManager::Get(identifier);

			m_Assets[identifier.ToString()] = CreateAsset(spec);
		}

		return m_Assets[identifier.ToString()];
	}

	template <typename T>
	bool AssetLibrary<T>::Exists(const Identifier& identifier) const {
		return m_Assets.contains(identifier.ToString());
	}

	template <typename T>
	void AssetLibrary<T>::Add(const Identifier& identifier, const Ref<T>& asset) {
		KRC_ASSERT(!Exists(identifier), "Asset already exists!");
		m_Assets[identifier.ToString()] = asset;
	}

	Ref<Shader> ShaderLibrary::CreateAsset(AssetSpecification& specs) {
		return RenderCommand::CreateShader(specs);
	}

	Ref<Texture2D> Texture2DLibrary::CreateAsset(AssetSpecification& specs) {
		return RenderCommand::CreateTexture(specs);
	}

	Ref<SpriteSheet> SpriteSheetLibrary::CreateAsset(AssetSpecification& specs) {
		return CreateRef<SpriteSheet>(specs);
	}

	Ref<Font> FontLibrary::CreateAsset(AssetSpecification& specs) {
		return CreateRef<Font>(specs);
	}

	KR_INTERNAL_ASSETMANAGER_LIBTYPE_CPP(Shader)
	KR_INTERNAL_ASSETMANAGER_LIBTYPE_CPP(Texture2D)
	KR_INTERNAL_ASSETMANAGER_LIBTYPE_CPP(SpriteSheet)
	KR_INTERNAL_ASSETMANAGER_LIBTYPE_CPP(Font)
}
