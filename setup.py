from distutils.core import setup, Extension
setup(name="_educollections",
      version="1.0",
      author='Nicholas A. Kraft',
      author_email='nkraft@cs.ua.edu',
      url='http://github.com/nkraft/educollections',
      description='Collections for demonstrating order notation',
      platforms='any',
      ext_modules=[Extension('_educollections', ['_educollectionsmodule.c', '_educollectionslists.c'])])
