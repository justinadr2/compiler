@if %1 == main (
    cl /EHsc /Fo:int/ %1.cpp ast.cpp lexer.cpp parser.cpp
) else if %~1 == try (
    cl /EHsc /Fo"int/" %1.cpp
) 

%1.exe