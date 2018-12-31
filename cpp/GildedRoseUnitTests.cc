#include <gtest/gtest.h>

#include "GildedRose.h"
#include <ostream>

template<typename Container, typename Pred>
bool contains(Container list, Pred pred)
{
	return find_if (list.cbegin (),
		list.cend (),
		pred) != list.cend ();
}

bool operator==(const Item& lhs, const Item rhs)
{
	return lhs.name == rhs.name &&
			lhs.sellIn == rhs.sellIn &&
			lhs.quality == rhs.quality;
}

struct GildedRoseTest : testing::Test
{
	const int TWICE_AS_FAST = 2;
	const int NORMAL_QUALITY_ADJUSTMENT = -1;
	const int PASS_SALE_DATE_QUALITY_ADJUSTMENT = TWICE_AS_FAST * NORMAL_QUALITY_ADJUSTMENT;
	const int AGED_BRIE_NORMAL_QUALITY_ADJUSTMENT = -NORMAL_QUALITY_ADJUSTMENT;
	const int AGED_BRIE_PASS_SALE_DATE_QUALITY_ADJUSTMENT = TWICE_AS_FAST * AGED_BRIE_NORMAL_QUALITY_ADJUSTMENT;
	const int QUALITY_LOWER_BOUND = 0;
	const int QUALITY_UPPER_BOUND = 50;

	const string REGULAR_ITEM1_NAME = "Foo";
	const string REGULAR_ITEM2_NAME = "Boo";
	const string AGED_BRIE_NAME = "Aged Brie";
	const string SULFURAS_NAME = "Sulfuras, Hand of Ragnaros";
	const string BACKSTAGE_NAME = "Backstage passes to a TAFKAL80ETC concert";

	auto matchItemByName(string name_to_look) const {
		return[name_to_look = move (name_to_look)] (auto& item) {
			return item.name == name_to_look;
		};
	};

	void givenItemWithNameSellInAndQuality(Item newItem)
	{
		items.emplace_back (newItem);
	}

	auto whenWeUpdateTheQuality()
	{
		app.updateQuality ();
	}

	auto find_item_by(const string& name) const
	{
		EXPECT_TRUE (contains (app.items, matchItemByName (name)));
		const auto item_p = find_if (app.items.cbegin (),
			app.items.cend (),
			matchItemByName(name));
		return *item_p;
	}

	vector<Item> items;
	GildedRose app{ items };
};

TEST_F(GildedRoseTest, elements_exist_after_updateQuality) {
    
	givenItemWithNameSellInAndQuality ({ REGULAR_ITEM1_NAME,5,6 });
	givenItemWithNameSellInAndQuality ({ REGULAR_ITEM2_NAME,20,4 });

	whenWeUpdateTheQuality ();

	EXPECT_TRUE (contains(app.items, matchItemByName(REGULAR_ITEM1_NAME)));
	EXPECT_TRUE (contains(app.items, matchItemByName(REGULAR_ITEM2_NAME)));
}

TEST_F (GildedRoseTest, each_day_our_system_lowers_SellIn_and_Quality_for_every_item)
{
	const int SOME_SELL_IN = 5;
	const int SOME_QUALITY = 6;

	givenItemWithNameSellInAndQuality ({ REGULAR_ITEM1_NAME,SOME_SELL_IN,SOME_QUALITY });
	givenItemWithNameSellInAndQuality ({ REGULAR_ITEM2_NAME,SOME_SELL_IN,SOME_QUALITY });

	whenWeUpdateTheQuality ();

	EXPECT_EQ (NORMAL_QUALITY_ADJUSTMENT, find_item_by (REGULAR_ITEM1_NAME).quality - SOME_QUALITY);
	EXPECT_EQ (SOME_SELL_IN - 1, find_item_by (REGULAR_ITEM1_NAME).sellIn);
	EXPECT_EQ (NORMAL_QUALITY_ADJUSTMENT, find_item_by (REGULAR_ITEM2_NAME).quality - SOME_QUALITY);
	EXPECT_EQ (SOME_SELL_IN - 1, find_item_by (REGULAR_ITEM2_NAME).sellIn);
}

