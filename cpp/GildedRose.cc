#include	"GildedRose.h"
#include <memory>

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
	explicit UpdatableItem() {}
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
		return GildedRose::PASS_SALE_DATE_QUALITY_ADJUSTMENT;
	}

	void updateQuality () const
	{
		int qualityAdjustment = NormalQualityAdjustment ();

		if (item_.sellIn < 0)
		{
			qualityAdjustment = PassDateQualityAdjustment ();
		}
		AdjustQuality (qualityAdjustment);
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

	Item& item_;
};

class AgedBrie :public NormalItem
{
public:
	explicit AgedBrie(Item& item): NormalItem{item}{}
protected:
	int NormalQualityAdjustment() const override
	{
		return GildedRose::AGED_BRIE_NORMAL_QUALITY_ADJUSTMENT;
	}

	int PassDateQualityAdjustment() const override
	{
		return GildedRose::AGED_BRIE_PASS_SALE_DATE_QUALITY_ADJUSTMENT;
	}
};

class BackstagePasses :public NormalItem
{
public:
	explicit BackstagePasses(Item& item): NormalItem{item}{}
protected:
	int NormalQualityAdjustment () const override
	{
		int qualityAdjustment = 1;

		if (item_.sellIn < 10)
		{
			qualityAdjustment = 2;
		}

		if (item_.sellIn < 5)
		{
			qualityAdjustment = 3;
		}
		return qualityAdjustment;
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

void GildedRose::updateQuality()
{
	for (auto& item : items)
	{
		CreateUpdatableItem (item)->update ();
	}
}
