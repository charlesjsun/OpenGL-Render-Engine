#include "Mesh.h"

Mesh::Mesh(std::vector<GLfloat>& Vertices, std::vector<GLuint>& Indices, std::vector<VertexAttribute>& Attributes)
{

	m_pIBO = new IndexBuffer(&Indices[0], Indices.size());

	VertexBuffer *pVBO = new VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	pVBO->Bind();
	pVBO->SetData(&Vertices[0], Vertices.size());

	m_pVAO = new VertexArray();
	m_pVAO->Bind();

	m_pVAO->SetVertexBuffer(pVBO);

	for (int i = 0; i < Attributes.size(); i++)
	{
		m_pVAO->AddVertexAttributePointer(Attributes[i]);
	}

	m_pVAO->Unbind();
	pVBO->Unbind();

}

Mesh::~Mesh()
{
	delete m_pVAO;
	delete m_pIBO;
}

void Mesh::Render(GLenum RenderMode) const
{

	m_pVAO->Bind();
	m_pIBO->Bind();
	glDrawElements(RenderMode, m_pIBO->GetCount(), GL_UNSIGNED_INT, 0);
	//m_pVAO->unbind();
	//m_pIBO->unbind();

}