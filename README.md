# Overview
ScreenTime like tool to collect, process and analyze attention/hyperactivity behavior of a computer user.

The goal is to track accurately the foreground processes running on user machine
and how often they are running. Run as a daemon, it that tracks this information 
and stores it in a format that is used to further analysis of user behavior like 

* How often on average user switches between processes ?
* What is the attention span given to each processes ?
* Under what categories does each of the processes fall into ?
  
# Tool dependencies

* Conda (optional - for managing multiple python versions)
* python 3.9+
* poetry (build/test)
* gcc/clang (to build c extension)
* Macos (only supported OS for now)

# Getting started

I would advice creating a virtual environment to localize installation files, either via
venv or conda. See below on how to create a python virtual environment using conda.

```
$ conda create -n py39 python=3.9

# new python 3.9 virtual environment created.

$ conda activate py39

# you are placed inside the python 3.9 virtual environment.

$ conda deactivate

# you are back to regular shell environment.

```

Install
--------
```
# assuming virtual environment is activated from here onwards
$ pip install poetry
$ poetry install
```

Building
-------
```
$ poetry build
```

Testing
-------
```
$ poetry run pytest
```

# License

MIT License Copyright (c) 2023 beyonddream
