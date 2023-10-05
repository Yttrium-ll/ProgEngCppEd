#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

#include <windows.h>

class Timer
{
public:

	using clock_t = std::chrono::steady_clock;

public:

	explicit Timer(std::string name) : m_name(std::move(name)), m_begin(clock_t::now())
	{
		std::cout << "Timer " << m_name << " launched ... " << std::endl << std::endl;
	}

	~Timer() noexcept
	{
		try
		{
			elapsed();
		}
		catch (...)
		{
			std::abort();
		}
	}

public:

	void elapsed() const
	{
		auto end = clock_t::now();

		std::cout << "Timer " << m_name << " elapsed " << std::setprecision(6) << std::fixed <<
			static_cast < double > (std::chrono::duration_cast < std::chrono::microseconds >
				(end - m_begin).count()) / 1'000'000.0 << " (seconds)" << std::endl << std::endl;
	}

private:

	const std::string m_name;

	const clock_t::time_point m_begin;
};

auto test_cpp_v1()
{
	printf("Hello, world!\n\n");

	MessageBoxA(GetForegroundWindow(), "Hello, world!\n\n", "window", MB_OK);
}

void test_asm_v1()
{
	const char * string = "Hello, world!\n\n";

	__asm
	{
		push [string]

		call printf // note: cdecl calling convention

		pop eax
	}

	const char * window = "window";

	__asm
	{
		push MB_OK

		push [window]
		push [string]
		
		call GetForegroundWindow

		push eax
		
		call MessageBoxA // note: stdcall calling convention
	}
}

auto test_cpp_v2()
{
	auto c = 0;

	{
		Timer timer("C++");

		auto a = 1;
		auto b = 1;

		for (auto i = 0; i < 10'000'000; ++i)
		{
			for (auto j = 0; j < 25; ++j)
			{
				c = a + b;
				a = b;
				b = c;
			}

			a = 1;
			b = 1;
		}
	}

	return c;
}

auto test_asm_v2()
{
	auto c = 0;
	
	{
		Timer timer("ASM");

		__asm
		{
			mov eax, 1
			mov ebx, 1

			mov esi, 10000000

		loop_1:

			mov edi, 25

		loop_2:

			mov ecx, eax
			add ecx, ebx
			mov eax, ebx
			mov ebx, ecx

			dec edi
			jnz loop_2

			mov eax, 1
			mov ebx, 1

			dec esi
			jnz loop_1

			mov [c], ecx
		}
	}

	return c;
}

auto test_cpp_v3(double * a, double * b, std::size_t size)
{
	auto result = 0.0;

	{
		Timer timer("C++");

		for (std::size_t i = 0; i < size; ++i)
		{
			result += std::pow(a[i] * b[i], 8.0);
		}
	}

	return result;
}

auto test_asm_v3(double * a, double * b, std::size_t size)
{
	auto result = 0.0;

	{
		const auto s = size;

		Timer timer("ASM");

		__asm
		{
			mov eax, [a]
			mov ebx, [b]
			mov ecx, [s]

			mov edx, 0

			fldz

		loop_1:
			
			fld qword ptr [eax + 8 * edx]

			mov esi, 3

		loop_pow_1:

			fld st(0)
			fmul

			dec esi
			jnz loop_pow_1

			fld qword ptr [ebx + 8 * edx]

			mov edi, 3

		loop_pow_2:

			fld st(0)
			fmul

			dec edi
			jnz loop_pow_2

			fmul
			fadd

			inc edx
			cmp edx, ecx
			jl loop_1;

			fstp [result]
		}
	}

	return result;
}

auto test_cpp_v4(double * a, std::size_t size)
{
	auto result = 0.0;

	{
		Timer timer("C++");

		/*
		for (std::size_t i = 2; i < size; ++i)
		{
			result += a[i] * a[i - 1] * a[i - 2];
		}
		*/

		for (std::size_t i = 3; i < size; i += 2)
		{
			result += a[i - 1] * a[i - 2] * (a[i - 3] + a[i]);
		}
	}

	return result;
}

auto test_asm_v4(double * a, std::size_t size)
{
	auto result = 0.0;

	{
		const auto s = size;

		Timer timer("ASM");

		__asm
		{
			mov eax, [a]
			mov esi, [s]

			mov ebx, 0
			mov ecx, 1
			mov edx, 2

			fldz

		loop_1:

			fld qword ptr [eax + 8 * ecx]
			fld qword ptr [eax + 8 * edx]

			fmul

			inc edx
			fld qword ptr [eax + 8 * edx]
			fld qword ptr [eax + 8 * ebx]

			fadd
			fmul
			fadd

			add ebx, 2
			add ecx, 2

			inc edx
			cmp edx, esi
			jl loop_1

			fstp [result]
		}
	}

	return result;
}

void test_1()
{
	std::cout << "Test 1 launched ... " << std::endl << std::endl;

	test_cpp_v1();
	test_asm_v1();
}

void test_2()
{
	std::cout << "Test 2 launched ... " << std::endl << std::endl;

	const auto result_cpp = test_cpp_v2();
	const auto result_asm = test_asm_v2();

	std::cout << "Result C++: " << result_cpp << std::endl;
	std::cout << "Result ASM: " << result_asm << std::endl << std::endl;
}

void test_3()
{
	std::cout << "Test 3 launched ... " << std::endl << std::endl;

	const std::size_t size = 10'000'000;

	auto a = new double[size];
	auto b = new double[size];

	for (std::size_t i = 0; i < size; ++i)
	{
		a[i] = 0.5;
		b[i] = 0.5;
	}

	const auto result_cpp = test_cpp_v3(a, b, size);
	const auto result_asm = test_asm_v3(a, b, size);

	std::cout << "Result C++: " << std::setprecision(6) << std::fixed << result_cpp << std::endl;
	std::cout << "Result ASM: " << std::setprecision(6) << std::fixed << result_asm << std::endl << std::endl;

	delete[] a;
	delete[] b;
}

void test_4()
{
	std::cout << "Test 4 launched ... " << std::endl << std::endl;

	const std::size_t size = 10'000'000; // note: assume size > 3 and even

	auto a = new double[size];

	for (std::size_t i = 0; i < size; ++i)
	{
		a[i] = 0.25;
	}

	const auto result_cpp = test_cpp_v4(a, size);
	const auto result_asm = test_asm_v4(a, size);

	std::cout << "Result C++: " << std::setprecision(6) << std::fixed << result_cpp << std::endl;
	std::cout << "Result ASM: " << std::setprecision(6) << std::fixed << result_asm << std::endl << std::endl;

	delete[] a;
}

int main()
{
	test_1();
	test_2();
	test_3();
	test_4();

	return 0;
}