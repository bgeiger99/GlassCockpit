#!/usr/bin/python
# This script autogenerates FlightGear_Protocol.h from the  FlightGear generic
# protocol XML description (e.g. FG_OpenGC.xml)
#
# Hugo Vincent, 22 April 2006

import sys
from elementtree.ElementTree import ElementTree

# Recursively parse XML file, building a parse tree
def parse_file(element, parent, parse_tree=[]):

	# Ignore these tags:
	if element.tag in ["format", "factor", "offset", "binary_mode"]: return

	if element.tag == "binary_footer":
		print "FIXME binary footer"

	if parent.tag == "output" and element.tag == "chunk":
		parse_tree.insert(0, {"name":"", "type":"", "node":""})

	if element.text:
		text = element.text.strip()
		if text != "":
			if element.tag == "name":
				parse_tree[0]["name"] = text
			elif element.tag == "type":
				parse_tree[0]["type"] = text
			elif element.tag == "node":
				parse_tree[0]["node"] = text
			
	for child in element.getchildren():
		parse_tree = parse_file(child, element)

	return parse_tree

# Translate the parse tree into a C header file and write it to filename
def write_output(filename, parse_tree):
	f = open(filename, "w")
	f.write("// Autogenerated file, do no edit\n\nclass FGData {\n")

	for chunk in parse_tree:
		# Rename types
		type = chunk["type"]
		if type == "float": type = "double"
		if type == "": type = "int"

		f.write("\t%s %s;\t// FlightGear property: %s\n" % (type, 
				chunk["name"], chunk["node"]))

	f.write("\n\tunsigned int magic;\n};\n")

if __name__ == "__main__":
	if len(sys.argv) != 3:
		print "Usage: fgXMLtoHeader.py FG_OpenGC.xml output.h"
		print "Copy output.h over ../Source/DataSources/FlightGear_Protocol.h"
		sys.exit(1)

	# Open the file, check it has the "PropertyList/generic/output"
	root = ElementTree(file=sys.argv[1]).getroot()
	if root == None or root.tag != "PropertyList":
		print "File is not a property list"
		sys.exit(1)

	# Get the output section
	output_sec = root.find("generic/output")
	if output_sec == None:
		print "File has no generic/output section"
		sys.exit(1)

	# Parse the file and write the result to a file
	parse_tree = parse_file(output_sec, output_sec)
	write_output(sys.argv[2], parse_tree)
