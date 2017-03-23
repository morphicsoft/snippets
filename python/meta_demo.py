#!/usr/bin/python

class _MyMeta(type):

	def __init__(cls, *args, **kwargs):
		print "Initialising class", cls
		type.__init__(cls, *args, **kwargs)


class Base(object):

	__metaclass__ = _MyMeta


class A(Base):

	def __init__(self):
		print "Initialising instance", self


class B(object):

	def __init__(self):
		print "Initialising instance", self


if __name__ == '__main__':

	a = A()
	b = B()

"""
Also see what happens if this file is imported from elsewhere.
"""
