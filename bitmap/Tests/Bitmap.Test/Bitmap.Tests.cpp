#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Libs/Bitmaps/Bitmaps.h"

using namespace testing;

TEST(BitmapTest, NullBitmap)
{
	std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(0);
	EXPECT_THAT(bitmap->get_bit_block(0), IsNull());
	EXPECT_THAT(bitmap->set_bit(0), IsNull());
	EXPECT_THAT(bitmap->clear_bit(0), IsNull());
	EXPECT_THAT(bitmap->is_bit_set(0), IsFalse());
}

TEST(BitmapTest, BitmapIndexOutOfRange)
{
	std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(4);
	EXPECT_THAT(bitmap->get_bit_block(4), IsNull());
	EXPECT_THAT(bitmap->set_bit(4), IsNull());
	EXPECT_THAT(bitmap->clear_bit(4), IsNull());
	EXPECT_THAT(bitmap->is_bit_set(4), IsFalse());
}

TEST(BitmapTest, BitmapIsNullInitialized)
{
	auto num_blocks = 4;

	{
		// aligns to block boundaries (4 * 4) = 16
		auto block_size = sizeof(unsigned int);
		auto total_blocks = num_blocks * block_size;

		std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(static_cast<int>(total_blocks));

		for (int i = 0; i < total_blocks; ++i)
		{
			auto p = bitmap->get_bit_block(i);

			EXPECT_THAT(p, Ne(nullptr));
			EXPECT_THAT(*p, Eq(0));
		}
	}

	{
		// one past block boundary (4 * 4) + 1 = 17
		auto block_size = sizeof(unsigned int);
		auto total_blocks = num_blocks * block_size + 1;

		std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(static_cast<int>(total_blocks));

		for (int i = 0; i < static_cast<int>(total_blocks); ++i)
		{
			auto p = bitmap->get_bit_block(i);

			EXPECT_THAT(p, Ne(nullptr));
			EXPECT_THAT(*p, Eq(0));
		}
	}

	{
		// one prior to block boundary (4 * 4) - 1 = 15
		auto block_size = sizeof(unsigned int);
		auto total_blocks = num_blocks * block_size - 1;

		std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(static_cast<int>(total_blocks));

		for (int i = 0; i < static_cast<int>(total_blocks); ++i)
		{
			auto p = bitmap->get_bit_block(i);

			EXPECT_THAT(p, Ne(nullptr));
			EXPECT_THAT(*p, Eq(0));
		}
	}
}

TEST(BitmapTest, Bitmap_SetBit_Individual)
{
	{
		// 1000
		std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(static_cast<int>(sizeof(unsigned int)));
		EXPECT_THAT(*(bitmap->set_bit(0)), Eq(8u));
	}

	{
		// 0100
		std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(static_cast<int>(sizeof(unsigned int)));
		EXPECT_THAT(*(bitmap->set_bit(1)), Eq(4u));
	}

	{
		// 0010
		std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(static_cast<int>(sizeof(unsigned int)));
		EXPECT_THAT(*(bitmap->set_bit(2)), Eq(2u));
	}

	{
		// 0001
		std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(static_cast<int>(sizeof(unsigned int)));
		EXPECT_THAT(*(bitmap->set_bit(3)), Eq(1u));
	}
}

TEST(BitmapTest, Bitmap_SetBit_Multiple)
{
	std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(5);

	// Set bits 0, 1.. 1100 = 12
	bitmap->set_bit(0);
	bitmap->set_bit(1);
	EXPECT_THAT(*(bitmap->get_bit_block(0)), Eq(12u));

	// Set bit 4.. 1000 = 8
	bitmap->set_bit(4);
	EXPECT_THAT(*(bitmap->get_bit_block(4)), Eq(8u));
}

TEST(BitmapTest, Bitmap_SetBit_SameBit)
{
	std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(5);

	// Set bits 0, 1.. 1100 = 12
	bitmap->set_bit(0);
	bitmap->set_bit(1);
	EXPECT_THAT(*(bitmap->get_bit_block(0)), Eq(12u));

	// Set bit 1.. again..
	bitmap->set_bit(1);
	EXPECT_THAT(*(bitmap->get_bit_block(0)), Eq(12u));
}

TEST(BitmapTest, Bitmap_GetBitBlock)
{
	std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(4);

	// Set bits 0, 1.. 1100 = 12
	bitmap->set_bit(0);
	bitmap->set_bit(1);
	EXPECT_THAT(*(bitmap->get_bit_block(0)), Eq(12u));
	EXPECT_THAT(*(bitmap->get_bit_block(1)), Eq(12u));
	EXPECT_THAT(*(bitmap->get_bit_block(2)), Eq(12u));
	EXPECT_THAT(*(bitmap->get_bit_block(3)), Eq(12u));

	// Sanity checks on the block boundaries
	EXPECT_THAT(bitmap->get_bit_block(4), IsNull());
}

TEST(BitmapTest, Bitmap_ClearBit)
{
	std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(4);

	// Set bits 0, 1.. 1111 = 15
	bitmap->set_bit(0);
	bitmap->set_bit(1);
	bitmap->set_bit(2);
	bitmap->set_bit(3);
	ASSERT_THAT(*(bitmap->get_bit_block(0)), Eq(15u));

	// clear bit 0.. 0111 = 7
	bitmap->clear_bit(0);
	ASSERT_THAT(*(bitmap->get_bit_block(0)), Eq(7u));

	// clear bit 1.. 0011 = 3
	bitmap->clear_bit(1);
	ASSERT_THAT(*(bitmap->get_bit_block(0)), Eq(3u));

	// clear bit 2.. 0001 = 1
	bitmap->clear_bit(2);
	ASSERT_THAT(*(bitmap->get_bit_block(0)), Eq(1u));

	// clear bit 3.. 0000 = 0
	bitmap->clear_bit(3);
	ASSERT_THAT(*(bitmap->get_bit_block(0)), Eq(0u));

	// Sanity checks on the block boundaries
	EXPECT_THAT(bitmap->get_bit_block(4), IsNull());
}

TEST(BitmapTest, Bitmap_IsBitSet)
{
	std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>(4);

	bitmap->set_bit(0);
	bitmap->set_bit(2);
	bitmap->set_bit(3);

	// 1011 = 11
	ASSERT_THAT(*(bitmap->get_bit_block(0)), Eq(11u));

	EXPECT_THAT(bitmap->is_bit_set(0), IsTrue());
	EXPECT_THAT(bitmap->is_bit_set(1), IsFalse());
	EXPECT_THAT(bitmap->is_bit_set(2), IsTrue());
	EXPECT_THAT(bitmap->is_bit_set(3), IsTrue());

	bitmap->clear_bit(0);
	bitmap->clear_bit(3);

	EXPECT_THAT(bitmap->is_bit_set(0), IsFalse());
	EXPECT_THAT(bitmap->is_bit_set(3), IsFalse());

	// Only bit 2 is currently set, 0010 = 2
	EXPECT_THAT(*(bitmap->get_bit_block(2)), Eq(2u));

	// Sanity checks on the block boundaries
	EXPECT_THAT(bitmap->is_bit_set(4), IsFalse());
}