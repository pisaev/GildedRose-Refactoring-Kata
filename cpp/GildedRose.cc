#include	"GildedRose.h"

const string GildedRose::AGED_BRIE_NAME = "Aged Brie";
const string GildedRose::SULFURAS_NAME = "Sulfuras, Hand of Ragnaros";
const string GildedRose::BACKSTAGE_NAME = "Backstage passes to a TAFKAL80ETC concert";

const int GildedRose::TWICE_AS_FAST = 2;
const int GildedRose::NORMAL_QUALITY_ADJUSTMENT = -1;
const int GildedRose::PASS_SALE_DATE_QUALITY_ADJUSTMENT = TWICE_AS_FAST * NORMAL_QUALITY_ADJUSTMENT;
const int GildedRose::AGED_BRIE_NORMAL_QUALITY_ADJUSTMENT = -NORMAL_QUALITY_ADJUSTMENT;
const int GildedRose::AGED_BRIE_PASS_SALE_DATE_QUALITY_ADJUSTMENT = TWICE_AS_FAST * AGED_BRIE_NORMAL_QUALITY_ADJUSTMENT;
const int GildedRose::QUALITY_LOWER_BOUND = 0;
const int GildedRose::QUALITY_UPPER_BOUND = 50;


GildedRose::GildedRose(vector<Item>& items) : items(items){}

bool GildedRose::isSaleDatePassed(Item& item) const
{
	return item.sellIn < 0;
}

void GildedRose::updateItemQuality (Item& item) const
{
	if (item.name == SULFURAS_NAME)
	{
	}
	else
	{
		item.sellIn -= 1;
	}

	if (item.name == AGED_BRIE_NAME)
	{
		if (item.quality < QUALITY_UPPER_BOUND)
		{
			item.quality += 1;
		}
	}
	else if (item.name == BACKSTAGE_NAME)
	{
		if (item.quality < QUALITY_UPPER_BOUND)
		{
			item.quality += 1;

			if (item.sellIn < 10)
			{
				if (item.quality < QUALITY_UPPER_BOUND)
				{
					item.quality += 1;
				}
			}

			if (item.sellIn < 5)
			{
				if (item.quality < QUALITY_UPPER_BOUND)
				{
					item.quality += 1;
				}
			}
		}
	}
	else if (item.name == SULFURAS_NAME)
	{
	}
	else
	{
		if (item.quality > QUALITY_LOWER_BOUND)
		{
			item.quality -= 1;
		}
	}

	if (item.name == AGED_BRIE_NAME)
	{
		if (isSaleDatePassed (item))
		{
			if (item.quality < QUALITY_UPPER_BOUND)
			{
				item.quality += 1;
			}
		}
	}
	else if (item.name == BACKSTAGE_NAME)
	{
		if (item.sellIn < 0)
		{
			item.quality -= item.quality;
		}
	}
	else if (item.name == SULFURAS_NAME)
	{
	}
	else
	{
		if (isSaleDatePassed(item))
		{
			if (item.quality > QUALITY_LOWER_BOUND)
			{
				item.quality -= 1;
			}
		}
	}
}


void GildedRose::updateQuality()
{
	for (auto& item : items)
	{
		updateItemQuality(item);
	}
}
