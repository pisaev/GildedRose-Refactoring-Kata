#include	"GildedRose.h"
#include <memory>

const string GildedRose::AGED_BRIE_NAME = "Aged Brie";
const string GildedRose::SULFURAS_NAME = "Sulfuras, Hand of Ragnaros";
const string GildedRose::BACKSTAGE_NAME = "Backstage passes to a TAFKAL80ETC concert";

const int GildedRose::TWICE_AS_FAST = 2;
const int GildedRose::NORMAL_QUALITY_ADJUSTMENT = -1;
const int GildedRose::PASS_SALE_DATE_QUALITY_ADJUSTMENT = TWICE_AS_FAST * NORMAL_QUALITY_ADJUSTMENT;
const int GildedRose::QUALITY_LOWER_BOUND = 0;
const int GildedRose::QUALITY_UPPER_BOUND = 50;


class UpdatableItem
{
public:
	virtual auto update () const -> void {};
	virtual ~UpdatableItem() = default;
};

using Surfuras = UpdatableItem;

class NormalItem :public UpdatableItem
{
public:
	NormalItem (Item& item) : item_{ item } {}

	auto update () const -> void override
	{
		Age ();
		updateQuality ();
	}

protected:
	virtual int NormalQualityAdjustment () const
	{
		return GildedRose::NORMAL_QUALITY_ADJUSTMENT;
	}

	virtual int PassDateQualityAdjustment () const
	{
		return GildedRose::TWICE_AS_FAST* NormalQualityAdjustment ();
	}

	Item& item_;

private:
	void updateQuality () const
	{
		if (isSaleDatePasses ())
		{
			AdjustQuality (PassDateQualityAdjustment ());
		}
		else
		{
			AdjustQuality (NormalQualityAdjustment ());
		}
	}

	bool isSaleDatePasses () const
	{
		return item_.sellIn < 0;
	}

	void Age () const
	{
		item_.sellIn -= 1;
	}

	void AdjustQuality (int qualityAdjustment) const
	{
		item_.quality += qualityAdjustment;

		if (item_.quality > GildedRose::QUALITY_UPPER_BOUND) item_.quality = GildedRose::QUALITY_UPPER_BOUND;
		if (item_.quality < GildedRose::QUALITY_LOWER_BOUND) item_.quality = GildedRose::QUALITY_LOWER_BOUND;
	}
};



class AgedBrie :public NormalItem
{
public:
	explicit AgedBrie(Item& item): NormalItem{item}{}
protected:
	int NormalQualityAdjustment() const override
	{
		return -NormalItem::NormalQualityAdjustment();
	}
};

class BackstagePasses :public NormalItem
{
public:
	explicit BackstagePasses(Item& item): NormalItem{item}{}
protected:
	int NormalQualityAdjustment () const override
	{
		if (item_.sellIn < 5)
		{
			return 3;
		}

		if (item_.sellIn < 10)
		{
			return  2;
		}

		return 1;
	}

	int PassDateQualityAdjustment() const override
	{
		return -item_.quality;
	}
};

unique_ptr<UpdatableItem> CreateUpdatableItem(Item& item)
{
	if (item.name == GildedRose::SULFURAS_NAME)
	{
		return make_unique<Surfuras> ();
	}
	if (item.name == GildedRose::AGED_BRIE_NAME)
	{
		return make_unique<AgedBrie> (item);
	}
	if (item.name == GildedRose::BACKSTAGE_NAME)
	{
		return make_unique<BackstagePasses> (item);
	}
	return make_unique<NormalItem> (item);
}

GildedRose::GildedRose (vector<Item>& items) : items (items) {}

void GildedRose::updateQuality()
{
	for (auto& item : items)
	{
		CreateUpdatableItem (item)->update ();
	}
}
