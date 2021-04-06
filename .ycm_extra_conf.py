INCD = [ '-I./include/libme' ]
C_CFLAGS = INCD + [ '-xc',  ]
CXX_CFLAGS = INCD + [ '-xc++', '-g', '-Wall', '-Wextra', '-std=c++20' ]
OBJC_CFLAGS = INCD + [ '-xobjc',  ]

SOURCES = {
	'/home/edvin/Dokument/libme/src/memory.cpp': CXX_CFLAGS + [  ],
	'/home/edvin/Dokument/libme/src/filesystem.cpp': CXX_CFLAGS + [  ],
	'/home/edvin/Dokument/libme/src/ios.cpp': CXX_CFLAGS + [  ]
}

def Settings(**kwargs):
	key = kwargs['filename']
	if key in SOURCES:
		return { 'flags': SOURCES[key] }
	else:
		if IsCFile(key):
			return { 'flags': C_CFLAGS }
		elif IsCXXFile(key):
			return { 'flags': CXX_CFLAGS }
		elif IsOBJCFile(key):
			return { 'flags': OBJC_CFLAGS }
	return { 'flags': [ ] }

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

def IsOBJCFile(file):
	OBJC_FILES = ['.h', '.m', '.mm', '.M']
	for ext in OBJC_FILES:
		if file.endswith(ext):
			return True
	return False

