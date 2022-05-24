#include "../../include/Ressources/RessourcesManager.h"

void Ressource::ressource_manager::delete_ressource(const std::string& key)
{
	manager_.erase(key);
}