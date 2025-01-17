#pragma once
#include <glad/glad.h>


namespace Tank
{
	class Shader;
	class Framebuffer
	{
	private:
		/// <summary>
		/// The viewport size when rendering to the view framebuffer.
		/// </summary>
		int m_w, m_h;

		GLuint m_fbo;
		GLuint m_rbo;
		GLuint m_texColBuf;
	public:
		Framebuffer(int w, int h);
		~Framebuffer();
		void checkStatus() const;
		GLuint getTexColBuf() const noexcept { return m_texColBuf; }
		constexpr int getW() const noexcept { return m_w; }
		constexpr int getH() const noexcept { return m_h; }
		void update() const;
		void rescale(int w, int h);
	};
}