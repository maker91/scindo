#pragma once

#include <typeinfo>

namespace scindo
{
	class entity;

	class component
	{
		friend class entity;

	public:
		component(component &)=delete;
		component(component &&)=delete;
		component &operator=(component &)=delete;

		virtual void enforce() {}
		virtual void tick(float dt) {}

	protected:
		component() {}

	protected:
		entity	*owner;
	};
}
