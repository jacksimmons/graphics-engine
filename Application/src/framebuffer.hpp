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

		std::unique_ptr<Shader> m_screenShader;
		GLuint m_fbo;
		GLuint m_rbo;
		GLuint m_texColBuf;
	public:
		Framebuffer(int w, int h);
		~Framebuffer();
		GLuint getTexColBuf() const noexcept { return m_texColBuf; }
		constexpr int getW() const noexcept { return m_w; }
		constexpr int getH() const noexcept { return m_h; }
		void update();
		void rescale(int w, int h);
	};
}