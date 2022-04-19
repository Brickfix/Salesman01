cd docs
start /w doxygen.exe
start /w .\make.bat html
start /W python -m clearTmpStuff
start .\build\html\index.html

:end