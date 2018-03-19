import re

#seen on https://github.com/loisaidasam/lirc-python
class Lirc:

	def __init__(self, config):
		self.name = None
		self.id = None		
		self.codes = []
		
		codes_section = False
		
		with open(config) as f:
			content = f.readlines()
		
		try:
			i = 0
			for line in content:
			
				line = re.sub(r'[ \t]+', ' ', line)
				line = re.sub(r'#.*', '', line)
				line = line.strip()
				i += 1
				if line:
					if not codes_section and line.startswith('name'):
						self.name = line.split(' ')[1]

					elif not codes_section and line.startswith('pre_data'):
						self.id = line.split(' ')[1][2:]

					elif line == 'begin codes':
						codes_section = True

					elif line == 'end codes':
						codes_section = False

					elif codes_section:
						fields = line.split(' ')
						self.codes.append({"name":fields[0], "code":str(self.id) + fields[1][2:]})
		except:
			self.codes.append({"name":"lirc.conf error on line %s"%i, "code":""})