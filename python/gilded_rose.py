# -*- coding: utf-8 -*-
class UpdatableItem:
    def __init__(self, item):
        self.item = item


    def update(self):
        self.Ageing()
        self.update_quality()

    def update_quality(self):
        pass

    def Ageing(self):
        self.item.sell_in -= 1


class Normal(UpdatableItem):
    def __init__(self, item):
        super().__init__(item)

    def update_quality(self):
        if self.item.quality > 0:
            self.item.quality = self.item.quality - 1
        if self.item.sell_in < 0:
            if self.item.quality > 0:
                self.item.quality = self.item.quality - 1


class Backstage(UpdatableItem):
    def __init__(self, item):
        super().__init__(item)

    def update_quality(self):
        if self.item.quality < 50:
            self.item.quality = self.item.quality + 1
            if self.item.sell_in < 10:
                if self.item.quality < 50:
                    self.item.quality = self.item.quality + 1
            if self.item.sell_in < 5:
                if self.item.quality < 50:
                    self.item.quality = self.item.quality + 1
        if self.item.sell_in < 0:
            self.item.quality = self.item.quality - self.item.quality


class AgedBrie(UpdatableItem):
    def __init__(self, item):
        super().__init__(item)


    def update_quality(self):
        if self.item.quality < 50:
            self.item.quality = self.item.quality + 1
        if self.item.sell_in < 0:
            if self.item.quality < 50:
                self.item.quality = self.item.quality + 1


class Sulfuras(UpdatableItem):
    def __init__(self, item):
        super().__init__(item)

    def update(self):
        pass


def createUpdatableItem(item):
    if item.name == "Aged Brie":
        return AgedBrie(item)
    if item.name == "Backstage passes to a TAFKAL80ETC concert":
        return Backstage(item)
    if item.name == "Sulfuras, Hand of Ragnaros":
        return Sulfuras(item)
    return Normal(item)


class GildedRose(object):

    def __init__(self, items):
        self.items = items

    def update_quality(self):
        for item in self.items:
            createUpdatableItem(item).update()


class Item:
    def __init__(self, name, sell_in, quality):
        self.name = name
        self.sell_in = sell_in
        self.quality = quality

    def __repr__(self):
        return "%s, %s, %s" % (self.name, self.sell_in, self.quality)
