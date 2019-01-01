#include <string>
#include <vector>

using namespace std;


class Item
{
public:
    string name;
    int sellIn;
    int quality;
    Item(string name, int sellIn, int quality) : name(name), sellIn(sellIn), quality(quality) 
    {}
};

class GildedRose
{
public:
    vector<Item> & items;
    GildedRose(vector<Item> & items);
	void updateQuality();

	static const string AGED_BRIE_NAME;
	static const string SULFURAS_NAME;
	static const string BACKSTAGE_NAME;

	static const int TWICE_AS_FAST;
	static const int NORMAL_QUALITY_ADJUSTMENT;
	static const int PASS_SALE_DATE_QUALITY_ADJUSTMENT;
	static const int AGED_BRIE_NORMAL_QUALITY_ADJUSTMENT;
	static const int AGED_BRIE_PASS_SALE_DATE_QUALITY_ADJUSTMENT;
	static const int QUALITY_LOWER_BOUND;
	static const int QUALITY_UPPER_BOUND;
private:
	bool isSaleDatePassed (Item& item) const;
	void update_Aged_Brie(Item& item) const;
	void update_Backstage(Item& item) const;
	void update_Normal(Item& item) const;
	void update_Surfuras(Item& item) const;
	void updateItemQuality (Item& item) const;
};

