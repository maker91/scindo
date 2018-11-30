#pragma once

#include <map>
#include <memory>
#include <typeinfo>
#include <iostream>

#include "component.hpp"

namespace scindo
{

	class entity
	{
	public:
		entity() {};
		entity(entity &&) = delete;
		entity(entity &) = delete;
		entity &operator=(entity &) = delete;
		
		virtual ~entity()
		{
			components.clear();
		};

		template <typename T, typename... Args>
		std::shared_ptr<T> attach_component(Args... args)
		{
			static_assert(std::is_base_of<component, T>::value, "component must be derived from scindo::component.");
		
			std::shared_ptr<T> c = std::make_shared<T>(args...);
			c->owner = this;
			c->enforce();

			components[typeid(T).hash_code()] = c;
			return c;
		}

		template <typename T, typename... Args>
		void enforce_component(Args... args)
		{
			if (!components.count(typeid(T).hash_code())) {
				std::cout << "warning: enforcing attachment of component " << typeid(T).name() << std::endl;
				attach_component<T>(args...);
			}
		}

		template <typename T>
		void dettach_component()
		{
			size_t hash = typeid(T).hash_code();
			
			if (components.count(hash)) {
				components.erase(hash);
			}
		}

		template <typename T>
		std::shared_ptr<T> request_component() const
		{
			size_t hash = typeid(T).hash_code();
			
			if (components.count(hash)) {
				return std::static_pointer_cast<T>(components.at(hash));
			}

			throw std::runtime_error("Component request failed!");
		}

		template <typename T>
		bool has_component() const
		{
			return components.count(typeid(T).hash_code()) == 1;
		}

		void tick(float dt)
		{
			for (auto &component : components) {
				component.second->tick(dt);
			}
		}

	private:
		std::map<std::size_t, std::shared_ptr<component>> components;
	};
}
