#include "pch.h"
#include "CppUnitTest.h"
#include "../spring_alg3/Graph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace springtest3
{
	TEST_CLASS(springtest3)
	{
	public:
		
		TEST_METHOD(TestFordBellman)
		{
			Graph* graph = new Graph(4);
			graph->set_edge(0, 3, 1000);
			graph->set_edge(0, 1, 10);
			graph->set_edge(1, 3, 10);
			graph->set_edge(0, 2, 100);
			graph->set_edge(2, 3, 100);
			Graph::Path* path = graph->find_best(0, 3);
			Assert::AreEqual(20Ui64, path->length);
			Graph::Node* curr = path->start;
			Assert::AreEqual(0, curr->node);
			curr = curr->next;
			Assert::AreEqual(1, curr->node);
			curr = curr->next;
			Assert::AreEqual(3, curr->node);
			Assert::IsNull(curr->next);
		}
	};
}
