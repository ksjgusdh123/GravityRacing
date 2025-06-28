#pragma once
#include "ISystem.h"

class FSystemManager
{
public:
	FSystemManager();
	~FSystemManager();

	template<typename T, typename... Args>
		requires std::derived_from<T, ISystem>
	void RegisterSystem(Args&&... args)
	{
		Systems[typeid(T)] = std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	std::shared_ptr<T> GetSystem()
	{
		auto it = Systems.find(typeid(T));
		if (it != Systems.end())
			return std::static_pointer_cast<T>(it->second);
		return nullptr;
	}

	void InitAll();
	void ShutdownAll();

private:
	std::unordered_map<std::type_index, std::shared_ptr<ISystem>> Systems;
};
