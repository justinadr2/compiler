@if %1 == main (
    cl /EHsc /Fo:int/ %1.cpp ast.cpp lexer.cpp parser.cpp builder.cpp
) else if %1 == try (
    cl /EHsc /Fo:int/ %1.c
) else if %1 == z (
    cl /EHsc /Fo:int/ %1.cpp
)

%1.exe