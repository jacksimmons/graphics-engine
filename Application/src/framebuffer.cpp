#include <iostream>
#include "framebuffer.hpp"
#include "scene.hpp"
#include "log.hpp"


Framebuffer::Framebuffer(int width, int height)
{
	m_w = width;
	m_h = height;

	try
	{
		m_screenShader = std::make_unique<Shader>("src/shaders/screenShader.vert", "src/shaders/screenShader.frag");
	}
	catch (ShaderCompileError const &e)
	{
		m_screenShader = nullptr;
		TE_CORE_CRITICAL("FB: Unable to read shader file.");
		std::cin.get();
	}

	m_screenShader->use();
	m_screenShader->setInt("screenTexture", 1); // ! Tutorial says 0 but 1 works for this program ?!

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	// Create texture for colour attachment
	// Require reading of individual pixels, so cannot use a renderbuffer.
	glGenTextures(1, &m_texColBuf);
	glBindTexture(GL_TEXTURE_2D, m_texColBuf);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_w, m_h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texColBuf, 0);

	// Create renderbuffer for depth/stencil attachment.
	// Renderbuffers are fast to write to and swap, which is all we need a container of
	// depth/stencil attachments to do.
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_w, m_h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		TE_CORE_CRITICAL("Error: Framebuffer is not complete.");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	m_screenShader->unuse();
}


Framebuffer::~Framebuffer()
{
	glDeleteTextures(1, &m_texColBuf);
	glDeleteFramebuffers(1, &m_fbo);
	glDeleteRenderbuffers(1, &m_rbo);
}


/// <summary>
/// Render a quad whose texture is the whole scene.
/// </summary>
void Framebuffer::update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Tank::Scene::getActiveScene()->update();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Disable depth testing so quad renders over everything.
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);

	// Re-enable depth testing.
	glEnable(GL_DEPTH_TEST);
}


void Framebuffer::rescale(int w, int h)
{
	glBindTexture(GL_TEXTURE_2D, m_texColBuf);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texColBuf, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	m_w = w;
	m_h = h;
}