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
	explicit UpdatableItem(Item& item): item_{item}{}
	virtual auto update () const -> void = 0;
	virtual ~UpdatableItem() = default;

protected:
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

class Surfuras:public UpdatableItem
{
public:
	explicit Surfuras(Item& item): UpdatableItem{item}{}

	auto update() const -> void override {}
};

class AgedBrie :public UpdatableItem
{
public:
	explicit AgedBrie(Item& item): UpdatableItem{item}{}

	void updateQuality() const
	{
		int qualityAdjustment = GildedRose::AGED_BRIE_NORMAL_QUALITY_ADJUSTMENT;

		if (item_.sellIn < 0)
		{
			if (item_.quality < GildedRose::QUALITY_UPPER_BOUND)
			{
				qualityAdjustment = GildedRose::AGED_BRIE_PASS_SALE_DATE_QUALITY_ADJUSTMENT;
			}
		}
		AdjustQuality(qualityAdjustment);
	}

	auto update() const -> void override
	{
		Age();
		updateQuality();
	}
};

class BackstagePasses :public UpdatableItem
{
public:
	explicit BackstagePasses(Item& item): UpdatableItem{item}{}

	void updateQuality() const
	{
		AdjustQuality (1);

		if (item_.sellIn < 10)
		{
			AdjustQuality (1);
		}

		if (item_.sellIn < 5)
		{
			AdjustQuality (1);
		}
		if (item_.sellIn < 0)
		{
			AdjustQuality (-item_.quality);
		}
	}

	auto update() const -> void	override
	{
		Age ();

		updateQuality();
	}
};

class NormalItem :public UpdatableItem
{
public:
	NormalItem(Item& item): UpdatableItem{item}{}

	void updateQuality() const
	{
		if (item_.quality > GildedRose::QUALITY_LOWER_BOUND)
		{
			AdjustQuality (-1);
		}
		if (item_.sellIn < 0)
		{
			AdjustQuality (-1);
		}
	}

	auto update () const -> void override
	{
		Age ();
		updateQuality();
	}
};

unique_ptr<UpdatableItem> CreateUpdatableItem(Item& item)
{
	if (item.name == GildedRose::SULFURAS_NAME)
	{
		return make_unique<Surfuras > (item);
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
