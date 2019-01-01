#include "GildedRose.h"

GildedRose::GildedRose(vector<Item>& items) : items(items){}

void GildedRose::updateItemQuality (Item& item) const
{
	if (item.name == "Aged Brie")
	{
		if (item.quality < 50)
		{
			item.quality += 1;
		}
	}
	else if (item.name == "Backstage passes to a TAFKAL80ETC concert")
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
	else
	{
		if (item.quality > 0)
		{
			if (item.name != "Sulfuras, Hand of Ragnaros")
			{
				item.quality -= 1;
			}
		}
	}

	if (item.name != "Sulfuras, Hand of Ragnaros")
	{
		item.sellIn -= 1;
	}

	if (item.sellIn < 0)
	{
		if (item.name == "Aged Brie")
		{
			if (item.quality < 50)
			{
				item.quality += 1;
			}
		}
		else
		{
			if (item.name == "Backstage passes to a TAFKAL80ETC concert")
			{
				item.quality -= item.quality;
			}
			else
			{
				if (item.quality > 0)
				{
					if (item.name != "Sulfuras, Hand of Ragnaros")
					{
						item.quality -= 1;
					}
				}
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
