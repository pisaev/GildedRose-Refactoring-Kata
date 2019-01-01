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

class UpdatableItem
{
public:
	explicit UpdatableItem(Item& item): item_{item}{}
	virtual auto update () const -> void = 0;
	virtual ~UpdatableItem() = default;

protected:
	Item& item_;

};

class Surfuras:public UpdatableItem
{
public:
	explicit Surfuras(Item& item): UpdatableItem{item}
	{
	} ;
	auto update() const -> void override {	}
};

class AgedBrie :public UpdatableItem
{
public:
	explicit AgedBrie(Item& item): UpdatableItem{item}
	{
	}

	auto update() const -> void override
	{
		item_.sellIn -= 1;
		if (item_.quality < GildedRose::QUALITY_UPPER_BOUND)
		{
			item_.quality += 1;
		}

		if (item_.sellIn < 0)
		{
			if (item_.quality < GildedRose::QUALITY_UPPER_BOUND)
			{
				item_.quality += 1;
			}
		}
	}
};

class BackstagePasses :public UpdatableItem
{
public:
	explicit BackstagePasses(Item& item): UpdatableItem{item}{}

	auto update() const -> void	override
	{
		item_.sellIn -= 1;
		if (item_.quality < GildedRose::QUALITY_UPPER_BOUND)
		{
			item_.quality += 1;

			if (item_.sellIn < 10)
			{
				if (item_.quality < GildedRose::QUALITY_UPPER_BOUND)
				{
					item_.quality += 1;
				}
			}

			if (item_.sellIn < 5)
			{
				if (item_.quality < GildedRose::QUALITY_UPPER_BOUND)
				{
					item_.quality += 1;
				}
			}
		}
		if (item_.sellIn < 0)
		{
			item_.quality -= item_.quality;
		}

	}
};

class NormalItem :public UpdatableItem
{
public:
	NormalItem(Item& item): UpdatableItem{item}
	{
	}

	auto update () const -> void override
	{
		item_.sellIn -= 1;
		if (item_.quality > GildedRose::QUALITY_LOWER_BOUND)
		{
			item_.quality -= 1;
		}
		if (item_.sellIn < 0)
		{
			if (item_.quality > GildedRose::QUALITY_LOWER_BOUND)
			{
				item_.quality -= 1;
			}
		}
	}
};

void GildedRose::updateItemQuality (Item& item) const
{
	if (item.name == SULFURAS_NAME)
	{
		Surfuras surfuras (item);
		surfuras.update();
	}
	else if (item.name == AGED_BRIE_NAME)
	{
		AgedBrie aged_brie (item);
		aged_brie.update();
	}
	else if (item.name == BACKSTAGE_NAME)
	{
		BackstagePasses backstage_passes (item);
		backstage_passes.update();
	}
	else
	{
		NormalItem normal_item (item);
		normal_item .update();
	}
}


void GildedRose::updateQuality()
{
	for (auto& item : items)
	{
		updateItemQuality(item);
	}
}
