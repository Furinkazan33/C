# property_parser

Parsing property files containing any type of data (arrays, objects, basic types).

Example : 
i
```
	[section_1]
	char1 = 'c'
	int1 = 4

	[section_2]
	double1 = 3.14

	string1 = "ok"
	string2 = "ok 2\n"

	pointer=string1
	my_array = [ "all types array", 'c', 2, [ "an", "other array", pointer] ]

	obj = { toto = 2, s=my_array, tab = [1, 2, 3] }
 ```