TEST_F (GildedRoseTest, once_the_sell_by_date_has_passed,_quality_degrades_twice_as_fast)
{
	const int SELL_IN_DATE_PASSED = -1;
	const int SELL_IN_BEFORE_PASSING = SELL_IN_DATE_PASSED + 1;
	const int SOME_QUALITY = 6;

	givenItemWithNameSellInAndQuality ({ REGULAR_ITEM1_NAME,SELL_IN_BEFORE_PASSING,SOME_QUALITY });

	whenWeUpdateTheQuality ();

	EXPECT_EQ (PASS_SALE_DATE_QUALITY_ADJUSTMENT, find_item_by (REGULAR_ITEM1_NAME).quality - SOME_QUALITY);
	EXPECT_EQ (SELL_IN_DATE_PASSED, find_item_by (REGULAR_ITEM1_NAME).sellIn);
}

TEST_F (GildedRoseTest, the_quality_of_an_item_is_never_negative)
{
	const int SOME_SELL_IN = 7;

	givenItemWithNameSellInAndQuality ({ REGULAR_ITEM1_NAME,SOME_SELL_IN,QUALITY_LOWER_BOUND });

	whenWeUpdateTheQuality ();

	EXPECT_EQ (QUALITY_LOWER_BOUND, find_item_by (REGULAR_ITEM1_NAME).quality);
}

TEST_F (GildedRoseTest, Aged_Brie_actually_increases_in_Quality_the_older_it_gets)
{
	const int SOME_QUALITY = 4;
	const int SOME_SELL_IN = 7;
	givenItemWithNameSellInAndQuality ({ AGED_BRIE_NAME,SOME_SELL_IN,SOME_QUALITY });

	whenWeUpdateTheQuality ();

	EXPECT_EQ (AGED_BRIE_NORMAL_QUALITY_ADJUSTMENT, find_item_by (AGED_BRIE_NAME).quality - SOME_QUALITY);
}

TEST_F (GildedRoseTest, Aged_Brie_increases_in_Quality_once_the_sell_by_date_has_passed_twice_as_fast)
{
	const int SELL_IN_BEFORE_PASSING = 0;
	const int SOME_QUALITY = 6;
	givenItemWithNameSellInAndQuality ({ AGED_BRIE_NAME,SELL_IN_BEFORE_PASSING,SOME_QUALITY });

	whenWeUpdateTheQuality ();

	EXPECT_EQ (AGED_BRIE_PASS_SALE_DATE_QUALITY_ADJUSTMENT, find_item_by (AGED_BRIE_NAME).quality - SOME_QUALITY);
}

TEST_F (GildedRoseTest, Aged_Brie_the_quality_of_an_item_is_never_more_than_50)
{
	const int SOME_SELL_IN = 7;
	givenItemWithNameSellInAndQuality ({ AGED_BRIE_NAME,SOME_SELL_IN, QUALITY_UPPER_BOUND });	
	
	whenWeUpdateTheQuality ();

	EXPECT_EQ (QUALITY_UPPER_BOUND, find_item_by (AGED_BRIE_NAME).quality);
}

TEST_F (GildedRoseTest, the_quality_of_an_item_is_never_more_than_50_after_sale_date_passed_as_well)
{
	const int SELL_IN_BEFORE_PASSING = 0;
	givenItemWithNameSellInAndQuality ({ AGED_BRIE_NAME,SELL_IN_BEFORE_PASSING, QUALITY_UPPER_BOUND });

	whenWeUpdateTheQuality ();

	EXPECT_EQ (QUALITY_UPPER_BOUND, find_item_by (AGED_BRIE_NAME).quality);
}

