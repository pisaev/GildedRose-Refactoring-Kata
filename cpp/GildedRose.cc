#include "GildedRose.h"

const string GildedRose::AGED_BRIE_NAME = "Aged Brie";
const string GildedRose::SULFURAS_NAME = "Sulfuras, Hand of Ragnaros";
const string GildedRose::BACKSTAGE_NAME = "Backstage passes to a TAFKAL80ETC concert";

GildedRose::GildedRose(vector<Item>& items) : items(items){}

void GildedRose::updateItemQuality (Item& item) const
{
	if (item.name == AGED_BRIE_NAME)
	{
		if (item.quality < 50)
		{
			item.quality += 1;
		}
	}
	else if (item.name == BACKSTAGE_NAME)
	{
		if (item.quality < 50)
		{
			item.quality += 1;

			if (item.sellIn < 11)
			{
				if (item.quality < 50)
				{
					item.quality += 1;
				}
			}

			if (item.sellIn < 6)
			{
				if (item.quality < 50)
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
		if (item.quality > 0)
		{
			item.quality -= 1;
		}
	}

	if (item.name == SULFURAS_NAME)
	{
	}
	else
	{
		item.sellIn -= 1;
	}

	if (item.sellIn < 0)
	{
		if (item.name == AGED_BRIE_NAME)
		{
			if (item.quality < 50)
			{
				item.quality += 1;
			}
		}
		else if (item.name == BACKSTAGE_NAME)
		{
			item.quality -= item.quality;
		}
		else if (item.name == SULFURAS_NAME)
		{
		}
		else
		{
			if (item.quality > 0)
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
