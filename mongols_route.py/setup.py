import sys
try:
    from setuptools import setup,find_packages
except ImportError:
    from distutils.core import setup

import mongols_route

setup(name='mongols_route',
      version=mongols_route.__version__,
      description='route for pymongols',
      author=mongols_route.__author__,
      author_email=mongols_route.__author__,
      url='https://mongols.hi-nginx.com/doc/python.html',
      py_modules=['mongols_route'],
      packages= find_packages(),
      install_requires=['Jinja2>=2'],
      scripts=['mongols_route.py'],
      license='MPL-v3',
      platforms='LINUX',
      )
