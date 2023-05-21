# -*- coding: utf-8 -*-
from setuptools import setup

package_dir = \
{'': 'src'}

packages = \
['what_the_proc']

package_data = \
{'': ['*'], 'what_the_proc': ['ext/*']}

setup_kwargs = {
    'name': 'what-the-proc',
    'version': '0.1.0',
    'description': 'Tool to collect, process and analyze attention, hyperactivity behavior of a computer user.',
    'long_description': '# Overview\nTool to collect, process and analyze attention/hyperactivity behavior of a computer user.\n\nThe goal is to track accurately the foreground processes running on user machine\nand how often they are running. Run as a daemon, it that tracks this information \nand stores it in a format that is used to further analysis of user behavior like \n\n* How often on average user switches between processes ?\n* What is the attention span given to each processes ?\n* Under what categories does each of the processes fall into ?\n* What inference can we draw from the change in attention span of the user ?\n* Does the user suffer from attention disorder ?\n\n# Dependencies\n\n* python 3.9+\n* poetry (build)\n* gcc/clang (c ext)\n\n# License\n\nMIT License Copyright (c) 2023 beyonddream\n',
    'author': 'Arun a.k.a beyonddream',
    'author_email': None,
    'maintainer': None,
    'maintainer_email': None,
    'url': None,
    'package_dir': package_dir,
    'packages': packages,
    'package_data': package_data,
    'python_requires': '>=3.9,<4.0',
}
from build import *
build(setup_kwargs)

setup(**setup_kwargs)
