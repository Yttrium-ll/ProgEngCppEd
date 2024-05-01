#include <cassert>
#include <chrono>
#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

using namespace std::literals;

#include <boost/noncopyable.hpp>

//  ================================================================================================

class Functor
{
public:

	explicit Functor(int & value) noexcept: m_value(value) {}

	void operator()() const noexcept
	{
		for (std::size_t i = 0; i < 1'000'000; ++i) ++m_value;
	}

private:

	int & m_value;

}; // class Functor

//  ================================================================================================

constexpr void f(int & x, int y) noexcept { x = y; } // note: return unavailable for std::thread

class C { public: void print() const { std::cout << "C::print" << std::endl; } };

//  ================================================================================================

class Scoped_Thread : boost::noncopyable // note: std::thread is move-only class
{
public:

	explicit Scoped_Thread(std::thread thread) : m_thread(std::move(thread))
	{
		if (!m_thread.joinable()) throw std::runtime_error("invalid thread");
	}

   ~Scoped_Thread() noexcept
	{
		try { m_thread.join(); } catch (...) { std::abort(); }
	}

private:

	std::thread m_thread;

}; // class Scoped_Thread : boost::noncopyable

//  ================================================================================================

int main()
{
    std::thread thread([](){ std::cout << "Hello, world!" << std::endl; });

    std::cout << std::this_thread::get_id() << std::endl;
    std::cout <<           thread. get_id() << std::endl;

    thread.join(); // good: otherwise std::terminate called in destructor

    auto x = 42;

//  std::thread(Functor(x)).detach(); // bad: possible dangling reference

//  std::thread(f, x, 43).join(); // bad: internal copies for arguments

    std::thread(f, std::ref(x), 43).join(); // good: std::reference_wrapper

    assert(x == 43);

    const C c;

    std::thread(&C::print, &c).join(); // note: consider (c.*&C::print)()

    const Scoped_Thread scoped_thread(std::thread(f, std::ref(x), 42));

    assert(std::thread::hardware_concurrency() != 0);

    std::vector < std::thread > threads;

	for (std::size_t i = 0; i < std::thread::hardware_concurrency(); ++i)
	{
		threads.emplace_back([](){});
	}

	for (auto & thread : threads) thread.join();

    std::jthread jthread_1([](std::stop_token token) // note: threadsafe view
    {
        std::stop_callback callback(token, []()
        {
            std::cout << "jthread_1::callback" << std::endl;
        });

        for (std::size_t i = 0; i < 10; ++i)
        {
            std::cout << "jthread_1: " << i << std::endl;

            std::this_thread::sleep_for(0.1s);
        }
    });

    std::jthread jthread_2([](std::stop_token token) // note: threadsafe view
    {
        for (std::size_t i = 0; i < 10; ++i)
        {
            std::cout << "jthread_2: " << i << std::endl;

            std::this_thread::sleep_for(0.1s);

            if (token.stop_requested()) return; // note: interruption
        }
    });

    std::this_thread::sleep_for(0.5s);

    jthread_1.get_stop_source().request_stop();
    jthread_2.get_stop_source().request_stop(); // note: interruption

    return 0;
}