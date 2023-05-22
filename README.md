# Overview
Tool to collect, process and analyze attention/hyperactivity behavior of a computer user.

The goal is to track accurately the foreground processes running on user machine
and how often they are running. Run as a daemon, it that tracks this information 
and stores it in a format that is used to further analysis of user behavior like 

* How often on average user switches between processes ?
* What is the attention span given to each processes ?
* Under what categories does each of the processes fall into ?
* What inference can we draw from the change in attention span of the user ?
* Does the user suffer from attention disorder ?

# Dependencies

* python 3.9+
* poetry (build)
* gcc/clang (c ext)

# Getting started

I would advice creating a virtual environment localize installation files.

Install
--------
```
$ pip install poetry
$ poetry install
```

Testing
-------
```
$ poetry run pytest
```



# License

MIT License Copyright (c) 2023 beyonddream
