#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Textures.h"

#include "vendors/glm/glm.hpp"
#include "vendors/glm/gtc/matrix_transform.hpp"

namespace test {


	TestTexture2D::TestTexture2D() : m_Proj(glm::ortho(0.0f, 1280.f, 0.0f, 720.0f, -1.0f, 1.0f)), m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))), m_TranslationA(0, 0, 0)
	{
		//Set Indices and vertex positions
		float positions[] = {
			-243.0f, -130.0f, 0.0f, 0.0f,
			 243.0f, -130.0f, 1.0f, 0.0f,
			 243.0f,  130.0f, 1.0f, 1.0f,
			-243.0f,  130.0f, 0.0f, 1.0f,

			 0.0f, 0.0f, 0.0f, 0.0f,
			 486.0f, 0.0f, 1.0f, 0.0f,
			 486.0f, 260.0f, 1.0f, 1.0f,
			 0.0f, 260.0f, 0.0f, 1.0f
		};

		unsigned int indices[] =
		{
			0,1,2,
			2,3,0,

			4,5,6,
			6,7,4
		};

		//Setup blending method
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

		// Generate Texture and bind to shader
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.2f, 0.2f, 1.0f, 1.0f);
		m_Textuire = std::make_unique<Texture>("res/textures/Texture_meme.png");
		m_Shader->SetUniform1i("u_Texture", 0);

	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Textuire->Bind();

		//render once
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA); //model matrix
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_ModelViewProjectionMatrix", mvp);
			renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		}

	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


}