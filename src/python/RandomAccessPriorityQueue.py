
## Python implementation of a random access priority queue
# taken from:
# https://blog.inferentialist.com/2016/10/05/random-access-priority-queue.html

import PriorityItem

import itertools
import string
import random

class MinMaxRandomAccessPriorityQueue():
	def __bool__(self):
		return self.min_queue.__bool__()

	def __init__(self, cls):
		self.min_queue = RandomAccessPriorityQueue(cls)
		self.max_queue = RandomAccessPriorityQueue(cls)

	def insert(self, item):
		inverse = item.inverse()
		self.min_queue.insert(item)
		self.max_queue.insert(inverse)

	def get_min(self):
		return self.min_queue.front()

	def get_max(self):
		return self.max_queue.front()

	def get(self, key):
		return self.min_queue.front()

	def remove(self, key):
		if key is None:
			raise ValueError("heap remove: key must not be None")
		self.min_queue.pop(key)
		self.max_queue.pop(key)

	def pop_min(self, name=None):
		item = self.min_queue.pop(name)
		self.max_queue.pop(item.key())
		return item

	def pop_max(self,name=None):
		item = self.max_queue.pop(name)
		self.min_queue.pop(item.key())
		return item.inverse()

	def get_geq(self, threshold):
		items = self.max_queue.get_leq_to(threshold.inverse())
		return [x.inverse() for x in items]

	def __contains__(self, key):
		return key in self.min_queue


class RandomAccessPriorityQueue():

	def __bool__(self):
		return len(self._data) != 0

	def __contains__(self, key):
		return key in self._lookup

	def __init__(self, cls, fixed_size=None):
		self._cls = cls
		if not issubclass(cls, PriorityItem.PriorityItem):
			raise TypeError("%s must be a subclass of PriorityItem" % (cls.__name__))
		self._data = [ ]
		self._lookup = { } 
		self._fixed_size = fixed_size

	# insert a new item
	def insert(self, item):
		if type(item) != self._cls:
			err_msg = "insert requires type {0}".format(self._cls.__name__)
			raise TypeError( err_msg )

		if self._fixed_size is not None and self._fixed_size == len(self._data):
			if self._data[0] < item:
				self.pop()
			else:
				return

		self._data.append(item)
		ilast = len(self._data) - 1
		self._lookup[item.key()] = ilast
		self._fixup( ilast )

	# return the lowest priority node (or the one requested)
	def pop(self, name=None):
		if name is None:
			istart = 0
		else:
			istart = self._lookup[name]

		if 0 <= istart and istart < len(self._data):
			return self._fixdown(istart)

	def front(self):
		return self._data[0]

	# invariant:  move child up the tree until higher priority than parent
	def _fixup(self, i):
		if i > 0:
			ip = int((i-1)/2)
			if self._data[i] < self._data[ip]:
				self._swap(i, ip)
				self._fixup(ip)

	# invariant:  move down the tree promoting the lesser of the two children
	def _fixdown(self, i):
		sz = len(self._data)
		il = 2*i + 1
		ir = 2*i + 2

		## two children
		if il < sz and ir < sz:
			if self._data[il] < self._data[ir]:
				self._swap(i, il)
				return self._fixdown(il)
			else:
				self._swap(i,ir)
				return self._fixdown(ir)
		## only the left child
		elif il < sz:
			self._swap(i,il)
			return self._fixdown(il)
		## only the right child
		elif ir < sz:
			raise AssertionError("left child missing; right child not")
		## a leaf node
		##   swap with the last item in the array; 
		##   fixup the last item in the new location; 
		##   remove the requested item from its current location at the end of array
		else:
			ilast = len(self._data) - 1
			
			if i < ilast:
				self._swap(i, ilast)
				self._fixup(i)

			res = self._data[ilast]
			key = res.key()
			del self._data[-1]
			del self._lookup[key]

			return res

	# keep the lookup table and data arrays synced
	def _swap(self, i, j):
		key_i = self._data[i].key()
		key_j = self._data[j].key()

		tmp = self._data[i]
		self._data[i] = self._data[j]
		self._data[j] = tmp

		iidx = self._lookup[key_i]
		jidx = self._lookup[key_j]
		self._lookup[key_i] = jidx
		self._lookup[key_j] = iidx

	def get_leq_to(self, threshold):
		return self._leq_traverse(0, threshold)

	def _leq_traverse(self, index, threshold):
		if self._data[index] > threshold:
			return []
		values = [self._data[index]]
		sz = len(self._data)
		li = 2*index + 1
		ri = 2*index + 2
		if li < sz:
			values += self._leq_traverse(li, threshold)
		if ri < sz:
			values += self._leq_traverse(ri, threshold)
		return values
		
