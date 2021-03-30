CFLAGS = [  ]
C_CFLAGS = CFLAGS + [ '-xc' ]
CXX_CFLAGS = CFLAGS + [ '-xc++', '-g', '-Wall', '-Wextra', '-std=c++20', '-I./include/libme' ]

SOURCES = {
	'/home/edvin/Dokument/libme/src/memory.cpp': CXX_CFLAGS + [  ],
	'/home/edvin/Dokument/libme/src/variant.cpp': CXX_CFLAGS + [  ],
	'/home/edvin/Dokument/libme/src/filesystem.cpp': CXX_CFLAGS + [  ]
}

def Settings(**kwargs):
	key = kwargs['filename']
	if key in SOURCES:
		return { 'flags': SOURCES[key] }
	else:
		if IsCXXFile(key):
			return { 'flags': CXX_CFLAGS }
		elif IsCFile(key):
			return { 'flags': C_CFLAGS }
	return { 'flags': CFLAGS }

def IsCXXFile(file):
	CXX_FILES = ['.cpp', '.cxx', '.cc', '.hpp', '.hxx', '.hh']
	for ext in CXX_FILES:
		if file.endswith(ext):
			return True
	return False

def IsCFile(file):
	C_FILES = ['.c', '.h']
	for ext in C_FILES:
		if file.endswith(ext):
			return True
	return False

