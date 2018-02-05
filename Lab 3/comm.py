#!/usr/bin/python

import random, sys, string, locale, argparse
from optparse import OptionParser

locale.setlocale(locale.LC_ALL, 'C')

class lines:
	def __init__(self, fileInput):
		if fileInput == "-":
			self.lines = sys.stdin.readlines()
		else:
			fileOpener = open(fileInput, 'r')
			self.lines = fileOpener.readlines()
			fileOpener.close()
		



def main():
	version_msg= "%prog 2.0"
	usage_msg= """%prog [OPTION]... FILE1 FILE2

	Compare files line by line and output 3 columns, one for lines unique to FILE1, one for lines unique to FILE2, and lines common to both files"""

	parser = argparse.ArgumentParser()
	parser.add_argument("-u", action="store_true", dest="unsorted", help = "compares files that are unsorted", default = 0)
	parser.add_argument("-1", action="store_true", dest="one", help = "suppress printing of column 1", default = 0)
	parser.add_argument("-2", action="store_true", dest="two", help = "suppress printing of column 2", default = 0)
	parser.add_argument("-3", action="store_true", dest="three", help = "suppress printing of column 3", default = 0)
	parser.add_argument("file1", action="store")
	parser.add_argument("file2", action="store")
	options=parser.parse_args()

	if options.file1 == "-" and options.file2 == "-":
		parser.error("Need at least one file not from standard input")

	lines1 = lines(options.file1)
	lines2 = lines(options.file2)

	leadingTab2=""
	leadingTab3=""

	if options.one==0:
		leadingTab2="\t"
	if options.one==0 and options.two==0:
		leadingTab3="\t\t"
	elif options.one==0 or options.two==0:
		leadingTab3="\t"



	# unsorted input

	if options.unsorted:
		for line1 in lines1.lines:
			if line1 in lines2.lines:
				lines2.lines.remove(line1)
				if options.three==0:
					sys.stdout.write(leadingTab3+line1)
			else:
				if options.one==0:
					sys.stdout.write(line1)
		for line2 in lines2.lines:
			if options.two==0:
				sys.stdout.write(leadingTab2+line2)
	else:
		i1=0
		i2=0
		endOne=len(lines1.lines)
		endTwo=len(lines2.lines)
		while ((i1<endOne) and (i2<endTwo)):
			if lines1.lines[i1]==lines2.lines[i2]:
				if options.three==0:
					sys.stdout.write(leadingTab3+lines1.lines[i1])
				i1+=1
				i2+=1
			elif lines1.lines[i1]<lines2.lines[i2]:
				if options.one==0:
					sys.stdout.write(lines1.lines[i1])
				i1+=1
			else:
				if options.two==0:
					sys.stdout.write(leadingTab2+lines2.lines[i2])
				i2+=1
		if i1!=endOne:
			while i1<endOne:
				if options.one==0:
					sys.stdout.write(lines1.lines[i1])
				i1+=1
		if i2!=endTwo:
			while i2<endTwo:
				if options.two==0:
					sys.stdout.write(leadingTab2+lines2.lines[i2])
				i2+=1

if __name__=="__main__":
	main()








