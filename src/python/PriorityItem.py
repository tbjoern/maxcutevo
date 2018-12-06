class PriorityItem(object):

	def __repr__(self):
		raise NotImplementedError("derived classes must implement __repr__(self)")

	def __lt__(self, other):
		raise NotImplementedError("derived classes must implement __lt__(self, other)")

	def inverse(self):
		raise NotImplementedError("derived classes must implement inverse(self)")

	def key(self):
		raise NotImplementedError("derived classes must implement key(self)")
