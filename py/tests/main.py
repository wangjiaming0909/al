import sys
from import_test import title
import import_test2

print(sys.platform)
print(sys.byteorder)
print(sys.builtin_module_names)
print(sys.copyright)
print(sys.path)
print('-----------------')
print(title)
print(import_test2.title)

print('-----------------')
print(len(title))
print(title.count('i', 0, len(title)))
print(title.count('l'))
print(title.count('t'))
print(title.count('e'))
print(title.count('le'))
print(title.find('le'))
print(title.find('t'))

try:
    title.index('a')
    pass
except ValueError as e:
    print(e.args)
    pass
