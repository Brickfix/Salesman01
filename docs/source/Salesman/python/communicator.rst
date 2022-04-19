Communicator
============

Functions used to execute the Salesman01.exe with the subprocesses module,
returns the result.

Creating a command list
-----------------------
Following function allows quick creation of a list containing valid commands

.. autofunction:: Salesman.python.communicator.command_list

Converting Byte output
----------------------
The read-in results are returned in byte code, `convert_output` ensures a correct conversion
for future use.

.. autofunction:: Salesman.python.communicator.convert_output

Run Executable
--------------
This function executes the compiled C++ code.

.. autofunction:: Salesman.python.communicator.run_salesman_exe