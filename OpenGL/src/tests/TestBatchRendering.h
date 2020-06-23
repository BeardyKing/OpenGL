#pragma once

#include "test.h"

namespace test {

	class TestBatchRendering : public Test 
	{
	public:
		TestBatchRendering();
		~TestBatchRendering();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_ClearColour[4];
	};
}
