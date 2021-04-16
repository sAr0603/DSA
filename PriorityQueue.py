import heapq
import functools
from typing import *


def min_heap_comp(x, y):
	x < y


def max_heap_comp(x, y):
	x > y


@functools.total_ordering
class heap_node:
	def __init__(self, val=0, key=lambda x: x, comp=min_heap_comp):
		self.val = val
		self.key = key
		self.comp = comp

	def __lt__(self, other):  # set comparator as the comparator supplied in constructor
		x = self.key(self.val)
		y = self.key(other.val)
		return self.comp(x, y)

	def __eq__(self, other):
		return self.val == other

	def __repr__(self):
		return self.val.__repr__()

	def __str__(self):
		return self.val.__str__()


class PriorityQueue:
	"""Variant of Queue that retrieves open entries in priority order.
	"""

	def __init__(self, iterable: list[Any] = ()
				 , key=lambda x: x
				 , comp=min_heap_comp
				 , e: Any = None):
		"""
		creates priority queue from any iterable or empty list []

		:param iterable: transform iterable to priority queue outplace
		:param key: extract the paramter on which priority will be alloted
		:param e: Identity element which is returned when safe_fns are called
		"""
		self.__key = key
		self.__comp = comp
		self.__heap: list[heap_node] = [self.__make_node(x) for x in iterable]
		self.__e: Any = e
		heapq.heapify(self.__heap)

	def __len__(self) -> int:
		return len(self.__heap)

	def __repr__(self) -> str:
		return [x.val for x in self.__heap].__repr__()

	def __str__(self) -> str:
		return [x.val for x in self.__heap].__str__()

	def __eq__(self, other) -> bool:
		return self.__heap.__eq__(other)

	def __bool__(self) -> bool:
		return bool(self.__heap.__len__())

	def __hash__(self) -> int:
		return self.__heap.__hash__()

	def __iter__(self):
		return [x.val for x in self.__heap].__iter__()

	def top(self):
		"""

		:return: smallest element in heap
		"""
		return self.__heap[0].val

	def safe_top(self):
		"""

		:return: heap.top() if heap is not empty else identity element
		"""
		if not self.__heap:
			return self.__e
		else:
			return self.top()

	def append(self, val) -> None:
		"""
		heap.append

		:param val: key to be pushed in heap
		"""
		heapq.heappush(self.__heap, self.__make_node(val))

	def pop(self):
		"""
		heap.pop()

		:return: smallest value in heap according to comp fn
		"""
		return heapq.heappop(self.__heap).val

	def safe_pop(self):
		"""
		heap.safe_pop()

		:return: heap.pop() if heap is not empty else identity element
		"""
		if not self.__heap:
			return self.__e
		else:
			return self.pop()

	def __make_node(self, x) -> heap_node:
		"""
		heap_node creator

		:return: creates a node with given key filter and comparator fn and val
		"""
		return heap_node(x, self.__key, self.__comp)
