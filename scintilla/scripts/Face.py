# Face.py - module for reading and parsing Scintilla.iface file
# Implemented 2000 by Neil Hodgson neilh@scintilla.org
# Released to the public domain.
# Requires Python 2.5 or later

def sanitiseLine(line):
	if line[-1:] == '\n': line = line[:-1]
	if line.find("##") != -1:
		line = line[:line.find("##")]
	line = line.strip()
	return line

def decodeFunction(featureVal):
	retType, rest = featureVal.split(" ", 1)
	nameIdent, params = rest.split("(")
	name, value = nameIdent.split("=")
	params, rest = params.split(")")
	param1, param2 = params.split(",")
	return retType, name, value, param1, param2

def decodeEvent(featureVal):
	retType, rest = featureVal.split(" ", 1)
	nameIdent, params = rest.split("(")
	name, value = nameIdent.split("=")
	return retType, name, value

def decodeParam(p):
	param = p.strip()
	type = ""
	name = ""
	value = ""
	if " " in param:
		type, nv = param.split(" ")
		if "=" in nv:
			name, value = nv.split("=")
		else:
			name = nv
	return type, name, value

def IsEnumeration(t):
	return t[:1].isupper()

class Face:

	def __init__(self):
		self.order = []
		self.features = {}
		self.values = {}
		self.events = {}
		self.aliases = {}

	def ReadFromFile(self, name):
		currentCategory = ""
		currentComment = []
		currentCommentFinished = 0
		maxInt = (1 << 31) - 1
		lineno = 0
		for line in open(name).readlines():
			lineno += 1
			line = sanitiseLine(line)
			if line:
				if line[0] == "#":
					if line[1] == " ":
						if currentCommentFinished:
							currentComment = []
							currentCommentFinished = 0
						currentComment.append(line[2:])
				else:
					currentCommentFinished = 1
					featureType, featureVal = line.split(" ", 1)
					if featureType in ["fun", "get", "set"]:
						try:
							retType, name, value, param1, param2 = decodeFunction(featureVal)
						except ValueError:
							print("Failed to decode line %d: %s" % (lineno, line))
							raise
						p1 = decodeParam(param1)
						p2 = decodeParam(param2)
						self.features[name] = {
							"FeatureType": featureType,
							"ReturnType": retType,
							"Value": value,
							"Param1Type": p1[0], "Param1Name": p1[1], "Param1Value": p1[2],
							"Param2Type": p2[0], "Param2Name": p2[1], "Param2Value": p2[2],
							"Category": currentCategory, "Comment": currentComment
						}
						if value in self.values:
							raise Exception("Duplicate value " + value + " " + name + " on line:" + str(lineno))
						self.values[value] = 1
						self.order.append(name)
						currentComment = []
					elif featureType == "evt":
						retType, name, value = decodeEvent(featureVal)
						self.features[name] = {
							"FeatureType": featureType,
							"ReturnType": retType,
							"Value": value,
							"Category": currentCategory, "Comment": currentComment
						}
						if value in self.events:
							raise Exception("Duplicate event " + value + " " + name + " on line:" + str(lineno))
						self.events[value] = 1
						self.order.append(name)
					elif featureType == "cat":
						currentCategory = featureVal
					elif featureType == "val":
						try:
							name, value = featureVal.split("=", 1)
							if value[0] == '-':
								# add parenthesis for negative value
								value = '(' + value + ')'
							elif int(value, 0) > maxInt:
								# unsigned value
								value = value + 'U'
						except ValueError:
							print("Failure line %d: %s" % (lineno, featureVal))
							raise Exception()
						self.features[name] = {
							"FeatureType": featureType,
							"Category": currentCategory,
							"Value": value }
						self.order.append(name)
					elif featureType == "enu" or featureType == "lex":
						name, value = featureVal.split("=", 1)
						self.features[name] = {
							"FeatureType": featureType,
							"Category": currentCategory,
							"Value": value,
							"Comment": currentComment }
						self.order.append(name)
						currentComment = []
					elif featureType == "ali":
						# Enumeration alias
						name, value = featureVal.split("=", 1)
						self.aliases[name] = value
						currentComment = []

