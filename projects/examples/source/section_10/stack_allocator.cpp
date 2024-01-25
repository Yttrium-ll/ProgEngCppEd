#include <bit>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

class Stack_Allocator // note: used with deallocations at end for blocks of different sizes
{
private:

	using Header = std::uint8_t;

public:

	explicit Stack_Allocator(std::size_t size) : m_size(size)
	{
		m_begin = ::operator new(m_size);
	}

	~Stack_Allocator() noexcept
	{
		::operator delete(m_begin);
	}

	[[nodiscard]] void * allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t))
	{
		void * first = get_byte(m_begin) + m_offset;

		void * block = get_byte(first) + sizeof(Header); // note: first byte for data after header

		auto space = m_size - m_offset - sizeof(Header);

		if (block = std::align(alignment, size, block, space); block) // note: modifies block and space
		{
			auto header = get_header(get_byte(block) - sizeof(Header));

			*header = static_cast < Header > (std::distance(get_byte(first), get_byte(block)));

			m_offset = get_byte(block) - get_byte(m_begin) + size;

			return block; // note: aligned pointer
		}
		else return nullptr;
	}

	void deallocate(void * ptr) noexcept
	{
		auto header = get_header(get_byte(ptr) - sizeof(Header));

		m_offset = get_byte(ptr) - get_byte(m_begin) - *header;
	}

	void print() const
	{
		std::cout << m_begin << ": ";

		std::cout << std::setw(4) << std::right << std::setfill('0') << m_offset;

		std::cout << " / " << m_size << std::endl;
	}

private:

	std::byte * get_byte(void * ptr) const noexcept
	{
		return std::bit_cast < std::byte * > (ptr);
	}

	Header * get_header(void * ptr) const noexcept
	{
		return std::bit_cast < Header * > (ptr);
	}

private:

	std::size_t m_size   = 0;
	std::size_t m_offset = 0;

	void * m_begin = nullptr;

}; // class Stack_Allocator

int main()
{
	Stack_Allocator allocator(1024);

	auto ptr_0 = allocator.allocate( 1, 4); std::cout << ptr_0 << ' '; allocator.print(); // note: A
	auto ptr_1 = allocator.allocate( 2, 2); std::cout << ptr_1 << ' '; allocator.print(); // note: B
	auto ptr_2 = allocator.allocate(10   ); std::cout << ptr_2 << ' '; allocator.print(); // note: C
	auto ptr_3 = allocator.allocate( 4   ); std::cout << ptr_3 << ' '; allocator.print(); // note: D

	// note: 000H AHBB | 0000 000H | CCCC CCCC | CC00 000H | DDDD 0000 | ...

	allocator.deallocate(ptr_3);    std::cout << std::string(17, ' '); allocator.print();
	allocator.deallocate(ptr_2);    std::cout << std::string(17, ' '); allocator.print();

	// note: 000H AHBB | ...

	auto ptr_4 = allocator.allocate( 3, 4); std::cout << ptr_4 << ' '; allocator.print(); // note: E
	auto ptr_5 = allocator.allocate( 8   ); std::cout << ptr_5 << ' '; allocator.print(); // note: F

	// note: 000H AHBB | 000H EEEH | FFFF FFFF | ...

	return 0;
}