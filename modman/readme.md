# Module Manager
Interface to control rover applications via TCP.

## Usage
* `start modname`	Start module modname.
* `stop modname`	Stop module modname.
* `query modname`	Request status of module modname.
* `startall`		Start all modules.
* `stopall`			Stop all modules.
* `queryall`		Request status of all modules.

Modules are added as entries to the `modlist` dictionary in modman.py:
	`module name:[[module executable and arguments]]`
