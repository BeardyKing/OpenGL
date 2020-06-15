#pragma once

#include "test.h"

namespace test {

	class TestDrawTriangle : public Test
	{
	public:
		TestDrawTriangle();
		~TestDrawTriangle();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
	};
}
