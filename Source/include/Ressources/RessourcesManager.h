#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace Ressource
{
	// ReSharper disable once CppInconsistentNaming
	class IRessource
	{
	public:
		~IRessource() = default;
	};

	class ressource_manager
	{
	public:
		template <typename T, typename ...args>
		T* create_ressource(const char* const& filename, args... Args)
		{
		    static_assert(std::is_base_of_v<IRessource, T>, "T is not a child of IResources");

		    manager_[filename] = std::make_unique<T>(filename, Args...);
		    const std::unique_ptr<IRessource> &value = manager_[filename];
		    IRessource* ptr = value.get();
		    T* rsc = static_cast<T*>(ptr);
		    return rsc;
		}

		template <typename T>
		T* get_ressource(const std::string key)
		{
			auto it = manager_.find(key);
			if (it != manager_.end())
			{
				return static_cast<T*>(it->second);
			}
			return nullptr;
		}

		void delete_ressource(const std::string& key);

	private:
		std::unordered_map<std::string, std::unique_ptr<IRessource>> manager_;
	};
}  
