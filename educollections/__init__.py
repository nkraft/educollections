# Copyright (c) 2013, Nicholas A. Kraft
# All rights reserved.

# Distributed under a BSD-style license. (See accompanying file LICENSE or
# copy at http://github.com/nkraft/educollections/LICENSE)

"""Collections for demonstrating order notation."""


__all__ = ['List', 'ArrayList', 'SinglyLinkedList1', 'SinglyLinkedList2']


import abc
from _educollections import ArrayList, SinglyLinkedList1, SinglyLinkedList2


class Collection(metaclass=abc.ABCMeta):

    __slots__ = ()

    @abc.abstractmethod
    def size(self):
        """Returns the size of this List."""


class List(Collection):

    __slots__ = ()

    @abc.abstractmethod
    def append(self, item):
        """Adds the given item to the end of this List."""

    @abc.abstractmethod
    def clear(self):
        """Clears this List."""

    @abc.abstractmethod
    def get(self, index):
        """Returns the item at the given index in this List."""

    @abc.abstractmethod
    def insert(self, index, item):
        """Inserts the given item at the given index in this List."""

    @abc.abstractmethod
    def prepend(self, item):
        """Adds the given item to the front of this List."""

    @abc.abstractmethod
    def remove(self, index):
        """Removes and returns the item at the given index in this List."""

    @abc.abstractmethod
    def set(self, index, item):
        """Assigns the given item at the given index in this List."""


List.register(ArrayList)
List.register(SinglyLinkedList1)
List.register(SinglyLinkedList2)
