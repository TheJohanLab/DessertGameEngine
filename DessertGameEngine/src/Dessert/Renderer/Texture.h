#pragma once

namespace Dessert {
	
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		//In the case the texture is missing, put this generic texture instead
		static Ref<Texture2D> Create(const std::string& path);
	};

}