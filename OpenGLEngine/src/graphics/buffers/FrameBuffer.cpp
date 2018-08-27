#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{

}

FrameBuffer::~FrameBuffer()
{
	if (m_BufferID != -1)
		glDeleteFramebuffers(1, &m_BufferID);

}

void FrameBuffer::AttachTexture(const Texture2D *pTexture, GLenum attachment, bool drawToTexture)
{
	m_Textures[attachment] = pTexture;
	if (drawToTexture)
		m_DrawTextures.push_back(attachment);
}

void FrameBuffer::AddParameter(GLenum pname, GLint param)
{
	m_Parameters[pname] = param;
}

bool FrameBuffer::Init()
{
	glGenFramebuffers(1, &m_BufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);

	for (const auto& texture : m_Textures)
		texture.second->AttachToBoundFBO(texture.first);

	for (auto const& param : m_Parameters)
		glFramebufferParameteri(GL_FRAMEBUFFER, param.first, param.second);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) 
	{
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	if (m_DrawTextures.size() > 0)
		glDrawBuffers(m_DrawTextures.size(), &m_DrawTextures[0]);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);
}

void FrameBuffer::BindRead() const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_BufferID);
}

void FrameBuffer::BindDraw() const
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_BufferID);
}

void FrameBuffer::BindDefaultFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindDefaultAsRead()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void FrameBuffer::BindDefaultAsDraw()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}