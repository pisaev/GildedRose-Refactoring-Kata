def createUpdatableItem(item):
    if item.name == "Aged Brie":
        return AgedBrie(item)
    if item.name == "Backstage passes to a TAFKAL80ETC concert":
        return BackstagePasses(item)
    if item.name == "Sulfuras, Hand of Ragnaros":
        return Sulfuras(item)
    return Normal(item)


class GildedRose(object):

    def __init__(self, items):
        self.items = items

    def update_quality(self):
        for item in self.items:
            createUpdatableItem(item).update_quality()


class UpdatableItem:
    NORMAL_ITEM_UPDATE_QUALITY = -1
    TWICE_AS_FAST = 2
    PASS_SALE_DATE_QUALITY_ADJUSTMENT = TWICE_AS_FAST * NORMAL_ITEM_UPDATE_QUALITY

    def __init__(self, item):
        self.item = item

    def update_quality(self):
        self.ageItem()
        self._update_quality()

    def ageItem(self):
        self.item.sell_in -= 1

    def adjust_quantity_with_limit(self, quantity, lower_limit=0, upper_limit=50):
        self.item.quality += quantity

        if self.item.quality > upper_limit:
            self.item.quality = upper_limit

        if self.item.quality < lower_limit:
            self.item.quality = lower_limit

    def sell_by_date_passed(self):
        return self.item.sell_in < 0

    def _update_quality(self):
        if self.sell_by_date_passed():
            self.adjust_quantity_with_limit(self._afterSellDateUpdate())
        else:
            self.adjust_quantity_with_limit(self._beforeSellDateUpdate())

    def _afterSellDateUpdate(self):
        pass

    def _beforeSellDateUpdate(self):
        pass


class Sulfuras(UpdatableItem):
    def __init__(self, item):
        super().__init__(item)

    def ageItem(self):
        pass

    def _update_quality(self):
        return


class BackstagePasses(UpdatableItem):
    def __init__(self, item):
        super().__init__(item)

    def _afterSellDateUpdate(self):
        return -self.item.quality

    def _beforeSellDateUpdate(self):
        if self.item.sell_in < 5:
            return 3

        if self.item.sell_in < 10:
            return 2

        return -self.NORMAL_ITEM_UPDATE_QUALITY


class AgedBrie(UpdatableItem):

    def __init__(self, item):
        super().__init__(item)

    def _afterSellDateUpdate(self):
        return -self.PASS_SALE_DATE_QUALITY_ADJUSTMENT

    def _beforeSellDateUpdate(self):
        return -self.NORMAL_ITEM_UPDATE_QUALITY


class Normal(UpdatableItem):
    def __init__(self, item):
        super().__init__(item)

    def _afterSellDateUpdate(self):
        return self.PASS_SALE_DATE_QUALITY_ADJUSTMENT

    def _beforeSellDateUpdate(self):
        return self.NORMAL_ITEM_UPDATE_QUALITY


class Item:
    def __init__(self, name, sell_in, quality):
        self.name = name
        self.sell_in = sell_in
        self.quality = quality

    def __repr__(self):
        return "%s, %s, %s" % (self.name, self.sell_in, self.quality)
