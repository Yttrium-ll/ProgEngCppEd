#include <cassert>
#include <iterator>
#include <queue>
#include <stack>

////////////////////////////////////////////////////////

int main()
{
	{
		std::stack < int > stack;

		stack.push(1);

		stack.push(2);

		assert(stack.top() == 2);

		stack.pop();

		assert(stack.top() == 1);
	}

//  ----------------------------------------------------

	{
		std::queue < int > queue;

		queue.push(1);
		
		queue.push(2);

		assert(queue.front() == 1 && queue.back() == 2);

		queue.pop();

		assert(queue.front() == 1 && queue.back() == 1);
	}

//  ----------------------------------------------------

	{
		std::priority_queue < int > queue;

		queue.push(1);

		queue.push(3);

		queue.push(2);

		assert(queue.top() == 3);

		queue.pop();

		assert(queue.top() == 2);
	}
}