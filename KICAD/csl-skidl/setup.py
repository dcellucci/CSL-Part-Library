import os
from setuptools import setup

# Utility function to read the README file.
# Used for the long_description.  It's nice, because now 1) we have a top level
# README file and 2) it's easier to type in the README file than to put a raw
# string in below ...
def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

setup(
    name = "skullkid",
    version = "0.0.1",
    author = "Daniel Cellucci",
    author_email = "dwcellucci@gmail.com",
    description = ("Python schematic programming + automatic code generation"),
    license = "BSD",
    keywords = "embedded microcontroller schematic kicad",
    url = "http://packages.python.org/an_example_pypi_project",
    packages=['skullkid', 'tests'],
    long_description=read('README.MD'),
    classifiers=[
        "Development Status :: 1 - Planning",
        "Topic :: Software Development :: Embedded Systems",
        "License :: OSI Approved :: BSD License",
    ],
)