TEST_F (GildedRoseTest, Sulfuras_being_a_legendary_item,_never_has_to_be_sold_or_decreases_in_Quality)
{
	const int SOME_CURRENT_QUALITY = 30;
	const int SOME_SELL_IN = 10;
	givenItemWithNameSellInAndQuality ({ SULFURAS_NAME,SOME_SELL_IN, SOME_CURRENT_QUALITY });

	whenWeUpdateTheQuality ();

	EXPECT_EQ (SOME_CURRENT_QUALITY, find_item_by (SULFURAS_NAME).quality);
	EXPECT_EQ (SOME_SELL_IN, find_item_by (SULFURAS_NAME).sellIn);
}

TEST_F (GildedRoseTest, Sulfuras_Quality_not_limited_by_upper_bound)
{
	const int SOME_LARGE_QUALITY = QUALITY_UPPER_BOUND + 30;
	const int SOME_SELL_IN = 15;
	givenItemWithNameSellInAndQuality ({ SULFURAS_NAME,SOME_SELL_IN, SOME_LARGE_QUALITY });

	whenWeUpdateTheQuality ();

	EXPECT_EQ (SOME_LARGE_QUALITY, find_item_by (SULFURAS_NAME).quality);
}

struct Backstage_Passes
{
	friend auto operator<<(std::ostream& os, const Backstage_Passes& obj) -> std::ostream&
	{
		return os
			<< "sellIn: " << obj.sellIn
			<< " current_quality: " << obj.current_quality
			<< " expected_quality_adjustment: " << obj.expected_quality_adjustment;
	}

	int sellIn;
	int current_quality;
	int expected_quality_adjustment;
};

class Backstage_Passes_Test: public GildedRoseTest, public ::testing::WithParamInterface<Backstage_Passes>{};
TEST_P (Backstage_Passes_Test, increases_in_Quality)
{
	const int SOME_CURRENT_QUALITY = GetParam().current_quality;
	const int SOME_SELL_IN = GetParam().sellIn;
	givenItemWithNameSellInAndQuality({ BACKSTAGE_NAME, SOME_SELL_IN, SOME_CURRENT_QUALITY });

	whenWeUpdateTheQuality();

	EXPECT_EQ (GetParam().expected_quality_adjustment, find_item_by (BACKSTAGE_NAME).quality - SOME_CURRENT_QUALITY);
}

INSTANTIATE_TEST_CASE_P (Backstage_Passes_increases_in_Quality_the_older_it_gets, Backstage_Passes_Test, ::testing::ValuesIn({
	Backstage_Passes{11,4, 1},
	Backstage_Passes{15,4, 1},
	Backstage_Passes{15,49,1},
	}));

INSTANTIATE_TEST_CASE_P (Backstage_Passes_increases_in_Quality_by_two_when_there_are_between_6_and_10_days, Backstage_Passes_Test, ::testing::ValuesIn({
	Backstage_Passes{6,4, 2},
	Backstage_Passes{10,4,2},
	Backstage_Passes{8,48,2},
	}));

INSTANTIATE_TEST_CASE_P (Backstage_Passes_increases_in_Quality_by_three_when_there_are_5_days_or_less, Backstage_Passes_Test, ::testing::ValuesIn({
	Backstage_Passes{1,4, 3},
	Backstage_Passes{5,4, 3},
	Backstage_Passes{3,47,3},
	}));

INSTANTIATE_TEST_CASE_P (Backstage_Passes_the_quality_of_an_item_is_never_more_than_50, Backstage_Passes_Test, ::testing::ValuesIn ({
	Backstage_Passes{1, 47, 50 - 47},
	Backstage_Passes{5, 48, 50 - 48},
	Backstage_Passes{3, 49, 50 - 49},
	Backstage_Passes{3, 50, 50 - 50},
	}));

INSTANTIATE_TEST_CASE_P (Backstage_Passes_Quality_drops_to_0_after_the_concert, Backstage_Passes_Test, ::testing::ValuesIn ({
	Backstage_Passes{0,  7, -7},
	Backstage_Passes{-1, 4, -4},
	Backstage_Passes{-3, 5, -5},
	}));
