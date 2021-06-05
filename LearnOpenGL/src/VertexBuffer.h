#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;
	bool m_isDynamic;
	float* m_data;

public:
	VertexBuffer(const void* data, unsigned int size, bool isDynamic);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};