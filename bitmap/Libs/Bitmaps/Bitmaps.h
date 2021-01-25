// -----------------------------------------------------------------------
// Author:			Trent Whiting
// Description:		Bitmap implementation with a contiguous memory layout
// -----------------------------------------------------------------------

#pragma once
#include <memory>
#include <tuple>

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN             

class Bitmap
{
private:

	std::unique_ptr<unsigned int*> _bitmap;
	const unsigned int _bitmap_size;
	const unsigned int _block_size;

	//
	// Given the bit index, get the whole block the bit is stored in
	//
	std::tuple<unsigned int *, unsigned int> locate_bit(const unsigned int i) const;

	//
	// Calculate the bitmask for the current block
	//
	unsigned int get_bitmask(const unsigned int remainder) const;

public:

	Bitmap(const unsigned int bitmap_size);
	~Bitmap();

	//
	// Set the specified bit
	//
	unsigned int* set_bit(const unsigned int bit_index) const;

	//
	// Clears the specified bit if it has been set
	//
	unsigned int* clear_bit(unsigned int bit_index) const;

	//
	// Get the block that the bit is located in
	//
	unsigned int* get_bit_block(const unsigned int bit_index) const;

	//
	// Returns true if the specified bit has been set
	//
	bool is_bit_set(const unsigned int bit_index) const;
};
