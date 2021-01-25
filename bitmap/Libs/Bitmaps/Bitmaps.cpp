#pragma once
#include "pch.h"
#include "Bitmaps.h"

Bitmap::Bitmap(const unsigned int bitmap_size) : _bitmap_size(bitmap_size), _block_size(sizeof(unsigned int)), _bitmap(nullptr)
{
	if (bitmap_size > 0)
	{
		unsigned int blocks = 0;

		if (bitmap_size <= _block_size)
		{
			blocks = 1;
		}
		else
		{
			unsigned int remainder = bitmap_size % _block_size;

			if (remainder == 0)
			{
				blocks = (bitmap_size / _block_size);
			}
			else
			{
				blocks = (bitmap_size / _block_size) + 1;
			}

		}

		_bitmap = std::make_unique<unsigned int*>(new unsigned int[blocks]);

		for (unsigned int i = 0; i < blocks; i++)
		{
			(*_bitmap.get())[i] = 0;
		}
	}
}

Bitmap::~Bitmap()
{
	_bitmap.reset();
}

std::tuple<unsigned int *, unsigned int> Bitmap::locate_bit(const unsigned int i) const
{
	unsigned int remainder = i % _block_size;
	unsigned int block = i / _block_size;

	return { (*_bitmap.get()) + block, remainder};
}

unsigned int Bitmap::get_bitmask(const unsigned int remainder) const
{
	return 1 << (_block_size - 1) - remainder;
}

unsigned int * Bitmap::get_bit_block(const unsigned int bit_index) const
{
	if (_bitmap == nullptr)
	{
		return nullptr;
	}

	if (bit_index > _bitmap_size - 1)
	{
		return nullptr;
	}

	auto [p, r] = locate_bit(bit_index);

	return { p };
}

unsigned int* Bitmap::set_bit(const unsigned int bit_index) const
{
	if (_bitmap == nullptr)
	{
		return nullptr;
	}

	if (bit_index > _bitmap_size - 1)
	{
		return nullptr;
	}

	auto [p, r] = locate_bit(bit_index);

	auto bitmask = get_bitmask(r);
	*p |= bitmask;

	return p;
}

bool Bitmap::is_bit_set(const unsigned int bit_index) const
{
	if (_bitmap == nullptr)
	{
		return false;
	}

	if (bit_index > _bitmap_size - 1)
	{
		return false;
	}

	auto [p, r] = locate_bit(bit_index);

	auto block = *p;
	auto bitmask = get_bitmask(r);
	block &= bitmask;

	return block != 0;
}

unsigned int* Bitmap::clear_bit(unsigned int bit_index) const
{
	if (_bitmap == nullptr)
	{
		return nullptr;
	}

	if (bit_index > _bitmap_size - 1)
	{
		return nullptr;
	}

	auto [p , r] = locate_bit(bit_index);

	auto bitmask = get_bitmask(r);
	*p &= ~(bitmask);
	
	return p;
}
