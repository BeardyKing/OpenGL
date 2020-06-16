#include "TestRenderMultipleObjects.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


namespace test {
	TestRenderMultipleObjects::TestRenderMultipleObjects()
		:
	m_renderer(),
	m_layout(),
	m_va(),
	m_vb(),
	m_ib(),
	m_shader("res/shaders/Basic.shader"),
	m_texture("res/textures/fg.png"),
	m_positions{
		-50.0f, -50.0f, 0.0f, 0.0f, //0
		 50.0f, -50.0f, 1.0f, 0.0f, //1
		 50.0f,  50.0f, 1.0f, 1.0f, //2
		-50.0f,  50.0f, 0.0f, 1.0f  //3
	},
	m_indices{
		0,1,2,
		2,3,0
	},
	m_translationA(200, 200, 0),
	m_translationB(200, 400, 0),
	m_proj{ glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f) },
	m_view{ glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)) }
	{
		VertexBufferLayout* layout_temp = new VertexBufferLayout;
		m_layout = layout_temp;
		m_layout->Push<float>(2);
		m_layout->Push<float>(2);

		VertexBuffer* vb_temp = new VertexBuffer(m_positions, 4 * 4 * sizeof(float));
		m_vb = vb_temp;

		m_va.AddBuffer(*m_vb, *m_layout);

		IndexBuffer* ib_Temp = new IndexBuffer(m_indices, 6);
		m_ib = ib_Temp;

		m_shader.Bind();
		m_texture.Bind();
		m_shader.SetUniforms1i("u_Texture", 0);

		m_shader.Unbind();
		m_vb->Unbind();
		m_ib->Unbind();

		Renderer renderer_temp;
		m_renderer = &renderer_temp;
	}

	TestRenderMultipleObjects::~TestRenderMultipleObjects() {
		delete m_layout;
		delete m_vb;
		delete m_ib;

		m_layout = nullptr;
		m_vb = nullptr;
		m_ib = nullptr;
		//std::cout << "Layout Size : " << m_layout->getElements().size() << std::endl;
	}

	void TestRenderMultipleObjects::OnUpdate(float deltaTime) { }

	void TestRenderMultipleObjects::OnRender() {
		m_renderer->Clear();
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
			glm::mat4 mvp = m_proj * m_view * model;
			m_shader.Bind();
			m_shader.SetUniformsMat4f("u_MVP", mvp);

			m_renderer->Draw(m_va, *m_ib, m_shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
			glm::mat4 mvp = m_proj * m_view * model;
			m_shader.Bind();
			m_shader.SetUniformsMat4f("u_MVP", mvp);

			m_renderer->Draw(m_va, *m_ib, m_shader);
		}
	}

	void TestRenderMultipleObjects::OnImGuiRender() {
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("TRANSFROM");
		ImGui::SliderFloat3("translationA", &m_translationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("translationB", &m_translationB.x, 0.0f, 960.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}